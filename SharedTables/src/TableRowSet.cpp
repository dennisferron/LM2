#include "TableRowSet.hpp"

TableRowField::TableRowField(void* array_, std::size_t cell_extent_, std::size_t const& pos_)
        : array(array_), pos(pos_), cell_extent(cell_extent_)
{
}

void TableRowField::get_value_impl(void *target)
{
    memcpy(target, (char*)array + (pos*cell_extent), cell_extent);
}

void TableRowField::set_value_impl(void const *source)
{
    memcpy((char*)array + (pos*cell_extent), source, cell_extent);
}

IRowField& TableRowView::operator[](std::string column_name)
{
    return *(fields[column_name]);
}

TableRowView::TableRowView()
        : cur_pos(0)
{

}

TableRowView::~TableRowView()
{
    for (auto iter : fields)
    {
        delete iter.second;
    }
}

void TableRowView::move_next()
{
    ++cur_pos;
}

std::size_t TableRowView::get_pos() const
{
    return cur_pos;
}

void TableRowView::add_field(void *array, std::size_t cell_extent, std::string column_name)
{
    fields[column_name] = new TableRowField(array, cell_extent, this->cur_pos);
}

TableRowSet::TableRowSet(std::size_t num_rows_) : num_rows(num_rows_)
{
}

IRowView& TableRowSet::current_row()
{
    return row_view;
}

std::size_t TableRowSet::total_rows()
{
    return num_rows;
}

void TableRowSet::move_next()
{
    if (row_view.get_pos() >= num_rows)
        throw std::runtime_error("RowSet move_next past last row.");

    row_view.move_next();
}

void TableRowSet::add_field(void *array, ColumnInfo column_info)
{
    row_view.add_field(array, column_info.cell_extent, column_info.column_name);
}
