
#include "TestSharedTables/ExampleDTOs.hpp"
#include "gtest/gtest.h"
#include "TestSharedTables/MockTable.hpp"

TEST(TestExampleDTOs, TestCreateReaderExists)
{
    ASSERT_TRUE(&GameObjTable::create_dto_reader);
}
