/*****************************************************************************

        def.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 69933

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if defined (ohm_archi_def_CURRENT_HEADER)
   #error Recursive inclusion of ohm/archi/def header.
#endif
#define  ohm_archi_def_CURRENT_HEADER

#if ! defined (ohm_archi_def_HEADER_INCLUDED)
#define  ohm_archi_def_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif

#if defined (__BEOS__)
#include <SupportDefs.h>
#include <ByteOrder.h>
#endif


/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#define  ohm_archi_SYS_UNDEFINED       (1)
#define  ohm_archi_SYS_WIN             (2)
#define  ohm_archi_SYS_MACOS           (3)
#define  ohm_archi_SYS_BEOS            (4)
#define  ohm_archi_SYS_LINUX           (5)
                                    
#define  ohm_archi_ENDIAN_BIG          (1)
#define  ohm_archi_ENDIAN_LITTLE       (2)

// X86 is actually both x86 and x64 architectures.
#define  ohm_archi_PROC_X86            (1)
#define  ohm_archi_PROC_PPC            (2)
                                    
#define  ohm_archi_ARCH_CARBON         (1)
#define  ohm_archi_ARCH_MACHO          (2)

#define  ohm_archi_COMPILER_CLASS_STD  (1)
#define  ohm_archi_COMPILER_CLASS_VC   (2)
#define  ohm_archi_COMPILER_CLASS_CW   (3)
#define  ohm_archi_COMPILER_CLASS_GCC  (4)

// Keep compiler versions in ascending order
#define  ohm_archi_COMPILER_STD        (1)
#define  ohm_archi_COMPILER_UNKNOWN    (2)
#define  ohm_archi_COMPILER_VC6        (100)
#define  ohm_archi_COMPILER_VC2003     (101)
#define  ohm_archi_COMPILER_VC2005     (102)
#define  ohm_archi_COMPILER_VC2008     (103)
#define  ohm_archi_COMPILER_VC2010     (104)
#define  ohm_archi_COMPILER_CW7        (200)
#define  ohm_archi_COMPILER_CW8        (201)
#define  ohm_archi_COMPILER_CW9        (202)
#define  ohm_archi_COMPILER_GCC2       (300)
#define  ohm_archi_COMPILER_GCC3       (301)
#define  ohm_archi_COMPILER_GCC4       (302)
#define  ohm_archi_COMPILER_GCC4_2     (303)
#define  ohm_archi_COMPILER_GCC4_4     (304)
#define  ohm_archi_COMPILER_GCC4_6     (305)



// for correct inclusion of new platforms, see http://www.boost.org boost/config/select_platform_config.hpp

// System
#if defined(linux) || defined(__linux) || defined(__linux__) || defined (ohm_archi_EMULATE_LINUX_ON_MACOSX)
   #define  ohm_archi_SYS  (ohm_archi_SYS_LINUX)

#elif defined (_WIN32) || defined (WIN32) || defined (__WIN32__) || defined (__CYGWIN__) || defined (__CYGWIN32__)
   #define  ohm_archi_SYS  (ohm_archi_SYS_WIN)

#elif defined (__BEOS__)
   #define  ohm_archi_SYS  (ohm_archi_SYS_BEOS)

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
   #define  ohm_archi_SYS  (ohm_archi_SYS_MACOS)
   #if    (defined (__MACH__) && __MACH__)   \
       || (defined (__GNUC__) && (defined (__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__)))
      #define  ohm_archi_ARCH (ohm_archi_ARCH_MACHO)
   #else
      #define  ohm_archi_ARCH (ohm_archi_ARCH_CARBON)
   #endif
   
#else
   #error Unsupported operating system.

#endif



// Processor type
#if (ohm_archi_SYS == ohm_archi_SYS_WIN) || defined (__i386__) || defined (__x86_64__) || defined (__INTEL__)
   #define  ohm_archi_PROC (ohm_archi_PROC_X86)
#elif defined (__POWERPC__) || defined (__powerpc) || defined (_powerpc) || (__ppc__)
   #define  ohm_archi_PROC (ohm_archi_PROC_PPC)
#else
   #error Unsupported processor.
#endif



// Endian
#if (ohm_archi_PROC == ohm_archi_PROC_X86)
   #define  ohm_archi_ENDIAN  (ohm_archi_ENDIAN_LITTLE)
#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)
   #define  ohm_archi_ENDIAN  (ohm_archi_ENDIAN_BIG)
#else
   #error Unsupported processor architecture.
#endif



// Native word size, in bits
#if defined (_M_IA64) || defined (_WIN64) || defined (__64BIT__) || defined (__powerpc64__) || defined (__ppc64__) || defined (__x86_64__)
   #define  ohm_archi_WORD_SIZE  64
   #define  ohm_archi_WORD_SIZE_BYTE   8
#else
   #define  ohm_archi_WORD_SIZE  32
   #define  ohm_archi_WORD_SIZE_BYTE   4
#endif



// Compiler
#if defined (__MWERKS__)

   #define  ohm_archi_COMPILER_CLASS   (ohm_archi_COMPILER_CLASS_CW)

   #if ((__MWERKS__ & 0xFF00) == 0x2400)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_CW7)
   #elif ((__MWERKS__ & 0xFF00) == 0x3000)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_CW8)
   #elif ((__MWERKS__ & 0xFF00) == 0x3200)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_CW9)
   #else
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_UNKNOWN)
   #endif

#elif defined (__GNUC__)

   #define  ohm_archi_COMPILER_CLASS   (ohm_archi_COMPILER_CLASS_GCC)

   #if (__GNUC__ == 2)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_GCC2)
   #elif (__GNUC__ == 3)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_GCC3)
   #elif (__GNUC__ == 4)
      #if (__GNUC_MINOR__ == 0)
         #define  ohm_archi_COMPILER (ohm_archi_COMPILER_GCC4)
      #elif (__GNUC_MINOR__ == 2)
         #define  ohm_archi_COMPILER (ohm_archi_COMPILER_GCC4_2)
      #elif (__GNUC_MINOR__ == 4)
         #define  ohm_archi_COMPILER (ohm_archi_COMPILER_GCC4_4)
      #elif (__GNUC_MINOR__ == 6)
         #define  ohm_archi_COMPILER (ohm_archi_COMPILER_GCC4_6)
      #else
         #error unsupported GCC 4 minor version
      #endif
   #else
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_UNKNOWN)
   #endif

// Must be checked last, as other compiler could define this symbol.
#elif defined (_MSC_VER)

   #define  ohm_archi_COMPILER_CLASS   (ohm_archi_COMPILER_CLASS_VC)

   #if ((_MSC_VER / 100) == 12)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_VC6)
   #elif ((_MSC_VER / 100) == 13)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_VC2003)
   #elif ((_MSC_VER / 100) == 14)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_VC2005)
   #elif ((_MSC_VER / 100) == 15)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_VC2008)
   #elif ((_MSC_VER / 100) == 16)
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_VC2010)
   #else
      #define  ohm_archi_COMPILER (ohm_archi_COMPILER_UNKNOWN)
   #endif

#else

   #define  ohm_archi_COMPILER_CLASS   (ohm_archi_COMPILER_CLASS_STD)
   #define  ohm_archi_COMPILER         (ohm_archi_COMPILER_STD)

#endif



// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
// Compiler-specific features



// Wide character support
#define  ohm_archi_SUPPORT_WCHAR_T
#if    (ohm_archi_SYS == ohm_archi_SYS_WIN)  \
    && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
   // GCC/Cygwin doesn't support them
   #undef   ohm_archi_SUPPORT_WCHAR_T
#endif



// Prefix for function export
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
   #define  ohm_archi_CDECL   __cdecl

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_CW)
   #define  ohm_archi_CDECL

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
   #define  ohm_archi_CDECL

#else
   #error Unsupported Compiler.

#endif



// x is the function return value type
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
   #define  ohm_archi_EXPORT(x)  x

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_CW)
   #define  ohm_archi_EXPORT(x)  extern "C" x

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
   #define  ohm_archi_EXPORT(x)  extern "C" __attribute__((visibility("default"))) x
   
#else
   #error Unsupported Compiler.

#endif



// Data alignment for type declaration.
// alignsize is the alignment size
// The macro does not add the terminal semicolon.
//#* jno : maximum alignement on mac os x : 16 bytes
//#* jno : beware, on mac ox x, will not change the *size* of the structure,
// only the alignement. gcc forbids the use of arrays of problematic aligned
// structure (it won't compile)
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
   #define  ohm_archi_TYPEDEF_ALIGN( alignsize, srctype, dsttype) \
      typedef __declspec (align (alignsize)) srctype dsttype

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_CW)
   #error Undefined for this compiler.

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
   #define  ohm_archi_TYPEDEF_ALIGN( alignsize, srctype, dsttype) \
      typedef srctype __attribute__ ((aligned (alignsize))) dsttype

#else
   #error Unsupported Compiler.

#endif



// Function inlining
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
   #define  ohm_archi_FORCEINLINE   __forceinline
#else
   #define  ohm_archi_FORCEINLINE   inline
#endif



// Standard library namespace
#if (ohm_archi_COMPILER == ohm_archi_COMPILER_VC6)
   #define  ohm_archi_STD_NAME( x)  x
#else
   #define  ohm_archi_STD_NAME( x)  std::x
#endif



// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
// Architecture-specific features



// 64-bit and 128-bit Compare-And-Swap
#if (ohm_archi_PROC == ohm_archi_PROC_X86)

   #define  ohm_archi_HAS_CAS_64 1

   #if ohm_archi_WORD_SIZE == 64
      #define  ohm_archi_HAS_CAS_128   1
   #endif

#endif



namespace ohm
{
namespace archi
{



}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_archi_def_HEADER_INCLUDED

#undef ohm_archi_def_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
