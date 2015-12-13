
#include "IView.hpp"
#include "ITable.hpp"
#include "Schema.hpp"

#include <memory>

#undef DTO_STRUCT
#undef DTO_FIELD

#if defined(X_DTO_DECL_RDR)
    #define DTO_STRUCT(name, members) \
        struct name##Reader : public IDTOReader<name> {\
            virtual void write_dto_into_row(IRowView& row, name const& dto) override; \
            virtual void read_row_into_dto(IRowView& row, name& dto) override; \
        }; \
        template<> IDTOReader<name>::~IDTOReader() { } \
        IDTOReader<name>* name::create_dto_reader() \
        { \
            return new name##Reader(); \
        }
    #define DTO_FIELD(name, datatype) /* do nothing */
#elif defined(X_DTO_DEF_RDR)
    #define DTO_STRUCT(name, members) void name##Reader::write_dto_into_row(IRowView& row, name const& dto) { members }
    #define DTO_FIELD(name, datatype) row[#name].set_value<datatype>(dto.name);
#elif defined(X_DTO_DEF_WTR)
    #define DTO_STRUCT(name, members) void name##Reader::read_row_into_dto(IRowView& row, name& dto) { members }
    #define DTO_FIELD(name, datatype) row[#name].get_value<datatype>(dto.name);
#elif defined(X_DTO_DEF_SCHEMA)
    #define DTO_STRUCT(name, members) Schema name::create_schema() { Schema schema(#name); members return schema; }
    #define DTO_FIELD(name, datatype) schema.add_column(#name, DataTypes::datatype, sizeof(datatype));
#else
    #define DTO_STRUCT(name, members) struct name { \
            members \
            static IDTOReader<name>* create_dto_reader(); \
            static Schema create_schema(); \
        };
    #define DTO_FIELD(name, datatype) datatype name;
#endif

