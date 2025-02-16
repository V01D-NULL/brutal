#pragma once

#include <brutal/base.h>
#include <brutal/ds.h>
#include <brutal/log.h>
#include <cc/ast/val.h>

typedef enum
{
    CTYPE_INVALID,

    CTYPE_VOID,
    CTYPE_BOOL,
    CTYPE_PTR,
    CTYPE_ARRAY,
    CTYPE_SIGNED,
    CTYPE_UNSIGNED,
    CTYPE_FLOAT,
    CTYPE_STRUCT,
    CTYPE_UNION,
    CTYPE_ENUM,
    CTYPE_FUNC,
} CTypeType;

#define CTYPE_CONST (1 << 0)
#define CTYPE_RESTRICT (1 << 1)
#define CTYPE_VOLATILE (1 << 2)

typedef unsigned int CTypeAttr;

typedef struct ctype CType;
typedef struct ctype_member CTypeMember;
typedef struct ctype_constant CTypeConstant;

struct ctype
{
    CTypeType type;
    CTypeAttr attr;
    Str name;

    union
    {
        struct
        {
            int precision;
        } signed_, unsigned_, float_;

        struct
        {
            CType *subtype;
        } ptr_;

        struct
        {
            CType *subtype;
            int size;
        } array_;

        struct
        {
            Vec(CTypeMember) members;
        } struct_, union_;

        struct
        {
            Vec(CTypeConstant) constants;
        } enum_;

        struct
        {
            CType *ret;
            Vec(CTypeMember) params;
        } func_;
    };
};

struct ctype_member
{
    CType type;
    Str name;
};

struct ctype_constant
{
    Str name;
    CVal value;
};

typedef Slice(CType) CTypeSlice;
