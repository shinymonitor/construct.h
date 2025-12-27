#ifndef _CONSTRUCT_H
#define _CONSTRUCT_H

//================================================================
//SECTION: Config
//================================================================

#define CONSTRUCT_DECLSPEC static inline

//================================================================
//SECTION: Includes
//================================================================

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

//================================================================
//SECTION: Platform
//================================================================

#ifdef _WIN32
	#include <direct.h>
    #include <windows.h>
    #include <io.h>
    #include <process.h>
	#define CONSTRUCT_NL "\r\n"
	#define CONSTRUCT_PS '\\'
	#define CONSTRUCT_MKDIR_CMD "mkdir %s"
    #define CONSTRUCT_CP_CMD "copy /Y %s %s"
    #define CONSTRUCT_CP_R_CMD "xcopy /Y /E %s %s"
    #define CONSTRUCT_MV_CMD "move /Y %s %s"
	#define CONSTRUCT_RM_CMD "del /Q %s"
	#define CONSTRUCT_RMDIR_CMD "rmdir /S /Q %s"
	#define CONSTRUCT_FETCH_CMD "curl -L -o lib\\%s %s"
    typedef HANDLE CONSTRUCT_Proc;
    #define CONSTRUCT_INVALID_PROC INVALID_HANDLE_VALUE
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <errno.h>
	#define CONSTRUCT_NL "\n"
	#define CONSTRUCT_PS '/'
	#define CONSTRUCT_MKDIR_CMD "mkdir -p %s"
    #define CONSTRUCT_CP_CMD "cp %s %s"
    #define CONSTRUCT_CP_R_CMD "cp -r %s %s"
    #define CONSTRUCT_MV_CMD "mv %s %s"
	#define CONSTRUCT_RM_CMD "rm %s"
	#define CONSTRUCT_RMDIR_CMD "rm -rf %s"
	#define CONSTRUCT_FETCH_CMD "wget -q --show-progress -O lib/%s %s"
    typedef int CONSTRUCT_Proc;
    #define CONSTRUCT_INVALID_PROC (-1)
#endif

//================================================================
//SECTION: assert, realloc, free
//================================================================

#ifndef CONSTRUCT_ASSERT
#include <assert.h>
#define CONSTRUCT_ASSERT assert
#endif
#ifndef CONSTRUCT_REALLOC
#include <stdlib.h>
#define CONSTRUCT_REALLOC realloc
#endif
#ifndef CONSTRUCT_FREE
#include <stdlib.h>
#define CONSTRUCT_FREE free
#endif

//================================================================
//SECTION: Dynamic Arrays
//================================================================

#ifndef CONSTRUCT_DA_INIT_CAP
#define CONSTRUCT_DA_INIT_CAP 256
#endif
#define CONSTRUCT_da_define(item_t, da_t) typedef struct {item_t* items; size_t count, capacity;} da_t
#define CONSTRUCT_da_reserve(da, expected_capacity) do { \
    if ((expected_capacity) > (da)->capacity) { \
        if ((da)->capacity == 0) (da)->capacity = CONSTRUCT_DA_INIT_CAP; \
        while ((expected_capacity) > (da)->capacity) (da)->capacity *= 2; \
        (da)->items = CONSTRUCT_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); CONSTRUCT_ASSERT((da)->items != NULL && "DA RESERVE FAILED"); \
    } \
    else if ((expected_capacity) < (da)->capacity/2 && (da)->capacity > CONSTRUCT_DA_INIT_CAP) { \
        do {(da)->capacity /= 2;} while ((da)->capacity/2 > CONSTRUCT_DA_INIT_CAP && (expected_capacity) <= (da)->capacity/2); \
        (da)->items = CONSTRUCT_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); CONSTRUCT_ASSERT((da)->items != NULL && "DA RESERVE FAILED"); \
    } \
} while (0)
#define CONSTRUCT_da_append(da, item) do {CONSTRUCT_da_reserve((da), (da)->count + 1); (da)->items[(da)->count++] = (item);} while (0)
#define CONSTRUCT_da_append_many(da, new_items, new_items_count) do {CONSTRUCT_da_reserve((da), (da)->count + (new_items_count)); memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); (da)->count += (new_items_count);} while (0)
#define CONSTRUCT_da_resize(da, new_size) do {CONSTRUCT_da_reserve((da), new_size); (da)->count = (new_size);} while (0)

#define CONSTRUCT_da_items(da) (da)->items
#define CONSTRUCT_da_get(da, index) (CONSTRUCT_ASSERT((da)->count>index), (da)->items[index])
#define CONSTRUCT_da_last(da) (da)->items[(CONSTRUCT_ASSERT((da)->count > 0), (da)->count-1)]
#define CONSTRUCT_da_remove_unordered(da, i) do {size_t j = (i); CONSTRUCT_ASSERT(j < (da)->count); (da)->items[j] = (da)->items[--(da)->count];} while(0)
#define CONSTRUCT_da_foreach(Type, it, da) for (Type* it = (da)->items; it < (da)->items + (da)->count; ++it)
#define CONSTRUCT_da_reset(da) (da)->count=0
#define CONSTRUCT_da_free(da) do {(da)->count=0; (da)->capacity=0; CONSTRUCT_FREE((da)->items);} while(0)

