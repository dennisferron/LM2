#pragma once

#include <map>
#include "ITable.hpp"
#include "IView.hpp"

template <typename T>
class MockRowField : public IRowField
{
public:
    T value;

protected:
    virtual void get_value_impl(void *target) override
    {
         *(T*)target = value;
    }

    virtual void set_value_impl(void const *source) override
    {
        value = *(T*)source;
    }
};

class MockRowView : public IRowView
{
public:
    std::map<std::string, IRowField*> fields;
public:
    virtual IRowField& operator[](std::string column_name) override;
    ~MockRowView();
};

class MockRowSet : public IRowSet
{
public:
    MockRowView row;
    std::size_t num_rows;

public:
    MockRowSet();
    virtual IRowView& current_row() override;
    virtual std::size_t total_rows() override;
    virtual void move_next() override;
};

class MockTable : public ITable
{
public:
    MockRowSet row_set;

public:
    virtual IRowSet *open_for_reading() override;
    virtual void close_for_reading(IRowSet *row_set) override;
    virtual IRowSet *open_for_writing(std::size_t new_size) override;
    virtual void close_for_writing(IRowSet *row_set) override;
};
