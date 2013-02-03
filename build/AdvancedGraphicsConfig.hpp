#ifndef HDRIMAGE_CONFIG_HPP_INCLUDED
#define HDRIMAGE_CONFIG_HPP_INCLUDED

/* Version. */
#define HDRIMAGE_MAJOR_VERSION 
#define HDRIMAGE_MINOR_VERSION 
#define HDRIMAGE_PATCH_VERSION 

/* Endianess */
#define CPU_ENDIANNESS    0
#define CPU_BIG_ENDIAN    1
#define CPU_LITTLE_ENDIAN 0

/* Compiler */
#define GNU_C_COMPILER
#define INTEL_CXX_COMPILER

/* OS */
#define UNIX
/* #undef WIN32 */

/* #undef ARCHx86_32 */
#define ARCHx86_64

#define PAGE_SIZE 4096
#define SSE_ALIGN 8 * 8

/* Compiler specific instructions. */
#if defined( INTEL_CXX_COMPILER )
#    define INLINE        inline __forceinline
#    define ALIGN( X )    X
#    define PURE
#    define HOT
#    define RESTRICT      __restrict__
#    define FASTCALL( X ) X
#elif defined( GNU_CXX_COMPILER )
#    define INLINE        inline __attribute__((always_inline))
#    define ALIGN( X, A ) __attribute__((aligned(A))) X
#    define PURE          __attribute__((pure))
#    define HOT           __attribute__((hot))
#    define RESTRICT      __restrict_arr
#    define FASTCALL( X ) X __attribute__((fastcall))
#else
#    error "Unsupported compiler. Please use gcc or icc."
#endif

/* Standard C++ Library. */
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <limits>
#include <cassert>
#include <array>
#include <cmath>
#include <cstring>

#endif
