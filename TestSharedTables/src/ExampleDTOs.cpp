
#include "TestSharedTables/ExampleDTOs.hpp"

#define X_DTO_DECL_RDR
#include "TestSharedTables/ExampleDTOs.hpp"
#undef X_DTO_DECL_RDR

#define X_DTO_DEF_RDR
#include "TestSharedTables/ExampleDTOs.hpp"
#undef X_DTO_DEF_RDR

#define X_DTO_DEF_WTR
#include "TestSharedTables/ExampleDTOs.hpp"
#undef X_DTO_DEF_WTR

#define X_DTO_DEF_SCHEMA
#include "TestSharedTables/ExampleDTOs.hpp"
#undef X_DTO_DEF_SCHEMA
