#pragma once

#include <cstddef>
#include "ITableDataAllocator.hpp"
#include "ITable.hpp"
#include "Schema.hpp"
#include "IView.hpp"

class Table : public ITable
{
private:
    Schema schema;
    ITableDataAllocator* allocator;
    TableHeader* header;
    IRowSet* open_row_set;

    // Does not preserve existing data!
    void resize(std::size_t new_size);

    size_t row_count() const;

    IRowSet* build_row_set();

public:

    Table(Schema schema_, ITableDataAllocator* allocator_, std::size_t reserve_rows);
    Table(Schema schema_, ITableDataAllocator* allocator_, TableHeader* header_);

    virtual IRowSet* open_for_reading() override;
    virtual void close_for_reading(IRowSet* row_set) override;
    virtual IRowSet* open_for_writing(std::size_t new_size) override;
    virtual void close_for_writing(IRowSet* row_set) override;

    void* get_column_data(ColumnInfo const& columnInfo) const;
};
