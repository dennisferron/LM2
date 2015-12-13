#include "ITable.hpp"
#include "IView.hpp"
#include <vector>

template <typename T>
class TableView : public IView<T>
{
private:

    ITable* table;
    IDTOReader<T>* reader;
    std::vector<T> data;

public:

    TableView(ITable* table_) : table(table_)
    {
        reader = T::create_dto_reader();
    }

    ~TableView()
    {
        delete reader;
    }

    virtual std::vector<T>& get_vector() override
    {
        return data;
    }

    virtual void read_table_into_vector() override
    {
        IRowSet* rowset = table->open_for_reading();
        data.reserve(rowset->total_rows());
        for (std::size_t pos = 0; pos < data.size(); pos++, rowset->move_next())
        {
            reader->read_row_into_dto(rowset->current_row(), data[pos]);
        }
        table->close_for_reading(rowset);
    }

    virtual void write_vector_into_table() override
    {
        IRowSet* rowset = table->open_for_writing(data.size());
        for (std::size_t pos = 0; pos < data.size(); pos++, rowset->move_next())
        {
            reader->write_dto_into_row(rowset->current_row(), data[pos]);
        }
        table->close_for_writing(rowset);
    }
};

template <typename T>
class LocalView : public IView<T>
{
private:

    std::vector<T> data;

public:

    virtual std::vector<T>& get_vector() override
    {
        return data;
    }

    virtual void read_table_into_vector() override
    {
        // Do nothing
    }

    virtual void write_vector_into_table() override
    {
        // Do nothing
    }
};
