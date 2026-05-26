#ifndef _MUS_MISC_H
#define _MUS_MISC_H

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#define MUS_STRINGIFY(x) #x
#define MUS_TOSTRING(x) MUS_STRINGIFY(x)

#define MUS_UNUSED(...) (void)(__VA_ARGS__)
#define MUS_TODO(message) do {printf("%s:%d: TODO: %s\n", __FILE__, __LINE__, message); abort();} while(0)

#define MUS_PANIC(message) do {printf("%s:%d: PANIC: %s\n", __FILE__, __LINE__, message); abort();} while(0)
#define MUS_UNREACHABLE() do {printf("%s:%d: UNREACHABLE\n", __FILE__, __LINE__); abort();} while(0)

#define MUS_pop_first(xs, xs_sz) (assert((xs_sz) > 0), (xs_sz)--, *(xs)++)
#define MUS_return_defer(value) do { result = (value); goto defer; } while(0)

#ifdef MUS_CLOCK 
    static clock_t MUS_c1, MUS_c2;
#endif
#ifdef MUS_TIMER
    static time_t MUS_t1, MUS_t2;
#endif
#define MUS_CLOCK_START MUS_c1=clock()
#define MUS_CLOCK_END MUS_c2=clock()
#define MUS_CLOCK_SEC (((float)(MUS_c2 - MUS_c1))/CLOCKS_PER_SEC)
#define MUS_TIMER_START MUS_t1=time(NULL)
#define MUS_TIMER_END MUS_t2=time(NULL)
#define MUS_TIMER_SEC (MUS_t2 - MUS_t1)

#define MUS_AL(Type, ...) (Type[]){__VA_ARGS__}, (sizeof((Type[]){__VA_ARGS__})/sizeof(Type))
#define MUS_SA(...) MUS_AL(char*, __VA_ARGS__)

#define MUS_expect_tt(exp) if(!(exp)) return 1
#define MUS_expect_ft(exp) if((exp)) return 1
#define MUS_expect_pt(exp) if((exp)<0) return 1
#define MUS_expect_tf(exp) if(!(exp)) return 0
#define MUS_expect_ff(exp) if((exp)) return 0
#define MUS_expect_pf(exp) if((exp)<0) return 0

#define MUS_d_expect_tt(exp) if(!(exp)) MUS_return_defer(1)
#define MUS_d_expect_ft(exp) if((exp)) MUS_return_defer(1)
#define MUS_d_expect_pt(exp) if((exp)<0) MUS_return_defer(1)
#define MUS_d_expect_tf(exp) if(!(exp)) MUS_return_defer(0)
#define MUS_d_expect_ff(exp) if((exp)) MUS_return_defer(0)
#define MUS_d_expect_pf(exp) if((exp)<0) MUS_return_defer(0)

#define MUS_u_expect_t(exp) if(!(exp)) MUS_UNREACHABLE()
#define MUS_u_expect_f(exp) if((exp)) MUS_UNREACHABLE()
#define MUS_u_expect_p(exp) if((exp)<0) MUS_UNREACHABLE()

#endif // _MUS_MISC_H

#ifndef MUS_MISC_STRIP_PREFIX_GUARD_
#define MUS_MISC_STRIP_PREFIX_GUARD_
    #ifndef MUS_DONT_STRIP_PREFIX
        #define STRINGIFY MUS_STRINGIFY
        #define TOSTRING MUS_TOSTRING
        #define UNUSED MUS_UNUSED
        #define TODO MUS_TODO
        #define PANIC MUS_PANIC
        #define UNREACHABLE MUS_UNREACHABLE
        #define pop_first MUS_pop_first
        #define return_defer MUS_return_defer
        #define CLOCK_START MUS_CLOCK_START
        #define CLOCK_END MUS_CLOCK_END
        #define CLOCK_SEC MUS_CLOCK_SEC
        #define TIMER_START MUS_TIMER_START
        #define TIMER_END MUS_TIMER_END
        #define TIMER_SEC MUS_TIMER_SEC
        #define AL MUS_AL
        #define SA MUS_SA
        #define expect_tt MUS_expect_tt
        #define expect_ft MUS_expect_ft
        #define expect_pt MUS_expect_pt
        #define expect_tf MUS_expect_tf
        #define expect_ff MUS_expect_ff
        #define expect_pf MUS_expect_pf
        #define d_expect_tt MUS_d_expect_tt
        #define d_expect_ft MUS_d_expect_ft
        #define d_expect_pt MUS_d_expect_pt
        #define d_expect_tf MUS_d_expect_tf
        #define d_expect_ff MUS_d_expect_ff
        #define d_expect_pf MUS_d_expect_pf
        #define u_expect_t MUS_u_expect_t
        #define u_expect_f MUS_u_expect_f
        #define u_expect_p MUS_u_expect_p
    #endif
#endif // MUS_MISC_STRIP_PREFIX_GUARD_