//================================================================
//SECTION: Misc
//================================================================

#define CONSTRUCT_STRINGIFY(x) #x
#define CONSTRUCT_TOSTRING(x) CONSTRUCT_STRINGIFY(x)

#define CONSTRUCT_UNUSED(value) (void)(value)
#define CONSTRUCT_TODO(message) do {printf("%s:%d: TODO: %s\n", __FILE__, __LINE__, message); abort();} while(0)

#define CONSTRUCT_PANIC(message) do {printf("%s:%d: PANIC: %s\n", __FILE__, __LINE__, message); abort();} while(0)
#define CONSTRUCT_UNREACHABLE() do {printf("%s:%d: UNREACHABLE\n", __FILE__, __LINE__); abort();} while(0)

#define CONSTRUCT_pop_first(xs, xs_sz) (CONSTRUCT_ASSERT((xs_sz) > 0), (xs_sz)--, *(xs)++)
#define CONSTRUCT_return_defer(value) do { result = (value); goto defer; } while(0)

clock_t CONSTRUCT_c1, CONSTRUCT_c2;
#define CONSTRUCT_TIMER_START CONSTRUCT_c1=clock()
#define CONSTRUCT_TIMER_END CONSTRUCT_c2=clock()
#define CONSTRUCT_TIMER_SEC ((float)CONSTRUCT_c2 - (float)CONSTRUCT_c1)/CLOCKS_PER_SEC

#define CONSTRUCT_AL(Type, ...) (Type[]){__VA_ARGS__}, (sizeof((Type[]){__VA_ARGS__})/sizeof(Type))
#define CONSTRUCT_SA(...) CONSTRUCT_AL(char*, __VA_ARGS__)

//================================================================
//SECTION: Short cicuiting (Misc)
//================================================================

#define CONSTRUCT_expect_tt(exp) if(!(exp)) return 1
#define CONSTRUCT_expect_ft(exp) if((exp)) return 1
#define CONSTRUCT_expect_pt(exp) if((exp)<0) return 1
#define CONSTRUCT_expect_tf(exp) if(!(exp)) return 0
#define CONSTRUCT_expect_ff(exp) if((exp)) return 0
#define CONSTRUCT_expect_pf(exp) if((exp)<0) return 0

#define CONSTRUCT_d_expect_tt(exp) if(!(exp)) CONSTRUCT_return_defer(1)
#define CONSTRUCT_d_expect_ft(exp) if((exp)) CONSTRUCT_return_defer(1)
#define CONSTRUCT_d_expect_pt(exp) if((exp)<0) CONSTRUCT_return_defer(1)
#define CONSTRUCT_d_expect_tf(exp) if(!(exp)) CONSTRUCT_return_defer(0)
#define CONSTRUCT_d_expect_ff(exp) if((exp)) CONSTRUCT_return_defer(0)
#define CONSTRUCT_d_expect_pf(exp) if((exp)<0) CONSTRUCT_return_defer(0)

#define CONSTRUCT_u_expect_t(exp) if(!(exp)) CONSTRUCT_UNREACHABLE()
#define CONSTRUCT_u_expect_f(exp) if((exp)) CONSTRUCT_UNREACHABLE()
#define CONSTRUCT_u_expect_p(exp) if((exp)<0) CONSTRUCT_UNREACHABLE()

//================================================================
//SECTION: Logging
//================================================================

typedef enum {CONSTRUCT_INFO, CONSTRUCT_WARNING, CONSTRUCT_ERROR} CONSTRUCT_LogType;

CONSTRUCT_DECLSPEC void CONSTRUCT_print_log(CONSTRUCT_LogType level, const char* fmt, ...) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    printf("[%d/%02d/%02d@%02d:%02d:%02d] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    switch (level) {
        case CONSTRUCT_INFO: printf("[INFO] "); break;
        case CONSTRUCT_WARNING: printf("\x1B[33m[WARNING] "); break;
        case CONSTRUCT_ERROR: printf("\x1B[31;1;4m[ERROR] "); break;
    }
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\x1B[0m\n");
}

CONSTRUCT_DECLSPEC void CONSTRUCT_print_log_to_file(const char* file_path, CONSTRUCT_LogType level, const char* fmt, ...) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    FILE* file=fopen(file_path, "a");
    if (file == NULL) return;
    fprintf(file, "[%d/%02d/%02d@%02d:%02d:%02d] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    switch (level) {
        case CONSTRUCT_INFO: fprintf(file, "[INFO] "); break;
        case CONSTRUCT_WARNING: fprintf(file, "[WARNING] "); break;
        case CONSTRUCT_ERROR: fprintf(file, "[ERROR] "); break;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
    fprintf(file, "\n");
    fclose(file);
}

//================================================================
//SECTION: String Builder (Dynamic Array)
//================================================================

CONSTRUCT_da_define(char, CONSTRUCT_StringBuilder);

