#ifndef _MUS_LOG_H
#define _MUS_LOG_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

typedef enum {MUS_INFO, MUS_WARNING, MUS_ERROR} MUS_LogType;

static inline void MUS_print_log(MUS_LogType level, const char* fmt, ...) {
    switch (level) {
        case MUS_INFO: 
            fprintf(stdout, "[INFO] "); 
            break;
        case MUS_WARNING: 
            #ifndef _WIN32
                fprintf(stdout, "\x1B[33m");
            #endif
            fprintf(stdout, "[WARNING] "); 
            break;
        case MUS_ERROR:
            #ifndef _WIN32 
                fprintf(stdout, "\x1B[31;1;4m");
            #endif
            fprintf(stdout, "[ERROR] "); 
            break;
    }
    va_list args;
    va_start(args, fmt);
    if (level == MUS_ERROR) {
        va_list args_copy;
        va_copy(args_copy, args);
        vfprintf(stderr, fmt, args_copy); 
        fprintf(stderr, "\n"); 
        fflush(stderr);
        va_end(args_copy);
    }
    vfprintf(stdout, fmt, args);
    va_end(args);
    #ifndef _WIN32
        fprintf(stdout, "\x1B[0m");
    #endif
    fprintf(stdout, "\n");
}

static inline void MUS_print_log_to_file(const char* file_path, MUS_LogType level, const char* fmt, ...) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    FILE* file=fopen(file_path, "a");
    if (file == NULL) return;
    fprintf(file, "[%d/%02d/%02d@%02d:%02d:%02d] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    switch (level) {
        case MUS_INFO: fprintf(file, "[INFO] "); break;
        case MUS_WARNING: fprintf(file, "[WARNING] "); break;
        case MUS_ERROR: fprintf(file, "[ERROR] "); break;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
    fprintf(file, "\n");
    fclose(file);
}

#endif //_MUS_LOG_H

#ifndef MUS_LOG_STRIP_PREFIX_GUARD_
#define MUS_LOG_STRIP_PREFIX_GUARD_
    #ifndef MUS_DONT_STRIP_PREFIX
        #define INFO MUS_INFO
        #define WARNING MUS_WARNING
        #define ERROR MUS_ERROR
        #define LogType MUS_LogType
        #define print_log MUS_print_log
        #define print_log_to_file MUS_print_log_to_file
    #endif
#endif // MUS_LOG_STRIP_PREFIX_GUARD_