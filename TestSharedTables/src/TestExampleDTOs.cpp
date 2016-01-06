
#include "TestSharedTables/ExampleDTOs.hpp"
#include "gtest/gtest.h"
#include "TestSharedTables/MockTable.hpp"

TEST(TestExampleDTOs, TestCreateReaderExists)
{
    CHECK(&GameObjTable::create_dto_reader);
}
