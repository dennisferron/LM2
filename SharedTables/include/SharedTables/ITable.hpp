#pragma once

#include <string>
#include "Schema.hpp"

class IRowSet;

class ITable
{
public:
    virtual IRowSet* open_for_reading() = 0;
    virtual void close_for_reading(IRowSet* row_set) = 0;
    virtual IRowSet* open_for_writing(std::size_t new_size) = 0;
    virtual void close_for_writing(IRowSet* row_set) = 0;
};

// Note:  whether the rowset is deleted after use or not is
// dependent on the implementation of ITable and the specific
// implementation may delete it in the close methods.
// You should not explicitly delete the rowset, nor should
// you assume whether you will get the same vs a different
// rowset each time, and you should not attempt to access
// the rowset after the table is closed.
