#include "gtest/gtest.h"
#include "SharedTables/Table.hpp"
#include "SharedTables/TableHeader.hpp"
#include "SharedTables/View.hpp"

#include "TestSharedTables/ExampleDTOs.hpp"
#include "TestSharedTables/MockTable.hpp"

#include <iostream>
#include <map>

using namespace std;

class TestViewFixture : public ::testing::Test
{

};

TEST_F(TestViewFixture, TestWriteDTO)
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
    ASSERT_EQ(11, idField->value);
    ASSERT_EQ(1.1, posXField->value);
    idField->value = 22;
    posXField->value = 2.2;
    view.read_table_into_vector();
    ASSERT_EQ(1, src.size());
    ASSERT_EQ(22, src[0].id);
    ASSERT_EQ(2.2, src[0].posX);
}

TEST_F(TestViewFixture, TestWriteLoclDTO)
{
    LocalView<GameObjTable> view;
    std::vector<GameObjTable>& src = view.get_vector();
    src.push_back({ 11, 1.1 });
    view.write_vector_into_table();
    view.read_table_into_vector();
    ASSERT_EQ(1, src.size());
    ASSERT_EQ(11, src[0].id);
    ASSERT_EQ(1.1, src[0].posX);
}
