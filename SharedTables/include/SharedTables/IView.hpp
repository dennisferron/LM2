#pragma once

#include "Schema.hpp"

class IRowField
{
protected:
    virtual void get_value_impl(void* target) = 0;
    virtual void set_value_impl(void const* source) = 0;

public:
    virtual ~IRowField();

    template <typename T> void get_value(T& target)
    {
        get_value_impl(&target);
    }

    template <typename T> void set_value(T const& source)
    {
        set_value_impl(&source);
    }
};

class IRowView
{
public:
    virtual IRowField& operator[](std::string column_name) = 0;
};

class IRowSet
{
public:
    virtual ~IRowSet() = 0;
    virtual IRowView& current_row() = 0;
    virtual std::size_t total_rows() = 0;
    virtual void move_next() = 0;
};

template <typename T>
struct IDTOReader
{
    virtual ~IDTOReader() = 0;
    virtual void write_dto_into_row(IRowView &row, T const &dto) = 0;
    virtual void read_row_into_dto(IRowView &row, T &dto) = 0;
};

template <typename T>
class IView
{
public:
    virtual std::vector<T>& get_vector() = 0;
    virtual void read_table_into_vector() = 0;
    virtual void write_vector_into_table() = 0;
};