#define CONSTRUCT_sb_append_chr(sb, chr) CONSTRUCT_da_append(sb, chr)
#define CONSTRUCT_sb_append_buf(sb, buf, size) CONSTRUCT_da_append_many(sb, buf, size)
#define CONSTRUCT_sb_append_str(sb, str)  do {const char* s = (str); size_t n = strlen(s); CONSTRUCT_da_append_many(sb, s, n);} while (0)
#define CONSTRUCT_sb_append_null(sb) CONSTRUCT_da_append(sb, '\0')

#define CONSTRUCT_sb_items(sb) CONSTRUCT_da_items((sb))
#define CONSTRUCT_sb_get(sb, index) CONSTRUCT_da_get((sb), index)
#define CONSTRUCT_sb_reset(sb) CONSTRUCT_da_reset((sb))
#define CONSTRUCT_sb_free(sb) CONSTRUCT_da_free((sb))

#define CONSTRUCT_sb_last(sb) CONSTRUCT_da_last((sb))
#define CONSTRUCT_sb_remove_unordered(sb, i) CONSTRUCT_da_remove_unordered((sb), i)
#define CONSTRUCT_sb_foreach(it, da) CONSTRUCT_da_foreach(char, it, da)

#define CONSTRUCT_SB_Fmt "%.*s"
#define CONSTRUCT_SB_Arg(sb) (int)(sb)->count, (sb)->items

//================================================================
//SECTION: String View
//================================================================

typedef struct {
    const char* string;
    size_t from;
    size_t until;
} CONSTRUCT_StringView;

CONSTRUCT_DECLSPEC CONSTRUCT_StringView CONSTRUCT_sv_until_delim(CONSTRUCT_StringView* sv, char delim) {
    size_t i = 0;
    while (sv->from + i < sv->until && sv->string[sv->from + i] != delim) ++i;
    CONSTRUCT_StringView result = {sv->string, sv->from, sv->from + i};
    sv->from = sv->from + i + 1;
    return result;
}
CONSTRUCT_DECLSPEC void CONSTRUCT_sv_trim_left(CONSTRUCT_StringView* sv, size_t n){if (n > sv->until-sv->from) n = sv->until-sv->from; sv->from+=n;}
CONSTRUCT_DECLSPEC void CONSTRUCT_sv_trim_right(CONSTRUCT_StringView* sv, size_t n){if (n > sv->until-sv->from) n = sv->until-sv->from; sv->until-=n;}
CONSTRUCT_DECLSPEC void CONSTRUCT_sv_strip_left(CONSTRUCT_StringView* sv) {while (sv->from < sv->until && isspace(sv->string[sv->from])) ++sv->from;}
CONSTRUCT_DECLSPEC void CONSTRUCT_sv_strip_right(CONSTRUCT_StringView* sv) {while (sv->from < sv->until && isspace(sv->string[sv->until - 1])) --sv->until;}
CONSTRUCT_DECLSPEC void CONSTRUCT_sv_strip(CONSTRUCT_StringView* sv) {CONSTRUCT_sv_strip_left(sv); CONSTRUCT_sv_strip_right(sv);}
CONSTRUCT_DECLSPEC bool CONSTRUCT_sv_eq(CONSTRUCT_StringView* a, CONSTRUCT_StringView* b) {
    size_t len_a = a->until - a->from;
    size_t len_b = b->until - b->from;
    if (len_a != len_b) return false;
    return memcmp(a->string + a->from, b->string + b->from, len_a) == 0;
}
CONSTRUCT_DECLSPEC bool CONSTRUCT_sv_ends_with(CONSTRUCT_StringView* sv, char* cstr) {
    size_t sv_len = sv->until - sv->from;
    size_t cstr_len = strlen(cstr);
    if (cstr_len > sv_len) return false;
    return memcmp(sv->string + sv->until - cstr_len, cstr, cstr_len) == 0;
}
CONSTRUCT_DECLSPEC bool CONSTRUCT_sv_starts_with(CONSTRUCT_StringView* sv, char* cstr) {
    size_t sv_len = sv->until - sv->from;
    size_t cstr_len = strlen(cstr);
    if (cstr_len > sv_len) return false;
    return memcmp(sv->string + sv->from, cstr, cstr_len) == 0;
}
CONSTRUCT_DECLSPEC void CONSTRUCT_sv_to_cstr(CONSTRUCT_StringView* sv, char* cstr){
    size_t sv_len = sv->until - sv->from;
    memcpy(cstr, sv->string+sv->from, sv_len);
    cstr[sv_len]='\0';
}
CONSTRUCT_DECLSPEC CONSTRUCT_StringView CONSTRUCT_sv_from_cstr(const char* cstr) {return (CONSTRUCT_StringView){cstr, 0, strlen(cstr)};}
CONSTRUCT_DECLSPEC CONSTRUCT_StringView CONSTRUCT_sv_from_parts(const char* cstr, size_t from, size_t until) {return (CONSTRUCT_StringView){cstr, from, until};}

#define CONSTRUCT_sv_len(sv) ((sv)->until - (sv)->from)
#define CONSTRUCT_SV_Fmt "%.*s"
#define CONSTRUCT_SV_Arg(sv) (int)CONSTRUCT_sv_len(sv), (sv)->string + (sv)->from

