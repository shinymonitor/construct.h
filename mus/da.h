#ifndef _MUS_DA_H
#define _MUS_DA_H

#ifndef MUS_ASSERT
#include <assert.h>
#define MUS_ASSERT assert
#endif
#ifndef MUS_REALLOC
#include <stdlib.h>
#define MUS_REALLOC realloc
#endif
#ifndef MUS_FREE
#include <stdlib.h>
#define MUS_FREE free
#endif

//================================================================
//Dynamic Arrays
//================================================================

#ifndef MUS_DA_INIT_CAP
#define MUS_DA_INIT_CAP 256
#endif
#define MUS_da_define(item_t, da_t) typedef struct {item_t* items; size_t count, capacity;} da_t
#define MUS_da_reserve(da, expected_capacity) do { \
    if ((expected_capacity) > (da)->capacity) { \
        if ((da)->capacity == 0) (da)->capacity = MUS_DA_INIT_CAP; \
        while ((expected_capacity) > (da)->capacity) (da)->capacity *= 2; \
        (da)->items = MUS_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); MUS_ASSERT((da)->items != NULL && "DA RESERVE FAILED"); \
    } \
    else if ((expected_capacity) < (da)->capacity/2 && (da)->capacity > MUS_DA_INIT_CAP) { \
        do {(da)->capacity /= 2;} while ((da)->capacity > MUS_DA_INIT_CAP && (expected_capacity) <= (da)->capacity/2); \
        (da)->items = MUS_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); MUS_ASSERT((da)->items != NULL && "DA RESERVE FAILED"); \
    } \
} while (0)
#define MUS_da_append(da, item) do {MUS_da_reserve((da), (da)->count + 1); (da)->items[(da)->count++] = (item);} while (0)
#define MUS_da_append_many(da, new_items, new_items_count) do {MUS_da_reserve((da), (da)->count + (new_items_count)); memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); (da)->count += (new_items_count);} while (0)
#define MUS_da_resize(da, new_size) do {MUS_da_reserve((da), new_size); (da)->count = (new_size);} while (0)

#define MUS_da_remove_unordered(da, i) do {size_t j = (i); MUS_ASSERT(j < (da)->count); (da)->items[j] = (da)->items[--(da)->count];} while(0)
#define MUS_da_foreach(Type, it, da) for (Type* it = (da)->items; it < (da)->items + (da)->count; ++it)
#define MUS_da_reset(da) (da)->count=0
#define MUS_da_free(da) do {(da)->count=0; (da)->capacity=0; MUS_FREE((da)->items); (da)->items = NULL;} while(0)

//================================================================
//String Builder
//================================================================

MUS_da_define(char, MUS_StringBuilder);

#define MUS_sb_append_chr(sb, chr) MUS_da_append(sb, chr)
#define MUS_sb_append_buf(sb, buf, size) MUS_da_append_many(sb, buf, size)
#define MUS_sb_append_str(sb, str)  do {const char* s = (str); size_t n = strlen(s); MUS_da_append_many(sb, s, n);} while (0)
#define MUS_sb_append_null(sb) MUS_da_append(sb, '\0')

#define MUS_sb_str(sb) (sb)->items
#define MUS_sb_len(sb) (sb)->count
#define MUS_sb_get(sb, index) (MUS_ASSERT((sb)->count>index), (sb)->items[index])
#define MUS_sb_last(sb) (sb)->items[(MUS_ASSERT((sb)->count > 0), (sb)->count-1)]
#define MUS_sb_foreach(it, da) MUS_da_foreach(char, it, da)
#define MUS_sb_reset(sb) MUS_da_reset((sb))
#define MUS_sb_free(sb) MUS_da_free((sb))

#define MUS_SB_Fmt "%.*s"
#define MUS_SB_Arg(sb) (int)(sb)->count, (sb)->items

//================================================================
//Temp functions
//================================================================

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

MUS_da_define(char, MUS_Temp);

static MUS_Temp MUS_temp = {0};

