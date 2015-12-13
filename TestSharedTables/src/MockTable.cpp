#include "MockTable.hpp"

IRowField& MockRowView::operator[](std::string column_name)
{
    return *(fields[column_name]);
}

MockRowView::~MockRowView()
{
    for (auto iter : fields)
    {
        delete iter.second;
    }
}
MockRowSet::MockRowSet() : num_rows(1)
{
}

IRowView& MockRowSet::current_row()
{
    return row;
}

std::size_t MockRowSet::total_rows()
{
    return num_rows;
}

void MockRowSet::move_next()
{
    // do nothing
}

IRowSet *MockTable::open_for_reading()
{
    return &row_set;
}

void MockTable::close_for_reading(IRowSet *row_set)
{

}

IRowSet *MockTable::open_for_writing(std::size_t new_size)
{
    return &row_set;
}

void MockTable::close_for_writing(IRowSet *row_set)
{

}