//================================================================
//SECTION: Temp functions (Dynamic Arrays)
//================================================================

CONSTRUCT_da_define(char, CONSTRUCT_Temp);

static CONSTRUCT_Temp CONSTRUCT_temp = {0};

CONSTRUCT_DECLSPEC char* CONSTRUCT_temp_sprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t n = vsnprintf(NULL, 0, format, args);
    va_end(args);
    CONSTRUCT_da_resize(&CONSTRUCT_temp, n+1);
    va_start(args, format);
    vsnprintf(CONSTRUCT_temp.items, n + 1, format, args);
    va_end(args);
    return CONSTRUCT_temp.items;
}
CONSTRUCT_DECLSPEC char* CONSTRUCT_temp_strdup(char* str){
    size_t n = strlen(str);
    CONSTRUCT_da_resize(&CONSTRUCT_temp, n+1);
    memcpy(CONSTRUCT_temp.items, str, n);
    CONSTRUCT_temp.items[n] = '\0';
    return CONSTRUCT_temp.items;
}
CONSTRUCT_DECLSPEC char* CONSTRUCT_temp_strndup(char* str, size_t n){
    CONSTRUCT_da_resize(&CONSTRUCT_temp, n+1);
    memcpy(CONSTRUCT_temp.items, str, n);
    CONSTRUCT_temp.items[n] = '\0';
    return CONSTRUCT_temp.items;
}
CONSTRUCT_DECLSPEC void CONSTRUCT_temp_reset(void) {CONSTRUCT_da_reset(&CONSTRUCT_temp);}
CONSTRUCT_DECLSPEC void CONSTRUCT_temp_free(void) {CONSTRUCT_da_free(&CONSTRUCT_temp);}

//================================================================
//SECTION: File Handling (Platform, Temp, Logging, Dynamic Arrays)
//================================================================

CONSTRUCT_DECLSPEC bool CONSTRUCT_file_exists(char* file_path){FILE* file; if ((file = fopen(file_path, "r"))) {fclose(file); return true;} return false;}
CONSTRUCT_DECLSPEC time_t CONSTRUCT_get_mtime(char* file_path) {struct stat st; if (stat(file_path, &st) == -1) return 0; return st.st_mtime;}
CONSTRUCT_DECLSPEC bool CONSTRUCT_is_newer(char* new_path, char* old_path){
	time_t new_mtime = CONSTRUCT_get_mtime(new_path);
	time_t old_mtime = CONSTRUCT_get_mtime(old_path);
	if (new_mtime == 0 || old_mtime == 0) return true;
	return new_mtime > old_mtime;
}
CONSTRUCT_DECLSPEC const char* CONSTRUCT_path_basename(const char* path){
    #ifdef _WIN32
        const char* p1 = strrchr(path, '/');
        const char* p2 = strrchr(path, CONSTRUCT_PS);
        const char* p = (p1 > p2)? p1 : p2;
        return p ? p + 1 : path;
    #else
        const char* p = strrchr(path, CONSTRUCT_PS);
        return p ? p + 1 : path;
    #endif
}

CONSTRUCT_DECLSPEC bool CONSTRUCT_mkdir_if_not_exists(const char* path){return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_MKDIR_CMD, path)));}
CONSTRUCT_DECLSPEC bool CONSTRUCT_copy_file(const char* src_path, const char* dst_path){return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_CP_CMD, src_path, dst_path)));}
CONSTRUCT_DECLSPEC bool CONSTRUCT_copy_directory_recursively(const char* src_path, const char* dst_path) {return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_CP_R_CMD, src_path, dst_path)));}
CONSTRUCT_DECLSPEC bool CONSTRUCT_move_file(const char* src_path, const char* dst_path){return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_MV_CMD, src_path, dst_path)));}
CONSTRUCT_DECLSPEC bool CONSTRUCT_delete_file(const char* path) {return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_RM_CMD, path)));}
CONSTRUCT_DECLSPEC bool CONSTRUCT_delete_directory(const char* path) {return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_RMDIR_CMD, path)));}
CONSTRUCT_DECLSPEC bool CONSTRUCT_fetch_file(const char* path, const char* url){return (!system(CONSTRUCT_temp_sprintf(CONSTRUCT_FETCH_CMD, path, url)));}

CONSTRUCT_da_define(uint8_t, CONSTRUCT_DABytes);
CONSTRUCT_DECLSPEC bool CONSTRUCT_read_entire_file(const char* path, CONSTRUCT_DABytes* da_bytes) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) return false;
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_size < 0) {fclose(file); return false;}
    CONSTRUCT_da_reserve(da_bytes, da_bytes->count + file_size);
    size_t bytes_read = fread(da_bytes->items + da_bytes->count, 1, file_size, file);
    da_bytes->count += bytes_read;
    fclose(file);
    return bytes_read == (size_t)file_size;
}
CONSTRUCT_DECLSPEC bool CONSTRUCT_write_entire_file(const char* path, const void* data, size_t size) {
    FILE* file = fopen(path, "wb");
    if (file == NULL) return false;
    size_t bytes_written = fwrite(data, 1, size, file);
    fclose(file);
    return bytes_written == size;
}

