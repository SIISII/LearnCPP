#pragma once

// =============================================================================
//
// Warning control
//
// =============================================================================

// 'XXXX': function not inlined
#pragma warning(disable: 4710)

// function 'XXXX' selected for automatic inline expansion
#pragma warning(disable: 4711)

// Compiler will insert Spectre mitigation for memory load if /Qspectre switch
// specified
#pragma warning(disable: 5045)


#define  THIRD_PARTY_START                                                  \
    __pragma( warning(push) )                                               \
    __pragma( warning(disable: 4005) )                                      \
    __pragma( warning(disable: 4365) )                                      \
    __pragma( warning(disable: 4514) )                                      \
    __pragma( warning(disable: 4668) )                                      \
    __pragma( warning(disable: 4820) )                                      \
    __pragma( warning(disable: 5029) )                                      \
    __pragma( warning(disable: 5039) )


#define  THIRD_PARTY_END                                                    \
    __pragma( warning(pop) )


// =============================================================================
//
// Simple types
//
// =============================================================================

#include  <cstdint>


using  uint8        = std::uint8_t;
using  uint16       = std::uint16_t;
using  uint32       = std::uint32_t;
using  uint64       = std::uint64_t;

using  int8         = std::int8_t;
using  int16        = std::int16_t;
using  int32        = std::int32_t;
using  int64        = std::int64_t;

using  char8        = char8_t;
using  char16       = char16_t;
using  char32       = char32_t;

using  sint         = std::intptr_t;
using  uint         = std::uintptr_t;

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
