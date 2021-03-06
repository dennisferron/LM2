#include "gtest/gtest.h"
#include "SharedTables/Table.hpp"
#include "SharedTables/TableHeader.hpp"

#include <iostream>
#include <map>

using namespace std;


class AllocationSource
{
private:
    int times_allocated;
    int times_deallocated;
    std::vector<std::pair<std::size_t, void*>> buffers;

    int find_buffer(void* addr)
    {
        for (int i=0; i<buffers.size(); i++)
        {
            if (buffers[i].second == addr)
                return i;
        }

        return -1;
    }

public:
    AllocationSource()
            : times_allocated(0), times_deallocated(0)
    {}

    void add(void* buffer, std::size_t size)
    {
        buffers.push_back(std::make_pair(size, buffer));
    }

    void* allocate(std::size_t size)
    {
        EXPECT_FALSE(times_allocated > times_deallocated) << "double allocation";
        EXPECT_TRUE(times_allocated < buffers.size() /* allocated more than # of buffers */ );
        EXPECT_TRUE(times_deallocated < buffers.size() /* deallocated more than # of buffers */ );
        EXPECT_EQ(buffers[times_allocated].first /* expected size */, size /* requested size */);
        return buffers[times_allocated++].second;
    }

    void deallocate(void* addr)
    {
        EXPECT_TRUE(times_deallocated < buffers.size() /* deallocated more than # of buffers */ );
        EXPECT_TRUE(times_allocated < buffers.size() /* allocated more than # of buffers */ );
        EXPECT_EQ(buffers[times_allocated-1].second /* expected address */, addr /* requested address */);
        EXPECT_EQ(times_allocated - 1 /* before deallocation expect 1 less than allocations */, times_deallocated);
        ++times_deallocated;
    }

    bool has_buffer(void* addr)
    {
        return find_buffer(addr) != -1;
    }

    bool is_allocated(void* addr)
    {
        EXPECT_TRUE(has_buffer(addr));
        return times_allocated > find_buffer(addr);
    }

    bool is_deallocated(void* addr)
    {
        EXPECT_TRUE(has_buffer(addr));
        return times_deallocated > find_buffer(addr);
    }
};

class FixedAllocator : public ITableDataAllocator
{
private:
    std::map<std::string, AllocationSource> column_data;
    AllocationSource data_ptrs;
    TableHeader header;

public:

    FixedAllocator()
    {
    }

    TableHeader& get_header()
    {
        return header;
    }

    bool column_is_allocated(string column_name, void* buffer)
    {
        EXPECT_TRUE(column_data.find(column_name) != column_data.end());
        return column_data[column_name].is_allocated(buffer);
    }

    bool column_is_deallocated(string column_name, void* buffer)
    {
        EXPECT_TRUE(column_data.find(column_name) != column_data.end());
        return column_data[column_name].is_deallocated(buffer);
    }

    bool data_ptrs_is_allocated(void* buffer)
    {
        return data_ptrs.is_allocated(buffer);
    }

    bool data_ptrs_is_deallocated(void* buffer)
    {
        return data_ptrs.is_deallocated(buffer);
    }

    void add_column(std::string name, void* buffer, std::size_t size)
    {
        column_data[name].add(buffer, size);
    }

    void add_data_ptrs(void* buffer, std::size_t size)
    {
        data_ptrs.add(buffer, size);
    }

    virtual ~FixedAllocator() {}

    virtual void* allocate_column(std::string column_name, std::size_t nbytes)
    {
        EXPECT_TRUE(column_data.find(column_name) != column_data.end());
        return column_data[column_name].allocate(nbytes);
    }

    virtual void deallocate_column(std::string column_name, void* addr)
    {
        EXPECT_TRUE(column_data.find(column_name) != column_data.end());
        column_data[column_name].deallocate(addr);
    }

    virtual void* allocate_data_ptrs(size_t nbytes)
    {
        return data_ptrs.allocate(nbytes);
    }

    virtual void deallocate_data_ptrs(void* addr)
    {
        data_ptrs.deallocate(addr);
    }

    virtual TableHeader *construct_table_header(std::string table_name)
    {
        return &header;
    }
};

class TestTableFixture : public ::testing::Test
{
public:
    static int const max_rows = 100;
    static int const str_size = 10;

    int col0_buf[max_rows];
    char col1_buf[max_rows][str_size];

    std::size_t data_ptrs[2];

    Schema CreateSchema()
    {
        Schema schema;
        schema.table_name = "TestTable";

        ColumnInfo col0;
        col0.column_name = "Col0";
        col0.column_index = 0;
        col0.data_type = DataTypes::dt_int32;
        col0.cell_extent = sizeof(int);

        ColumnInfo col1;
        col1.column_name = "Col1";
        col1.column_index = 1;
        col1.data_type = DataTypes::dt_byte_array;
        col1.cell_extent = str_size;

        schema.columns = { col0, col1 };

        return schema;
    }

