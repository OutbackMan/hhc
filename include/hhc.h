#if !defined(HHC_H)
#define HHC_H

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdint.h>

#define KILOBYTES(amount) ((amount) * 1024LL) 
#define MEGABYTES(amount) (KILOBYTES(amount) * 1024LL) 
#define GIGABYTES(amount) (MEGABYTES(amount) * 1024LL) 

#define FAIL_IF_FALSE(static_expr) \
 (0 * sizeof(struct { int flag_bitfield:(2 * (static_expr) - 1); })

#define IS_ARRAY(array) ((void *)&(array) == &(array)[0])
#define ARRAY_LENGTH(array) \
  ((sizeof(array) / sizeof(array[0])) + FAIL_IF_FALSE(IS_ARRAY(array)))

#define GLOBAL static
#define INTERNAL static
#define PERSIST static

#define FALLTHROUGH_INTENDED
#define NO_DEFAULT_CASE

#define TO_STRING(value) #value
#define STRINGIFY(value) TO_STRING(value)

#if defined(__GNUC__)
#define COMPILER "gcc-"STRINGIFY(__GNUC__)"." \
  STRINGIFY(__GNUC_MINOR__)"."STRINGIFY(__GNUC_PATCHLEVEL__)
#elif defined(__llvm__)
#define COMPILER "llvm-"STRINGIFY(__clang_major__)"." \
  STRINGIFY(__clang_minor__)"."STRINGIFY(__clang_patchlevel__)
#elif defined(_MSC_VER)
#if _MSC_VER <= 1800
#define COMPILER "MSVC 2013"
#elif _MSC_VER <= 1900
#define COMPILER "MSVC 2015"
#elif _MSC_VER <= 1912
#define COMPILER "MSVC 2017"
#else
#error "Visual studio compiler must be 2013 version or earlier"
#endif
#elif defined(__EMSCRIPTEN__)
#define COMPILE "emcc-"STRINGIFY(__EMSCRIPTEN_major__)"." \
  STRINGIFY(__EMSCRIPTEN_minor__)"."STRINGIFY(__EMSCRIPTEN_tiny__)
#else
#error "Compiler must be gcc, clang or visual studio"
#endif

#if defined(__GNUC__) || defined(__llvm__) 
#define ALIGN(val) __attribute__((aligned(val)))
#define PURE __attribute__((pure))
#define CONST __attribute__((const))
#define COLD __attribute__((cold))
#define HOT __attribute__((hot))
#define CHECK __attribute__((warn_unused_result))
#define DEPRECATED __attribute__((deprecated))
#define USED __attribute__((used))
#define UNUSED __attribute__((unused))
#define PACKED __attribute__((packed))
#define LIKELY(val) __builtin_expect(!!(val), 1)
#define UNLIKELY(val) __builtin_expect(!!(val), 0)
#define DESIGNATED __attribute__((designated_init))
#define FLATTEN __attribute__((flatten))
#elif defined (_MSC_VER)
#define inline __inline
#define ALIGN(val) __declspec(align(val))
#define DEPRECATED __declspec(deprecated)
#define CONST
#define COLD
#define HOT
#define CHECK
#define DEPRECATED
#define USED
#define UNUSED
#define PACKED
#define LIKELY(val)
#define UNLIKELY(val)
#define DESIGNATED
#define FLATTEN
#else
#define ALIGN(val)
#define PURE
#define CONST
#define COLD
#define HOT
#define CHECK
#define DEPRECATED
#define USED
#define UNUSED
#define PACKED
#define LIKELY(val)
#define UNLIKELY(val)
#define DESIGNATED
#define FLATTEN
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;


#endif
