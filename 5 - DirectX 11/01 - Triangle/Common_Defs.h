// *****************************************************************************
//
// Definitions for wide-used common constants, types and macros.
//
// *****************************************************************************

#pragma once


// =============================================================================
//
// Warning control
//
// =============================================================================

#if  defined (_MSC_VER)

    // 'XXX': structure was padded due to alignment specifier
    #pragma warning(disable: 4324)

    // 'XXX': unreferenced inline function has been removed
    #pragma warning(disable: 4514)

    // 'XXX': 'N' bytes padding added after data member 'YYY'
    #pragma warning(disable: 4820)

    // Compiler will insert Spectre mitigation for memory load if /Qspectre
    // switch specified
    #pragma warning(disable: 5045)

    // 'xxxx': a non-static data member with a volatile qualified type no
    // longer implies that compiler generated copy/move constructors and
    // copy/move assignment operators are not trivial
    #pragma warning(disable: 5220)


    #define  WRN_NO_RETURN_ENABLE

    #define  WRN_NO_RETURN_DISABLE


    #define  THIRD_PARTY_START                                              \
        __pragma(warning(push))                                             \
        __pragma(warning(disable: 4200))                                    \
        __pragma(warning(disable: 4324))                                    \
        __pragma(warning(disable: 4365))                                    \
        __pragma(warning(disable: 4371))                                    \
        __pragma(warning(disable: 4541))                                    \
        __pragma(warning(disable: 4619))                                    \
        __pragma(warning(disable: 4625))                                    \
        __pragma(warning(disable: 4626))                                    \
        __pragma(warning(disable: 4643))                                    \
        __pragma(warning(disable: 4668))                                    \
        __pragma(warning(disable: 4800))                                    \
        __pragma(warning(disable: 5026))                                    \
        __pragma(warning(disable: 5027))                                    \
        __pragma(warning(disable: 5031))                                    \
        __pragma(warning(disable: 5039))


    #define  THIRD_PARTY_END  __pragma(warning(pop))

#else

    #error  "Undefined compiler"

#endif


// =============================================================================
//
// Necessary standard includes
//
// =============================================================================

THIRD_PARTY_START

#include  <cstddef>
#include  <cstdint>
#include  <type_traits>

THIRD_PARTY_END


// =============================================================================
//
// Standard macros
//
// =============================================================================

#define  BYTE0(X)  static_cast<uint8>(X)
#define  BYTE1(X)  static_cast<uint8>(X >> 8)
#define  BYTE2(X)  static_cast<uint8>(X >> 16)
#define  BYTE3(X)  static_cast<uint8>(X >> 24)
#define  BYTE4(X)  static_cast<uint8>(X >> 32)
#define  BYTE5(X)  static_cast<uint8>(X >> 40)
#define  BYTE6(X)  static_cast<uint8>(X >> 48)
#define  BYTE7(X)  static_cast<uint8>(X >> 56)

#define  MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define  MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define  COUNT_OF(Array)        (sizeof(Array) / sizeof(Array[0]))

#define  ALIGN_SIZE_TO_2(n)     ((sizeof(n) +  1u) & ~0x1u)
#define  ALIGN_SIZE_TO_4(n)     ((sizeof(n) +  3u) & ~0x3u)
#define  ALIGN_SIZE_TO_8(n)     ((sizeof(n) +  7u) & ~0x7u)
#define  ALIGN_SIZE_TO_16(n)    ((sizeof(n) + 15u) & ~0xFu)


// =============================================================================
//
// Simple types
//
// =============================================================================

using  uint8  = std::uint8_t;
using  uint16 = std::uint16_t;
using  uint32 = std::uint32_t;
using  uint64 = std::uint64_t;

using  int8   = std::int8_t;
using  int16  = std::int16_t;
using  int32  = std::int32_t;
using  int64  = std::int64_t;

using  char8  = char8_t;

using  char16 = char16_t;
using  char32 = char32_t;
using  wchar  = wchar_t;

using  sint   = std::intptr_t;
using  uint   = std::uintptr_t;

using  pvoid        =                void *;
using  pcvoid       = const          void *;
using  pvvoid       =       volatile void *;
using  pcvvoid      = const volatile void *;

using  pint8        =                int8  *;
using  pcint8       = const          int8  *;
using  pvint8       =       volatile int8  *;
using  pcvint8      = const volatile int8  *;
using  pint16       =                int16 *;
using  pcint16      = const          int16 *;
using  pvint16      =       volatile int16 *;
using  pcvint16     = const volatile int16 *;
using  pint32       =                int32 *;
using  pcint32      = const          int32 *;
using  pvint32      =       volatile int32 *;
using  pcvint32     = const volatile int32 *;
using  pint64       =                int64 *;
using  pcint64      = const          int64 *;
using  pvint64      =       volatile int64 *;
using  pcvint64     = const volatile int64 *;

using  puint8       =                uint8  *;
using  pcuint8      = const          uint8  *;
using  pvuint8      =       volatile uint8  *;
using  pcvuint8     = const volatile uint8  *;
using  puint16      =                uint16 *;
using  pcuint16     = const          uint16 *;
using  pvuint16     =       volatile uint16 *;
using  pcvuint16    = const volatile uint16 *;
using  puint32      =                uint32 *;
using  pcuint32     = const          uint32 *;
using  pvuint32     =       volatile uint32 *;
using  pcvuint32    = const volatile uint32 *;
using  puint64      =                uint64 *;
using  pcuint64     = const          uint64 *;
using  pvuint64     =       volatile uint64 *;
using  pcvuint64    = const volatile uint64 *;

using  psint        =                sint *;
using  pcsint       = const          sint *;
using  pvsint       =       volatile sint *;
using  pcvsint      = const volatile sint *;

using  puint        =                uint *;
using  pcuint       = const          uint *;
using  pvuint       =       volatile uint *;
using  pcvuint      = const volatile uint *;

using  pchar        =                char *;
using  pcchar       = const          char *;
using  pvchar       =       volatile char *;
using  pcvchar      = const volatile char *;

using  pchar8       =                char8 *;
using  pcchar8      = const          char8 *;
using  pvchar8      =       volatile char8 *;
using  pcvchar8     = const volatile char8 *;

using  pchar16      =                char16 *;
using  pcchar16     = const          char16 *;
using  pvchar16     =       volatile char16 *;
using  pcvchar16    = const volatile char16 *;

using  pchar32      =                char32 *;
using  pcchar32     = const          char32 *;
using  pvchar32     =       volatile char32 *;
using  pcvchar32    = const volatile char32 *;