    FixedAllocator CreateAllocator(Schema schema)
    {
        FixedAllocator allocator;
        int const str_size = schema.columns[1].cell_extent;
        allocator.add_column("Col0", col0_buf, max_rows * sizeof(int));
        allocator.add_column("Col1", col1_buf, max_rows * str_size);
        allocator.add_data_ptrs(data_ptrs, 2 * sizeof(size_t));
        return allocator;
    }
};


TEST_F(TestTableFixture, TableConstructor)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);

    ASSERT_TRUE(allocator.data_ptrs_is_allocated(data_ptrs));
    ASSERT_TRUE(!allocator.data_ptrs_is_deallocated(data_ptrs));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));


    delete table;
}

TEST_F(TestTableFixture, TableNoResizeSame)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);

    ASSERT_TRUE(allocator.data_ptrs_is_allocated(data_ptrs));
    ASSERT_TRUE(!allocator.data_ptrs_is_deallocated(data_ptrs));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    IRowSet* row_set = table->open_for_writing(max_rows);

    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    table->close_for_writing(row_set);

    delete table;
}

TEST_F(TestTableFixture, TableNoResizeSmaller)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);

    ASSERT_TRUE(allocator.data_ptrs_is_allocated(data_ptrs));
    ASSERT_TRUE(!allocator.data_ptrs_is_deallocated(data_ptrs));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    IRowSet* row_set = table->open_for_writing(max_rows/2);

    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    table->close_for_writing(row_set);

    delete table;
}

TEST_F(TestTableFixture, TableResizeLarger)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);

    ASSERT_TRUE(allocator.data_ptrs_is_allocated(data_ptrs));
    ASSERT_TRUE(!allocator.data_ptrs_is_deallocated(data_ptrs));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    int col0_buf2[2*max_rows];
    char col1_buf2[2*max_rows][str_size];
    allocator.add_column("Col0", col0_buf2, 2*max_rows * sizeof(int));
    allocator.add_column("Col1", col1_buf2, 2*max_rows * str_size);

    IRowSet* row_set = table->open_for_writing(2*max_rows);

    ASSERT_TRUE(allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_deallocated("Col1", col1_buf));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf2));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf2));

    table->close_for_writing(row_set);

    delete table;
}

TEST_F(TestTableFixture, TableOpenForWriting)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);

    ASSERT_TRUE(allocator.data_ptrs_is_allocated(data_ptrs));
    ASSERT_TRUE(!allocator.data_ptrs_is_deallocated(data_ptrs));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    IRowSet* row_set = table->open_for_writing(max_rows);

    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    void* col0 = table->get_column_data(schema.get_column("Col0"));
    ASSERT_EQ(col0_buf, col0);

    void* col1 = table->get_column_data(schema.get_column("Col1"));
    ASSERT_EQ(col1_buf, col1);

    table->close_for_writing(row_set);

    delete table;
}

TEST_F(TestTableFixture, TableOpenForReading)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);

    ASSERT_TRUE(allocator.data_ptrs_is_allocated(data_ptrs));
    ASSERT_TRUE(!allocator.data_ptrs_is_deallocated(data_ptrs));

    ASSERT_TRUE(allocator.column_is_allocated("Col0", col0_buf));
    ASSERT_TRUE(allocator.column_is_allocated("Col1", col1_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    IRowSet* row_set = table->open_for_reading();

    ASSERT_TRUE(!allocator.column_is_deallocated("Col0", col0_buf));
    ASSERT_TRUE(!allocator.column_is_deallocated("Col1", col1_buf));

    void* col0 = table->get_column_data(schema.get_column("Col0"));
    ASSERT_EQ(col0_buf, col0);

    void* col1 = table->get_column_data(schema.get_column("Col1"));
    ASSERT_EQ(col1_buf, col1);

    table->close_for_reading(row_set);

    delete table;
}

TEST_F(TestTableFixture, TableWritingRowSet)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);

    Table* table = new Table(schema, &allocator, max_rows);
    IRowSet* row_set = table->open_for_writing(max_rows);
    IRowView& cur_row = row_set->current_row();

    cur_row["Col0"].set_value(99);

    ASSERT_EQ(99, col0_buf[0]);

    table->close_for_writing(row_set);

    delete table;
}

TEST_F(TestTableFixture, TableReadingRowSet)
{
    Schema schema = CreateSchema();
    FixedAllocator allocator = CreateAllocator(schema);
    Table* table = new Table(schema, &allocator, max_rows);

    col0_buf[0] = 99;

    IRowSet* row_set = table->open_for_reading();

    void* col0 = table->get_column_data(schema.get_column("Col0"));
    ASSERT_EQ(99, *(int*)col0);

    table->close_for_reading(row_set);

    delete table;
}