#define CONSTRUCT_mkdir_if_not_exists_l(path) do {if(CONSTRUCT_mkdir_if_not_exists(path)) CONSTRUCT_print_log(CONSTRUCT_INFO, "CREATED DIRECTORY %s", #path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT CREATE DIRECTORY %s", #path);} while(0)
#define CONSTRUCT_copy_file_l(src_path, dst_path) do {if(CONSTRUCT_copy_file(src_path, dst_path)) CONSTRUCT_print_log(CONSTRUCT_INFO, "COPIED FILE %s TO %s", #src_path, #dst_path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT COPY FILE %s TO %s", #src_path, #dst_path);} while(0)
#define CONSTRUCT_copy_directory_recursively_l(src_path, dst_path) do {if(CONSTRUCT_copy_directory_recursively(src_path, dst_path)) CONSTRUCT_print_log(CONSTRUCT_INFO, "RECURSIVELY COPIED DIRECTORY %s TO %s", #src_path, #dst_path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT RECURSIVELY COPY DIRECTORY %s TO %s", #src_path, #dst_path);} while(0)
#define CONSTRUCT_move_file_l(src_path, dst_path) do {if(CONSTRUCT_move_file(src_path, dst_path)) CONSTRUCT_print_log(CONSTRUCT_INFO, "MOVED FILE %s TO %s", #src_path, #dst_path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT MOVE FILE %s TO %s", #src_path, #dst_path);} while(0)
#define CONSTRUCT_delete_file_l(path) do {if(CONSTRUCT_delete_file(path)) CONSTRUCT_print_log(CONSTRUCT_INFO, "DELETED FILE %s", #path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT DELETE FILE %s", #path);} while(0)
#define CONSTRUCT_delete_directory_l(path) do {if(CONSTRUCT_delete_directory(path)) CONSTRUCT_print_log(CONSTRUCT_INFO, "DELETED DIRECTORY %s", #path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT DELETE DIRECTORY %s", #path);} while(0)
#define CONSTRUCT_fetch_file_l(path, url) do {if(CONSTRUCT_fetch_file(path, url)) CONSTRUCT_print_log(CONSTRUCT_INFO, "FETCHED FILE %s TO %s", #path, #url); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT FETCH FILE %s TO %s", #path, #url);} while(0)
#define CONSTRUCT_read_entire_file_l(path, dab) do {if(CONSTRUCT_read_entire_file(path, dab)) CONSTRUCT_print_log(CONSTRUCT_INFO, "READ FILE %s", #path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT READ FILE %s", #path);} while(0)
#define CONSTRUCT_write_entire_file_l(path, data, size) do {if(CONSTRUCT_write_entire_file(path, data, size)) CONSTRUCT_print_log(CONSTRUCT_INFO, "WROTE FILE %s", #path); else CONSTRUCT_print_log(CONSTRUCT_ERROR, "COULDNT WRITE FILE %s", #path);} while(0)

//================================================================
//SECTION: Processes
//================================================================

CONSTRUCT_da_define(CONSTRUCT_Proc, CONSTRUCT_Procs);

CONSTRUCT_DECLSPEC int CONSTRUCT_proc_wait(CONSTRUCT_Proc proc) {
    #ifdef _WIN32
    if (proc == INVALID_HANDLE_VALUE) return -1;
    DWORD result = WaitForSingleObject(proc, 1);
    if (result == WAIT_FAILED) {CONSTRUCT_print_log(CONSTRUCT_ERROR, "WaitForSingleObject failed: %lu", GetLastError()); return -1;}
    if (result == WAIT_TIMEOUT) return 0;
    DWORD exit_code;
    if (!GetExitCodeProcess(proc, &exit_code)) {CONSTRUCT_print_log(CONSTRUCT_ERROR, "GetExitCodeProcess failed: %lu", GetLastError()); return -1;}
    CloseHandle(proc);
    if (exit_code != 0) {CONSTRUCT_print_log(CONSTRUCT_ERROR, "Process exited with code %lu", exit_code); return -1;}
    return 1;
    #else
    if (proc == CONSTRUCT_INVALID_PROC) return -1;
    int status;
    int ret = waitpid(proc, &status, WNOHANG);
    if (ret == -1) {CONSTRUCT_print_log(CONSTRUCT_ERROR, "waitpid failed: %s", strerror(errno)); return -1;}
    if (ret == 0) return 0;
    if (!WIFEXITED(status)) {CONSTRUCT_print_log(CONSTRUCT_ERROR, "Process did not exit normally"); return -1;}
    int exit_code = WEXITSTATUS(status);
    if (exit_code != 0) {CONSTRUCT_print_log(CONSTRUCT_ERROR, "Process exited with code %d", exit_code); return -1;}
    return 1;
    #endif
}
CONSTRUCT_DECLSPEC bool CONSTRUCT_procs_wait(CONSTRUCT_Procs* procs){
    bool all_done = false;
    while (!all_done) {
        all_done = true;
        for (size_t i = 0; i < procs->count; ++i) {
            int result = CONSTRUCT_proc_wait(procs->items[i]);
            if (result == -1) return false;
            if (result == 0) all_done = false;
        }
        if (!all_done) {
            #ifdef _WIN32
            Sleep(10);
            #else
            usleep(10000);
            #endif
        }
    }
    return true;
}
CONSTRUCT_DECLSPEC bool CONSTRUCT_procs_flush(CONSTRUCT_Procs* procs){
    bool success = CONSTRUCT_procs_wait(procs);
    procs->count = 0;
    return success;
}

