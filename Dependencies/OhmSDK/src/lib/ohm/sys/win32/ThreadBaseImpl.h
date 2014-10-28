/*****************************************************************************

        ThreadBaseImpl.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68831

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



#if defined (ohm_sys_ThreadBaseImpl_CURRENT_HEADER)
   #error Recursive inclusion of ohm/sys/ThreadBaseImpl header.
#endif
#define  ohm_sys_ThreadBaseImpl_CURRENT_HEADER

#if ! defined (ohm_sys_ThreadBaseImpl_HEADER_INCLUDED)
#define  ohm_sys_ThreadBaseImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#include <windows.h>



#define  ohm_sys_ThreadBase_CRT_IGNORE    0
#define  ohm_sys_ThreadBase_CRT_USE       1

// If the MS CRT is used, threads should be run with _beginthreadex and
// _endthreadex, instead of CreateThread or ExitThread.
#define  ohm_sys_ThreadBase_CRT           ohm_sys_ThreadBase_CRT_USE



namespace ohm
{
namespace sys
{



class ThreadBase;

class ThreadBaseImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ThreadBaseImpl ();

   ::HANDLE       _handle;
#if (ohm_sys_ThreadBase_CRT == ohm_sys_ThreadBase_CRT_USE)
   unsigned int   _id;
#else    // ohm_sys_ThreadBase_CRT
   ::DWORD        _id;
#endif   // ohm_sys_ThreadBase_CRT
   conc::AtomicInt <bool>
                  _run_flag;
   conc::AtomicInt <bool>        // Set when the thread is terminated or exited.
                  _term_flag;

#if (ohm_sys_ThreadBase_CRT == ohm_sys_ThreadBase_CRT_USE)
   static unsigned int __stdcall
                  dispatch_and_run (void *thread_ptr);
#else    // ohm_sys_ThreadBase_CRT
   static ::DWORD WINAPI
                  dispatch_and_run (::LPVOID thread_ptr);
#endif   // ohm_sys_ThreadBase_CRT



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ThreadBaseImpl (const ThreadBaseImpl &other);
   ThreadBaseImpl &
                  operator = (const ThreadBaseImpl &other);
   bool           operator == (const ThreadBaseImpl &other);
   bool           operator != (const ThreadBaseImpl &other);

}; // class ThreadBaseImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_ThreadBaseImpl_HEADER_INCLUDED

#undef ohm_sys_ThreadBaseImpl_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
