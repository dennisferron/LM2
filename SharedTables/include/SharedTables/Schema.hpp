#pragma once

#include <string>
#include <vector>

#undef DEFINE_SCHEMA_TYPES
#undef DEFINE_SCHEMA_ENUM

#define DEFINE_SCHEMA_ENUM
#include "SchemaXMacros.hpp"
#undef DEFINE_SCHEMA_ENUM

#define DEFINE_SCHEMA_TYPES
#include "SchemaXMacros.hpp"
#define DEFINE_SCHEMA_TYPES

struct ColumnInfo
{
    std::size_t cell_extent;
    std::size_t cell_length;
    std::size_t data_type_size;
    std::string column_name;
    std::size_t column_index;
    DataTypes data_type;
};

struct Schema
{
    std::string table_name;
    std::vector<ColumnInfo> columns;

    Schema();
    Schema(std::string name);
    void add_column(std::string name, DataTypes data_type, std::size_t size);
    ColumnInfo get_column(std::string name);
};
