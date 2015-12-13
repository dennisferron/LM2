#pragma once

#include <boost/interprocess/offset_ptr.hpp>

typedef boost::interprocess::offset_ptr<void> ColumnDataPtr;
typedef boost::interprocess::offset_ptr<ColumnDataPtr> ColumnDataPtrArray;

struct TableHeader
{
    ColumnDataPtrArray column_data;
    std::size_t row_count;
    std::size_t max_row_count;
    bool is_allocated;
    bool is_writing;
    bool is_reading;
};
