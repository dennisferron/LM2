//
// Created by Dennis Ferron on 10/19/15.
//

#include "SharedTables/Schema.hpp"
#include "SharedTables/Exceptions.hpp"

Schema::Schema()
{

}

Schema::Schema(std::string name)
    : table_name(name)
{

}

void Schema::add_column(std::string name, DataTypes data_type, std::size_t size)
{
    ColumnInfo column;
    column.column_index = columns.size();
    column.column_name = name;
    column.data_type = data_type;
    column.cell_extent = size;
    columns.push_back(column);
}

ColumnInfo Schema::get_column(std::string name)
{
    for (auto c : columns)
        if (c.column_name == name)
            return c;

    throw InvalidColumnException();  // the column name was not found
}
