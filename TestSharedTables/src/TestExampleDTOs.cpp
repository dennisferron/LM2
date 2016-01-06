
#include "TestSharedTables/ExampleDTOs.hpp"
#include "gtest/gtest.h"
#include "TestSharedTables/MockTable.hpp"

SUITE(TestExampleDTOs)
{
    TEST(TestCreateReaderExists)
    {
        CHECK(&GameObjTable::create_dto_reader);
    }
}
