/*****************************************************************************

        ThreadMgr.h
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 64156

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



#if ! defined (ohm_sys_ThreadMgr_HEADER_INCLUDED)
#define  ohm_sys_ThreadMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   // Nothing

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #if (ohm_archi_ARCH == ohm_archi_ARCH_MACHO)
   #else
      #pragma message ("unsupported architecture")
   #endif

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)
   #include <pthread.h>

#elif (ohm_archi_SYS == ohm_archi_SYS_BEOS)
   #include <kernel/OS.h>

#else
   #error Unsupported operating system.

#endif



namespace ohm
{
namespace sys
{



class ThreadMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Priority
   {
      Priority_LOW    = -1,
      Priority_NORMAL =  0,
      Priority_HIGH   =  1
   };

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   typedef  unsigned long  IdType;  // ::DWORD actually, which is defined as an unsigned long in windef.h.

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   typedef  MPTaskID    IdType;

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)
   typedef  pthread_t   IdType;

#elif (ohm_archi_SYS == ohm_archi_SYS_BEOS)
   typedef  thread_id   IdType;

#else
   #error Unsupported operating system.

#endif

   static void    yield ();
   static void    sleep (double time_s);
   static IdType  get_current_thread_id ();
   static IdType  get_invalid_thread_id ();
   static void    set_thread_name (const char *name_0);
   static int     count_nbr_logical_proc ();
   static int     set_priority (Priority prio);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ThreadMgr ();
                  ~ThreadMgr ();
                  ThreadMgr (const ThreadMgr &other);
   ThreadMgr &    operator = (const ThreadMgr &other);
   bool           operator == (const ThreadMgr &other);
   bool           operator != (const ThreadMgr &other);

}; // class ThreadMgr



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/ThreadMgr.hpp"



#endif   // ohm_sys_ThreadMgr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
