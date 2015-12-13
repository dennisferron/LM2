//
// Created by Dennis Ferron on 9/23/15.
//

#undef SCHEMA_DATATYPES
#undef SCHEMA_DT

#if defined(DEFINE_SCHEMA_TYPES)
    #define SCHEMA_DATATYPES(members) members
    #define SCHEMA_DT(name, type) typedef type name;
#elif defined(DEFINE_SCHEMA_ENUM)
    #define SCHEMA_DATATYPES(members) enum class DataTypes { dt_undefined members };
    #define SCHEMA_DT(name, type) ,name
#else
#define SCHEMA_DATATYPES(members) static_assert(false, "XMacro undefined");
#define SCHEMA_DT(name, type)
#endif

SCHEMA_DATATYPES(
        SCHEMA_DT(dt_byte, unsigned char)
        SCHEMA_DT(dt_int16, short)
        SCHEMA_DT(dt_int32, int)
        SCHEMA_DT(dt_int64, long long)
        SCHEMA_DT(dt_float, float)
        SCHEMA_DT(dt_double, double)
        SCHEMA_DT(dt_byte_array, unsigned char)
)

#undef DEFINE_SCHEMA_TYPES
#undef DEFINE_SCHEMA_ENUM
#undef SCHEMA_DATATYPES
#undef SCHEMA_DT