//================================================================
//SECTION: Commands (Platform, Processes, StringBuilder, Dynamic Arrays)
//================================================================

CONSTRUCT_da_define(char*, CONSTRUCT_Command);
typedef struct {
    CONSTRUCT_Procs* async;
    size_t max_procs;
} CONSTRUCT_CommandOptions;

#define CONSTRUCT_command_append(command, ...) CONSTRUCT_da_append_many(command, ((const char*[]){__VA_ARGS__}), (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))
#define CONSTRUCT_command_append_separator(command) CONSTRUCT_command_append(command, ";")
#define CONSTRUCT_command_extend(command, other_command) CONSTRUCT_da_append_many(command, (other_command)->items, (other_command)->count)
#define CONSTRUCT_command_free(command) CONSTRUCT_FREE(command.items)

CONSTRUCT_DECLSPEC void CONSTRUCT_command_render(CONSTRUCT_Command* command, CONSTRUCT_StringBuilder* render){
    CONSTRUCT_sb_reset(render);
    for (size_t i = 0; i < command->count; ++i) {
        const char* arg = command->items[i];
        if (arg == NULL) break;
        if (i > 0) CONSTRUCT_sb_append_chr(render, ' ');
        if (!strchr(arg, ' ')) CONSTRUCT_sb_append_str(render, arg);
        else {
            CONSTRUCT_sb_append_chr(render, '\'');
            CONSTRUCT_sb_append_str(render, arg);
            CONSTRUCT_sb_append_chr(render, '\'');
        }
    }
    CONSTRUCT_sb_append_null(render);
}

