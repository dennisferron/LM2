#include "UnitTest++.h"
#include "Table.hpp"
#include "TableHeader.hpp"
#include "View.hpp"

#include "ExampleDTOs.hpp"
#include "MockTable.hpp"

#include <iostream>
#include <map>

using namespace std;


SUITE(TestView)
{
    TEST(TestWriteDTO)
    {
        MockTable table;
        auto& fields = table.row_set.row.fields;
        MockRowField<int>* idField =  new MockRowField<int>;
        fields["id"] = idField;
        MockRowField<double>* posXField = new MockRowField<double>;
        fields["posX"] = posXField;
        TableView<GameObjTable> view(&table);
        std::vector<GameObjTable>& src = view.get_vector();
        src.push_back({ 11, 1.1 });
        view.write_vector_into_table();
        CHECK_EQUAL(11, idField->value);
        CHECK_EQUAL(1.1, posXField->value);
        idField->value = 22;
        posXField->value = 2.2;
        view.read_table_into_vector();
        CHECK_EQUAL(1, src.size());
        CHECK_EQUAL(22, src[0].id);
        CHECK_EQUAL(2.2, src[0].posX);
    }

    TEST(TestWriteLoclDTO)
    {
        LocalView<GameObjTable> view;
        std::vector<GameObjTable>& src = view.get_vector();
        src.push_back({ 11, 1.1 });
        view.write_vector_into_table();
        view.read_table_into_vector();
        CHECK_EQUAL(1, src.size());
        CHECK_EQUAL(11, src[0].id);
        CHECK_EQUAL(1.1, src[0].posX);
    }
}
