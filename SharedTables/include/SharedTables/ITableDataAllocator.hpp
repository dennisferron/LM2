#pragma once

#include <string>

class TableHeader;

class ITableDataAllocator
{
public:
    virtual ~ITableDataAllocator();
    virtual TableHeader* construct_table_header(std::string table_name) = 0;
    virtual void* allocate_column(std::string column_name, std::size_t nbytes) = 0;
    virtual void deallocate_column(std::string column_name, void* addr) = 0;
    virtual void* allocate_data_ptrs(size_t nbytes) = 0;
    virtual void deallocate_data_ptrs(void* addr) = 0;
};
