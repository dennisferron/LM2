
#include "ExampleDTOs.hpp"
#include "UnitTest++.h"
#include "MockTable.hpp"

SUITE(TestExampleDTOs)
{
    TEST(TestCreateReaderExists)
    {
        CHECK(&GameObjTable::create_dto_reader);
    }
}
