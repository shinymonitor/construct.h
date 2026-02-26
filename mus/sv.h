#ifndef _MUS_SV_H
#define _MUS_SV_H

#include <string.h>
#include <ctype.h>
#include <stdbool.h> 

typedef struct {const char* str; size_t len;} MUS_StringView;

static inline MUS_StringView MUS_sv_chop_by_delim(MUS_StringView* sv, char delim) {
    bool nd = false;
    MUS_StringView result = {0};
    for (size_t i = 0; i < sv->len; ++i) {
        if (nd == false && sv->str[i] != delim) {result.str = sv->str + i; nd = true;}
        else if (nd == true && sv->str[i] == delim && sv->str[i - 1] != '\\') {result.len = i - (result.str - sv->str); sv->str += i + 1; sv->len -= i + 1; return result;}
    }
    if (nd) {
        result.len = sv->len - (result.str - sv->str);
        sv->str += sv->len; sv->len = 0;
        return result;
    }
    return (MUS_StringView){NULL, 0};
}
static inline void MUS_sv_trim_left(MUS_StringView* sv, size_t n){if (n > sv->len) n = sv->len; sv->str += n; sv->len -= n;}
static inline void MUS_sv_trim_right(MUS_StringView* sv, size_t n){if (n > sv->len) n = sv->len; sv->len -= n;}
static inline void MUS_sv_strip_left(MUS_StringView* sv) {while (sv->len > 0 && isspace(sv->str[0])) {sv->str++; sv->len--;}}
static inline void MUS_sv_strip_right(MUS_StringView* sv) {while (sv->len > 0 && isspace(sv->str[sv->len - 1])) --sv->len;}
static inline void MUS_sv_strip(MUS_StringView* sv) {MUS_sv_strip_left(sv); MUS_sv_strip_right(sv);}

static inline bool MUS_sv_equal(MUS_StringView* a, MUS_StringView* b) {
    size_t len_a = a->len;
    size_t len_b = b->len;
    if (len_a != len_b) return false;
    return memcmp(a->str, b->str, len_a) == 0;
}
static inline bool MUS_sv_ends_with(MUS_StringView* sv, char* cstr) {
    size_t cstr_len = strlen(cstr);
    if (cstr_len > sv->len) return false;
    return memcmp(sv->str + sv->len - cstr_len, cstr, cstr_len) == 0;
}
static inline bool MUS_sv_starts_with(MUS_StringView* sv, char* cstr) {
    size_t cstr_len = strlen(cstr);
    if (cstr_len > sv->len) return false;
    return memcmp(sv->str, cstr, cstr_len) == 0;
}
static inline void MUS_sv_to_cstr(MUS_StringView* sv, char* cstr){
    size_t sv_len = sv->len;
    memcpy(cstr, sv->str, sv_len);
    cstr[sv_len]='\0';
}
static inline MUS_StringView MUS_sv_from_cstr(const char* cstr) {return (MUS_StringView){cstr, strlen(cstr)};}
static inline MUS_StringView MUS_sv_from_parts(const char* cstr, size_t len) {return (MUS_StringView){cstr, len};}

#define MUS_sv_len(sv) ((sv).len)
#define MUS_SV_Fmt "%.*s"
#define MUS_SV_Arg(sv) (int)MUS_sv_len(sv), (sv).str

#endif //_MUS_SV_H

#ifndef MUS_SV_STRIP_PREFIX_GUARD_
#define MUS_SV_STRIP_PREFIX_GUARD_
    #ifndef MUS_DONT_STRIP_PREFIX
        #define StringView MUS_StringView
        #define sv_chop_by_delim MUS_sv_chop_by_delim
        #define sv_trim_left MUS_sv_trim_left
        #define sv_trim_right MUS_sv_trim_right
        #define sv_strip_left MUS_sv_strip_left
        #define sv_strip_right MUS_sv_strip_right
        #define sv_strip MUS_sv_strip
        #define sv_equal MUS_sv_equal
        #define sv_ends_with MUS_sv_ends_with
        #define sv_starts_with MUS_sv_starts_with
        #define sv_to_cstr MUS_sv_to_cstr
        #define sv_from_cstr MUS_sv_from_cstr
        #define sv_from_parts MUS_sv_from_parts
        #define sv_len MUS_sv_len
        #define SV_Fmt MUS_SV_Fmt
        #define SV_Arg MUS_SV_Arg
    #endif
#endif // MUS_SV_STRIP_PREFIX_GUARD_