static inline char* MUS_temp_sprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t n = vsnprintf(NULL, 0, format, args);
    va_end(args);
    MUS_da_resize(&MUS_temp, n+1);
    va_start(args, format);
    vsnprintf(MUS_temp.items, n + 1, format, args);
    va_end(args);
    return MUS_temp.items;
}
static inline char* MUS_temp_strdup(char* str){
    size_t n = strlen(str);
    MUS_da_resize(&MUS_temp, n+1);
    memcpy(MUS_temp.items, str, n);
    MUS_temp.items[n] = '\0';
    return MUS_temp.items;
}
static inline char* MUS_temp_strndup(char* str, size_t n){
    MUS_da_resize(&MUS_temp, n+1);
    memcpy(MUS_temp.items, str, n);
    MUS_temp.items[n] = '\0';
    return MUS_temp.items;
}
static inline void MUS_temp_reset(void) {MUS_da_reset(&MUS_temp);}
static inline void MUS_temp_free(void) {MUS_da_free(&MUS_temp);}

static inline char* MUS_utemp_sprintf(MUS_Temp* temp, const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t n = vsnprintf(NULL, 0, format, args);
    va_end(args);
    MUS_da_resize(temp, n+1);
    va_start(args, format);
    vsnprintf(temp->items, n + 1, format, args);
    va_end(args);
    return temp->items;
}
static inline char* MUS_utemp_strdup(MUS_Temp* temp, char* str){
    size_t n = strlen(str);
    MUS_da_resize(temp, n+1);
    memcpy(temp->items, str, n);
    temp->items[n] = '\0';
    return temp->items;
}
static inline char* MUS_utemp_strndup(MUS_Temp* temp, char* str, size_t n){
    MUS_da_resize(temp, n+1);
    memcpy(temp->items, str, n);
    temp->items[n] = '\0';
    return temp->items;
}
static inline void MUS_utemp_reset(MUS_Temp* temp) {MUS_da_reset(temp);}
static inline void MUS_utemp_free(MUS_Temp* temp) {MUS_da_free(temp);}

#endif //_MUS_DA_H

#ifndef MUS_DA_STRIP_PREFIX_GUARD_
#define MUS_DA_STRIP_PREFIX_GUARD_
    #ifndef MUS_DONT_STRIP_PREFIX
        #define da_define MUS_da_define
        #define da_reserve MUS_da_reserve
        #define da_append MUS_da_append
        #define da_append_many MUS_da_append_many
        #define da_resize MUS_da_resize
        #define da_remove_unordered MUS_da_remove_unordered
        #define da_foreach MUS_da_foreach
        #define da_reset MUS_da_reset
        #define da_free MUS_da_free
        #define StringBuilder MUS_StringBuilder
        #define sb_append_chr MUS_sb_append_chr
        #define sb_append_buf MUS_sb_append_buf
        #define sb_append_str MUS_sb_append_str
        #define sb_append_null MUS_sb_append_null
        #define sb_str MUS_sb_str
        #define sb_len MUS_sb_len
        #define sb_get MUS_sb_get
        #define sb_reset MUS_sb_reset
        #define sb_free MUS_sb_free
        #define sb_last MUS_sb_last
        #define sb_foreach MUS_sb_foreach
        #define SB_Fmt MUS_SB_Fmt
        #define SB_Arg MUS_SB_Arg
        #define Temp MUS_Temp
        #define temp_sprintf MUS_temp_sprintf
        #define temp_strdup MUS_temp_strdup
        #define temp_strndup MUS_temp_strndup
        #define temp_reset MUS_temp_reset
        #define temp_free MUS_temp_free
        #define utemp_sprintf MUS_utemp_sprintf
        #define utemp_strdup MUS_utemp_strdup
        #define utemp_strndup MUS_utemp_strndup
        #define utemp_reset MUS_utemp_reset
        #define utemp_free MUS_utemp_free
    #endif
#endif // MUS_DA_STRIP_PREFIX_GUARD_