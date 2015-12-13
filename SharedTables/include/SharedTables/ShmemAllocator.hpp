#pragma once

#include <cstddef>

class ShmemAllocator : public ITableDataAllocator
{
private:
    managed_shared_memory& segment;

public:

    ShmemAllocator(managed_shared_memory& segment_)
            : segment(segment_) {}

    virtual TableHeader* construct_table_header(string table_name)
    {
        return segment.construct<TableHeader>(table_name.c_str())();
    }

    virtual void* allocate(size_t nbytes)
    {
        return segment.allocate(nbytes);
    }

    virtual void deallocate(void* addr)
    {
        segment.deallocate(addr);
    }
};
