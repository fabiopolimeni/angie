// Copyright (c) 2017 Fabio Polimeni
// 18/03/2017
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

/*
 * List of supported operating systems (ANGIE_OS_x):
 *
 * DARWIN            - Any Darwin based system
 * DARWIN32          - Any 32-bit Darwin based system
 * DARWIN64          - Any 64-bit Darwin based system
 * BSD4              - Any BSD 4.x based system
 * FREEBSD           - FreeBSD
 * DRAGONFLY         - DragonFlyBSD
 * NETBSD            - NetBSD
 * OPENBSD           - OpenBSD
 * AIX               - IBM AIX
 * HPUX              - HP-UX
 * TRU64             - Tru64
 * SOLARIS           - Sun (Oracle) Solaris
 * QNX               - QNX 4.x
 * QNX6              - QNX Neutrino 6.x
 * SCO               - SCO OpenServer 5/6
 * UNIXWARE          - UnixWare 7
 * IRIX              - SGI IRIX
 * HAIKU             - Haiku
 * SYLLABLE          - Syllable
 * BEOS              - BeOS
 * OS2               - OS/2
 * VMS               - OpenVMS
 * UNIX              - Any UNIX BSD/SYSV based system
 * LINUX             - Linux
 * MAC9              - Mac OS 9 (Classic)
 * MAC               - Any macOS
 * MAC32             - 32-bit macOS
 * MAC64             - 64-bit macOS
 * CYGWIN            - Cygwin
 * WIN               - 32-bit Windows
 * WIN64             - 64-bit Windows
 */

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define ANGIE_OS_DARWIN
#  define ANGIE_OS_BSD4
#  ifdef __LP64__
#    define ANGIE_OS_DARWIN64
#  else
#    define ANGIE_OS_DARWIN32
#  endif
# elif defined(Macintosh) || defined(macintosh)
#  define ANGIE_OS_MAC9
#elif defined(__MSYS__)
#  define ANGIE_OS_MSYS
#elif defined(__CYGWIN__)
#  define ANGIE_OS_CYGWIN
#elif defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64)
#  define ANGIE_OS_WIN64
#elif defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#  define ANGIE_OS_WIN
#elif defined(__linux) || defined(__linux__)
#  define ANGIE_OS_LINUX
#elif defined(__FreeBSD__)
#  define ANGIE_OS_FREEBSD
#  define ANGIE_OS_BSD4
#elif defined(__DragonFly__)
#  define ANGIE_OS_DRAGONFLY
#  define ANGIE_OS_BSD4
#elif defined(__NetBSD__)
#  define ANGIE_OS_NETBSD
#  define ANGIE_OS_BSD4
#elif defined(__OpenBSD__)
#  define ANGIE_OS_OPENBSD
#  define ANGIE_OS_BSD4
#elif defined(_AIX)
#  define ANGIE_OS_AIX
#elif defined(hpux) || defined(__hpux)
#  define ANGIE_OS_HPUX
#elif defined(__osf__) || defined(__osf)
#  define ANGIE_OS_TRU64
#elif defined(__sun) || defined(sun)
#  define ANGIE_OS_SOLARIS
#elif defined(__QNXNTO__)
#  define ANGIE_OS_QNX6
#elif defined(__QNX__)
#  define ANGIE_OS_QNX
#elif defined(_SCO_DS)
#  define ANGIE_OS_SCO
#elif defined(__USLC__) || defined(__UNIXWARE__)
#  define ANGIE_OS_UNIXWARE
#elif defined(__svr4__) && defined(i386)
#  define ANGIE_OS_UNIXWARE
#elif defined(__sgi) || defined(sgi)
#  define ANGIE_OS_IRIX
#elif defined(__HAIKU__)
#  define ANGIE_OS_HAIKU
#elif defined(__SYLLABLE__)
#  define ANGIE_OS_SYLLABLE
#elif defined(__BEOS__)
#  define ANGIE_OS_BEOS
#elif defined(__OS2__)
#  define ANGIE_OS_OS2
#elif defined(VMS) || defined(__VMS)
#  define ANGIE_OS_VMS
#endif

#ifdef ANGIE_OS_WIN64
#  define ANGIE_OS_WIN
#endif

