#ifndef __ASM_OBFUSHEADER_
#define __ASM_OBFUSHEADER_

#include <cutils.h>

#ifdef __cplusplus
static constexpr int _CTime = __TIME__[0] + __TIME__[1] + __TIME__[3] + __TIME__[4] + __TIME__[6] + __TIME__[7];
#define __CTimeSeed ((__COUNTER__ + _CTime) * 2654435761u)
#else // for C we cannot base it on __TIME__, since there's no constexpr, or XX:XX:XX will be added to the binary
#define __CTimeSeed ((__COUNTER__ + __LINE__) * 2654435761u)
#endif
#define __RND(Min, Max) (Min + (__CTimeSeed % (Max - Min + 1)))

#ifndef INLINE
#ifdef _MSVC
#define INLINE __forceinline // Visual C++
#else
#define INLINE inline __attribute__((always_inline)) // GCC/G++/CLANG
#endif
#endif

#ifndef OB_JUNK
#ifdef NDEBUG
#define OB_JUNK() obfusheader_junk<__RND(1,6)>()
#else
#define OB_JUNK() ((void)0)
#endif
#endif

template <unsigned int Dummy>
static INLINE void obfusheader_junk() {
    __asm__ volatile("" ::: "memory");

    constexpr unsigned int v = Dummy;

    if constexpr (v == 1) {
        __asm__ volatile(
            "xor %%eax, %%eax\n"
            "jz 1f\n"
            "addl $0x11, %%esp\n"
            "1:\n"
            :
            :
            : "eax", "cc"
        );
    } else if constexpr (v == 2) {
        __asm__ volatile(
            "jmp 1f\n"
            ".byte 0xE8\n"
            "pause\n"
            "1:\n"
            :
            :
            : "cc"
        );
    } else if constexpr (v == 3) {
        __asm__ volatile(
            "push %%ebx\n"
            "xor %%ebx, %%ebx\n"
            "pause\n"
            "test %%ebx, %%ebx\n"
            "jnz 1f\n"
            "jz 2f\n"
            "1:\n"
            ".byte 0xC7\n"
            "2:\n"
            "pop %%ebx\n"
            :
            :
            : "ebx", "cc"
        );
    } else if constexpr (v == 4) {
        __asm__ volatile(
            "jmp 1f\n"
            ".byte 0xE8\n"
            "2:\n"
            "ret\n"
            ".byte 0xE8\n"
            "1:\n"
            "call 2b\n"
            :
            :
            : "memory", "cc"
        );
    }else if constexpr (v == 5) {
        __asm__ volatile(
            "call 1f\n"
            ".byte 0x90\n"
            "1:\n"
            "pop %%eax\n"
            "add $7, %%eax\n"
            "jmp *%%eax\n"
            :
            :
            : "eax", "memory", "cc"
       );
    }else if constexpr (v == 6) {
        __asm__ volatile(
            "push %%eax\n"
            "call 1f\n"
            ".byte 0x66\n"
            ".byte 0x2E\n"
            "1:\n"
            "pop %%eax\n"
            "rol $3, %%eax\n"
            "pop %%eax\n"   // restore
            :
            :
            : "memory","cc"
      );
    }
}


#endif