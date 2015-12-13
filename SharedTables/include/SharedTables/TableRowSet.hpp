#include "IView.hpp"
#include <map>
#include <memory>

class TableRowField : public IRowField
{
private:
    void* array;
    std::size_t const& pos;
    std::size_t cell_extent;

public:
    TableRowField(void* array_, std::size_t cell_extent_, std::size_t const& pos_);
    virtual void get_value_impl(void *target);
    virtual void set_value_impl(void const *source);
};

class TableRowView : public IRowView
{
private:
    std::map<std::string, IRowField*> fields;
    std::size_t cur_pos;

public:
    TableRowView();
    virtual IRowField& operator[](std::string column_name);
    ~TableRowView();
    void move_next();
    void add_field(void* array, std::size_t cell_extent, std::string column_name);
    std::size_t get_pos() const;
};

class TableRowSet : public IRowSet
{
private:
    TableRowView row_view;
    std::size_t num_rows;

public:
    TableRowSet(std::size_t num_rows_);
    virtual IRowView& current_row();
    virtual std::size_t total_rows();
    virtual void move_next();
    void add_field(void* array, ColumnInfo column_info);
};
