#include "Table.hpp"
#include "Exceptions.hpp"
#include "TableHeader.hpp"

#include "boost/interprocess/detail/cast_tags.hpp"
#include "TableRowSet.hpp"

using namespace std;
using namespace boost;
using namespace boost::interprocess;
using namespace boost::interprocess::ipcdetail;

IRowField::~IRowField() {}

IRowSet* Table::build_row_set()
{
    TableRowSet* result = new TableRowSet(row_count());
    for (ColumnInfo const& column_info : schema.columns)
    {
        void* data_ptr = get_column_data(column_info);
        result->add_field(data_ptr, column_info);
    }
    return result;
}

void* Table::get_column_data(ColumnInfo const& column_info) const
{
    size_t index = column_info.column_index;
    ColumnDataPtr* array = header->column_data.get();
    ColumnDataPtr ptr = array[index];
    void* result = ptr.get();
    return result;
}

// Does not preserve existing data!
void Table::resize(size_t new_size)
{
    header->is_allocated = false;

    if (new_size > header->max_row_count)
    {
        vector<ColumnInfo> const & columns = schema.columns;
        for (int i=0; i < columns.size(); i++)
        {
            allocator->deallocate_column(
                    columns[i].column_name,
                    header->column_data[i].get());
        }

        // resize all of the column_data arrays
        header->row_count = 0;
        header->max_row_count = new_size;

        for (int i=0; i < columns.size(); i++)
        {
            header->column_data[i] = allocator->allocate_column(
                    columns[i].column_name,
                    header->max_row_count * columns[i].cell_extent);
        }
    }

    header->row_count = new_size;

    header->is_writing = false;
    header->is_allocated = true;
}

Table::Table(Schema schema_, ITableDataAllocator* allocator_, std::size_t reserve_rows)
: schema(schema_), allocator(allocator_), open_row_set(nullptr)
{
    header = allocator->construct_table_header(schema.table_name);
    header->is_allocated = false;
    header->is_writing = false;
    header->row_count = 0;
    header->max_row_count = reserve_rows;
    header->column_data = ColumnDataPtrArray((ColumnDataPtr)allocator->allocate_data_ptrs(
            sizeof(ColumnDataPtr) * schema.columns.size()), reinterpret_cast_tag());

    vector<ColumnInfo> const & columns = schema.columns;
    for (int i=0; i < columns.size(); i++)
    {
        header->column_data[i] = ColumnDataPtr(allocator->allocate_column(
                columns[i].column_name,
                header->max_row_count * columns[i].cell_extent));
    }

    header->is_allocated = true;
}

Table::Table(Schema schema_, ITableDataAllocator* allocator_, TableHeader* header_)
: schema(schema_), allocator(allocator_), header(header_)
{
}

size_t Table::row_count() const
{
    if (!header->is_allocated)
        throw InvalidTableException();

    return header->row_count;
}

IRowSet* Table::open_for_writing(size_t new_size)
{
    // TODO: change resize to gracefully handle if table is not allocated by not trying to delete invalid old column data
    if (!header->is_allocated)
        throw InvalidTableException();

    resize(new_size);

    header->is_writing = true;

    return open_row_set = build_row_set();
}

void Table::close_for_writing(IRowSet* row_set)
{
    if (!header->is_allocated)
        throw InvalidTableException();

    if (!header->is_writing)
        throw InvalidOperationException();  // must open for writing first

    if (row_set != open_row_set)
        throw InvalidOperationException();  // row set is not the same one as was opened.

    header->is_writing = false;
    delete row_set;
}

IRowSet* Table::open_for_reading()
{
    if (!header->is_allocated)
        throw InvalidTableException();

    if (header->is_writing)
        throw InvalidOperationException();  // cannot open for reading if open for writing

    header->is_reading = true;

    return open_row_set = build_row_set();
}

void Table::close_for_reading(IRowSet* row_set)
{
    if (!header->is_allocated)
        throw InvalidTableException();

    if (header->is_writing)
        throw InvalidOperationException();  // table is open for writing

    if (!header->is_reading)
        throw InvalidOperationException();  // must open for reading first

    if (row_set != open_row_set)
        throw InvalidOperationException();  // row set is not the same one as was opened.

    header->is_reading = false;
    delete row_set;
}