#define CONSTRUCT_command_run(command, ...) CONSTRUCT_command_run_with_options((command), (CONSTRUCT_CommandOptions){__VA_ARGS__})
CONSTRUCT_DECLSPEC bool CONSTRUCT_command_run_with_options(CONSTRUCT_Command* command, CONSTRUCT_CommandOptions options) {
    bool result = true;
    if (command->count == 0) return true;
    size_t max_procs = options.max_procs > 0 ? options.max_procs : 1;
    if (options.async && max_procs > 0) {
        while (options.async->count >= max_procs) {
            bool found = false;
            for (size_t i = 0; i < options.async->count; ++i) {
                int ret = CONSTRUCT_proc_wait(options.async->items[i]);
                if (ret < 0) CONSTRUCT_return_defer(false);
                if (ret > 0) {
                    CONSTRUCT_da_remove_unordered(options.async, i);
                    found = true;
                    break;
                }
            }
            if (!found) {
                #ifdef _WIN32
                Sleep(10);
                #else
                usleep(10000);
                #endif
            }
        }
    }
    CONSTRUCT_StringBuilder command_sb = {0};
    CONSTRUCT_command_render(command, &command_sb);
    #ifdef _WIN32
    STARTUPINFOA si = {0};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    PROCESS_INFORMATION pi = {0};
    if (!CreateProcessA(NULL, command_sb.items, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        CONSTRUCT_print_log(CONSTRUCT_ERROR, "CreateProcess failed: %lu", GetLastError());
        CONSTRUCT_sb_free(&command_sb);
        CONSTRUCT_return_defer(false);
    }
    CloseHandle(pi.hThread);
    CONSTRUCT_sb_free(&command_sb);
    CONSTRUCT_Proc proc = pi.hProcess;
    #else
    pid_t pid = fork();
    if (pid == -1) {
        CONSTRUCT_print_log(CONSTRUCT_ERROR, "fork failed: %s", strerror(errno));
        CONSTRUCT_sb_free(&command_sb);
        CONSTRUCT_return_defer(false);
    }
    if (pid == 0) {
        int exit_code = system(command_sb.items);
        exit(WEXITSTATUS(exit_code));
    }
    CONSTRUCT_sb_free(&command_sb);
    CONSTRUCT_Proc proc = pid;
    #endif
    if (options.async) {
        if (proc == CONSTRUCT_INVALID_PROC) CONSTRUCT_return_defer(false);
        CONSTRUCT_da_append(options.async, proc);
    } 
    else {
        while (true) {
            int ret = CONSTRUCT_proc_wait(proc);
            if (ret == -1) CONSTRUCT_return_defer(false);
            if (ret == 1) break;
            #ifdef _WIN32
            Sleep(10);
            #else
            usleep(10000);
            #endif
        }
    }
    
defer:
    CONSTRUCT_da_reset(command);
    return result;
}

//================================================================
//SECTION: CONSTRUCT build system (Dynammic Arrays, File Handling, Processes, Commands)
//================================================================

typedef struct {
    CONSTRUCT_Command command;
    CONSTRUCT_CommandOptions options;
} CONSTRUCT_CommandWithOptions;

typedef struct {char** targets; size_t targets_count; char** dependencies; size_t dependencies_count; CONSTRUCT_CommandWithOptions command_with_options;} CONSTRUCT_ConstructRule;

CONSTRUCT_da_define(CONSTRUCT_ConstructRule, CONSTRUCT_ConstructRules);

CONSTRUCT_da_define(bool, CONSTRUCT_Bools);

CONSTRUCT_DECLSPEC bool CONSTRUCT_run_construct(CONSTRUCT_ConstructRules* construct_rules){
    bool result=true;
    CONSTRUCT_Bools to_run={0};
    CONSTRUCT_da_resize(&to_run, construct_rules->count);
    bool done = false;
    while (!done) {
        size_t run_count = 0;
        for (size_t i = 0; i < construct_rules->count; ++i) {
            bool runnable = false;
            for (size_t j = 0; j < construct_rules->items[i].targets_count; ++j){
                if (construct_rules->items[i].dependencies_count == 0) { runnable = true; break; }
                    for (size_t k = 0; k < construct_rules->items[i].dependencies_count; ++k) {
                        if (!CONSTRUCT_file_exists(construct_rules->items[i].dependencies[k])) {runnable = false; break;}
                        if (!CONSTRUCT_file_exists(construct_rules->items[i].targets[j]) || CONSTRUCT_is_newer(construct_rules->items[i].dependencies[k], construct_rules->items[i].targets[j])) {runnable = true;}
                }
                if (!runnable) break;
            }
            to_run.items[i] = runnable;
            if (runnable) run_count++;
        }
        if (run_count == 0) {done = true; break;}
        for (size_t i = 0; i < construct_rules->count; ++i) {
            if (!to_run.items[i]) continue;
            if (!CONSTRUCT_command_run_with_options(&construct_rules->items[i].command_with_options.command, construct_rules->items[i].command_with_options.options)) CONSTRUCT_return_defer(false);
            if (construct_rules->items[i].command_with_options.options.async && !CONSTRUCT_procs_flush(construct_rules->items[i].command_with_options.options.async)) CONSTRUCT_return_defer(false);
        }
    }
defer:
    CONSTRUCT_da_free(&to_run);
    return result;
}

#endif // _CONSTRUCT_H

#ifndef CONSTRUCT_STRIP_PREFIX_GUARD_
#define CONSTRUCT_STRIP_PREFIX_GUARD_
    #ifndef CONSTRUCT_DONT_STRIP_PREFIX
        #define Proc CONSTRUCT_Proc 
        #define INVALID_PROC CONSTRUCT_INVALID_PROC 
        #define da_define CONSTRUCT_da_define
        #define da_reserve CONSTRUCT_da_reserve
        #define da_append CONSTRUCT_da_append
        #define da_append_many CONSTRUCT_da_append_many
        #define da_resize CONSTRUCT_da_resize
        #define da_items CONSTRUCT_da_items
        #define da_get CONSTRUCT_da_get
        #define da_last CONSTRUCT_da_last
        #define da_remove_unordered CONSTRUCT_da_remove_unordered
        #define da_foreach CONSTRUCT_da_foreach
        #define da_reset CONSTRUCT_da_reset
        #define da_free CONSTRUCT_da_free
        #define STRINGIFY CONSTRUCT_STRINGIFY
        #define TOSTRING CONSTRUCT_TOSTRING
        #define UNUSED CONSTRUCT_UNUSED
        #define TODO CONSTRUCT_TODO
        #define PANIC CONSTRUCT_PANIC
        #define UNREACHABLE CONSTRUCT_UNREACHABLE
        #define pop_first CONSTRUCT_pop_first
        #define return_defer CONSTRUCT_return_defer
        #define TIMER_START CONSTRUCT_TIMER_START
        #define TIMER_END CONSTRUCT_TIMER_END
        #define TIMER_SEC CONSTRUCT_TIMER_SEC
        #define AL CONSTRUCT_AL
        #define SA CONSTRUCT_SA
        #define expect_tt CONSTRUCT_expect_tt
        #define expect_ft CONSTRUCT_expect_ft
        #define expect_pt CONSTRUCT_expect_pt
        #define expect_tf CONSTRUCT_expect_tf
        #define expect_ff CONSTRUCT_expect_ff
        #define expect_pf CONSTRUCT_expect_pf
        #define d_expect_tt CONSTRUCT_d_expect_tt
        #define d_expect_ft CONSTRUCT_d_expect_ft
        #define d_expect_pt CONSTRUCT_d_expect_pt
        #define d_expect_tf CONSTRUCT_d_expect_tf
        #define d_expect_ff CONSTRUCT_d_expect_ff
        #define d_expect_pf CONSTRUCT_d_expect_pf
        #define u_expect_t CONSTRUCT_u_expect_t
        #define u_expect_f CONSTRUCT_u_expect_f
        #define u_expect_p CONSTRUCT_u_expect_p
        #define INFO CONSTRUCT_INFO
        #define WARNING CONSTRUCT_WARNING
        #define ERROR CONSTRUCT_ERROR
        #define LogType CONSTRUCT_LogType
        #define print_log CONSTRUCT_print_log
        #define print_log_to_file CONSTRUCT_print_log_to_file
        #define StringBuilder CONSTRUCT_StringBuilder
        #define sb_append_chr CONSTRUCT_sb_append_chr
        #define sb_append_buf CONSTRUCT_sb_append_buf
        #define sb_append_str CONSTRUCT_sb_append_str
        #define sb_append_null CONSTRUCT_sb_append_null
        #define sb_items CONSTRUCT_sb_items
        #define sb_get CONSTRUCT_sb_get
        #define sb_reset CONSTRUCT_sb_reset
        #define sb_free CONSTRUCT_sb_free
        #define sb_last CONSTRUCT_sb_last
        #define sb_remove_unordered CONSTRUCT_sb_remove_unordered
        #define sb_foreach CONSTRUCT_sb_foreach
        #define SB_Fmt CONSTRUCT_SB_Fmt
        #define SB_Arg CONSTRUCT_SB_Arg
        #define StringView CONSTRUCT_StringView
        #define sv_until_delim CONSTRUCT_sv_until_delim
        #define sv_trim_left CONSTRUCT_sv_trim_left
        #define sv_trim_right CONSTRUCT_sv_trim_right
        #define sv_strip_left CONSTRUCT_sv_strip_left
        #define sv_strip_right CONSTRUCT_sv_strip_right
        #define sv_strip CONSTRUCT_sv_strip
        #define sv_eq CONSTRUCT_sv_eq
        #define sv_ends_with CONSTRUCT_sv_ends_with
        #define sv_starts_with CONSTRUCT_sv_starts_with
        #define sv_to_cstr CONSTRUCT_sv_to_cstr
        #define sv_from_cstr CONSTRUCT_sv_from_cstr
        #define sv_from_parts CONSTRUCT_sv_from_parts
        #define sv_len CONSTRUCT_sv_len
        #define SV_Fmt CONSTRUCT_SV_Fmt
        #define SV_Arg CONSTRUCT_SV_Arg
        #define Temp CONSTRUCT_Temp
        #define temp_sprintf CONSTRUCT_temp_sprintf
        #define temp_strdup CONSTRUCT_temp_strdup
        #define temp_strndup CONSTRUCT_temp_strndup
        #define temp_reset CONSTRUCT_temp_reset
        #define temp_free CONSTRUCT_temp_free
        #define file_exists CONSTRUCT_file_exists
        #define get_mtime CONSTRUCT_get_mtime
        #define is_newer CONSTRUCT_is_newer
        #define path_basename CONSTRUCT_path_basename
        #define mkdir_if_not_exists CONSTRUCT_mkdir_if_not_exists
        #define copy_file CONSTRUCT_copy_file
        #define copy_directory_recursively CONSTRUCT_copy_directory_recursively
        #define move_file CONSTRUCT_move_file
        #define delete_file CONSTRUCT_delete_file
        #define delete_directory CONSTRUCT_delete_directory
        #define fetch_file CONSTRUCT_fetch_file
        #define DABytes CONSTRUCT_DABytes
        #define read_entire_file CONSTRUCT_read_entire_file
        #define write_entire_file CONSTRUCT_write_entire_file
        #define mkdir_if_not_exists_l CONSTRUCT_mkdir_if_not_exists_l
        #define copy_file_l CONSTRUCT_copy_file_l
        #define copy_directory_recursively_l CONSTRUCT_copy_directory_recursively_l
        #define move_file_l CONSTRUCT_move_file_l
        #define delete_file_l CONSTRUCT_delete_file_l
        #define delete_directory_l CONSTRUCT_delete_directory_l
        #define fetch_file_l CONSTRUCT_fetch_file_l
        #define read_entire_file_l CONSTRUCT_read_entire_file_l
        #define write_entire_file_l CONSTRUCT_write_entire_file_l
        #define Procs CONSTRUCT_Procs
        #define proc_wait CONSTRUCT_proc_wait
        #define procs_wait CONSTRUCT_procs_wait
        #define procs_flush CONSTRUCT_procs_flush
        #define Command CONSTRUCT_Command
        #define CommandOptions CONSTRUCT_CommandOptions
        #define command_append CONSTRUCT_command_append
        #define command_extend CONSTRUCT_command_extend
        #define command_free CONSTRUCT_command_free
        #define command_render CONSTRUCT_command_render
        #define command_run CONSTRUCT_command_run
        #define command_run_with_options CONSTRUCT_command_run_with_options
        #define CommandWithOptions CONSTRUCT_CommandWithOptions
        #define ConstructRule CONSTRUCT_ConstructRule
        #define ConstructRules CONSTRUCT_ConstructRules
        #define Bools CONSTRUCT_Bools
        #define run_construct CONSTRUCT_run_construct
    #endif
#endif // CONSTRUCT_STRIP_PREFIX_GUARD_