#pragma once

#include <brutal/alloc/base.h>
#include <brutal/base/cast.h>
#include <brutal/base/keywords.h>
#include <brutal/base/macros.h>
#include <brutal/mem.h>

typedef struct
{
    char *data;
    int length;

    Alloc *alloc;
    int data_size;
    int capacity;
} VecImpl;

#define Vec(T)          \
    union               \
    {                   \
        struct          \
        {               \
            T *data;    \
            int length; \
        };              \
        VecImpl _impl;  \
    }

void vec_init_impl(VecImpl *impl, int data_size, Alloc *alloc);

void vec_deinit_impl(VecImpl *impl);

bool vec_expand_impl(VecImpl *impl);

bool vec_reserve_impl(VecImpl *impl, int n);

bool vec_reserve_po2_impl(VecImpl *impl, int n);

bool vec_compact_impl(VecImpl *impl);

bool vec_insert_impl(VecImpl *impl, int idx);

void vec_splice_impl(VecImpl *impl, int start, int count);

void vec_swapsplice_impl(VecImpl *impl, int start, int count);

void vec_swap_impl(VecImpl *impl, int idx1, int idx2);

#define vec_init(v, alloc_) vec_init_impl(impl$(v), sizeof(*(v)->data), alloc_)

#define vec_deinit(v) vec_deinit_impl(impl$(v))

#define vec_push(v, val) \
    (vec_expand_impl(impl$(v)) ? ((v)->data[(v)->length++] = (val), true) : false)

#define vec_pop(v) (v)->data[--(v)->length]

#define vec_splice(v, start, count) \
    (vec_splice_impl(impl$(v), start, count), (v)->length -= (count))

#define vec_swapsplice(v, start, count) \
    (vec_swapsplice_impl(impl$(v), start, count), (v)->length -= (count))

#define vec_insert(v, idx, val) (                     \
    {                                                 \
        bool result = vec_insert_impl(impl$(v), idx); \
        if (result)                                   \
        {                                             \
            (v)->data[idx] = (val);                   \
            (v)->length++;                            \
        }                                             \
        result;                                       \
    })

#define vec_sort(v, fn) qsort((v)->data, (v)->length, sizeof(*(v)->data), fn)

#define vec_swap(v, idx1, idx2) vec_swap_impl(impl$(v), idx1, idx2)

#define vec_truncate(v, len) \
    ((v)->length = (len) < (v)->length ? (len) : (v)->length)

#define vec_clear(v) ((v)->length = 0)

#define vec_first(v) (v)->data[0]

#define vec_last(v) (v)->data[(v)->length - 1]

#define vec_begin(v) ((v)->data)

#define vec_end(v) ((v)->data + (v)->length)

#define vec_reserve(v, n) vec_reserve_impl(impl$(v), n)

#define vec_compact(v) vec_compact_impl(impl$(v))

#define vec_find(v, val, idx)                         \
    STMT(                                             \
        for ((idx) = 0; (idx) < (v)->length; (idx)++) \
        {                                             \
            if ((v)->data[(idx)] == (val))            \
                break;                                \
        }                                             \
                                                      \
        if ((idx) == (v)->length) {                   \
            (idx) = -1;                               \
        })

#define vec_remove(v, val)           \
    do                               \
    {                                \
        int idx__;                   \
        vec_find(v, val, idx__);     \
                                     \
        if (idx__ != -1)             \
        {                            \
            vec_splice(v, idx__, 1); \
        }                            \
    } while (0)

#define vec_reverse(v)                                   \
    do                                                   \
    {                                                    \
        int i__ = (v)->length / 2;                       \
        while (i__--)                                    \
        {                                                \
            vec_swap((v), i__, (v)->length - (i__ + 1)); \
        }                                                \
    } while (0)

#define vec_foreach(VAR, SELF)                                          \
    if ((SELF)->length)                                                 \
        for (typeof((SELF)->data + 0) __once, __it = vec_begin(SELF); ( \
                 {                                                      \
                     __once = nullptr;                                  \
                     __it != vec_end(SELF);                             \
                 });                                                    \
             __it++)                                                    \
            for (typeof(*(SELF)->data) VAR = *__it; __once == nullptr; __once = (typeof((SELF)->data))1)

typedef Vec(void *) VecPtr;
typedef Vec(int) VecInt;
typedef Vec(char) VecChar;