#if defined(ANGIE_OS_DARWIN)
#  define ANGIE_OS_MAC
#  if defined(ANGIE_OS_DARWIN64)
#     define ANGIE_OS_MAC64
#  elif defined(ANGIE_OS_DARWIN32)
#     define ANGIE_OS_MAC32
#  endif
#endif

#if defined(ANGIE_OS_WIN) || defined(ANGIE_OS_MAC9)
#  undef ANGIE_OS_UNIX
#elif !defined(ANGIE_OS_UNIX)
#  define ANGIE_OS_UNIX
#endif

/*
 * List of supported compilers (ANGIE_CC_x):
 *
 * MSVC            - Microsoft Visual C/C++
 * GNU             - GNU C/C++
 * MINGW           - MinGW C/C++
 * INTEL           - Intel C/C++
 * CLANG           - LLVM Clang C/C++
 * SUN             - Sun WorkShop/Studio C/C++
 * XLC             - IBM XL C/C++
 * HP              - HP C/aC++
 * DEC             - DEC C/C++
 * MIPS            - MIPSpro C/C++
 * USLC            - SCO OUDK and UDK C/C++
 * WATCOM          - Watcom C/C++
 * BORLAND         - Borland C/C++
 * PGI             - Portland Group C/C++
 */

#if defined(_MSC_VER)
#  define ANGIE_CC_MSVC
#  if defined(__INTEL_COMPILER)
#    define ANGIE_CC_INTEL
#  endif
#  if defined(__clang__)
#    define ANGIE_CC_CLANG
#  endif
#elif defined(__GNUC__)
#  define ANGIE_CC_GNU
#  if defined(__MINGW32__)
#    define ANGIE_CC_MINGW
#  endif
#  if defined(__INTEL_COMPILER)
#    define ANGIE_CC_INTEL
#  endif
#  if defined(__clang__)
#    define ANGIE_CC_CLANG
#  endif
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#  define ANGIE_CC_SUN
#elif defined(__xlc__) || defined(__xlC__)
#  define ANGIE_CC_XLC
#elif defined(__HANGIE_cc) || defined(__HANGIE_aCC)
#  define ANGIE_CC_HP
#elif defined (__DECC) || defined(__DECCXX)
#  define ANGIE_CC_DEC
#elif (defined(__sgi) || defined(sgi)) && \
      (defined(_COMPILER_VERSION) || defined(_SGI_COMPILER_VERSION))
#  define ANGIE_CC_MIPS
#elif defined(__USLC__) && defined(__SCO_VERSION__)
#  define ANGIE_CC_USLC
#elif defined(__WATCOMC__)
#  define ANGIE_CC_WATCOM
#elif defined(__BORLANDC__)
#  define ANGIE_CC_BORLAND
#elif defined(__INTEL_COMPILER)
#  define ANGIE_CC_INTEL
#elif defined(__PGI)
#  define ANGIE_CC_PGI
#endif

/**
 * @def ANGIE_NO_RETURN
 * @brief Notifies a compiler that a function will never return a value (i.e.
 * due to the abort () call).
 * @since 0.0.1
 */
#if defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#  define ANGIE_NO_RETURN _Noreturn
#elif defined(ANGIE_CC_MSVC) || (defined(ANGIE_CC_BORLAND) && __BORLANDC__ >= 0x0550)
#  define ANGIE_NO_RETURN __declspec(noreturn)
#elif __has_attribute(noreturn) || \
      defined(ANGIE_CC_GNU) || \
     (defined(ANGIE_CC_SUN) && __SUNPRO_C >= 0x5110) || \
     (defined(ANGIE_CC_SUN) && __SUNPRO_CC >= 0x5110)
#  define ANGIE_NO_RETURN __attribute__((noreturn))
#else
#  define ANGIE_NO_RETURN
#endif

/**
 * @def ANGIE_END_DECLS
 * @brief Closes .h file declarations to be exported as C functions, should be
 * always used after #ANGIE_BEGIN_DECLS.
 * @since 0.0.1
 */
#ifdef __cplusplus
#  define ANGIE_BEGIN_DECLS extern "C" {
#  define ANGIE_END_DECLS }
#else
#  define ANGIE_BEGIN_DECLS
#  define ANGIE_END_DECLS
#endif