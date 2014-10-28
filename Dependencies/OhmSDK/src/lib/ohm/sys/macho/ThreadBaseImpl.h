/*****************************************************************************

        ThreadBaseImpl.h
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55115

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

#include "ohm/sys/Mutex.h"



namespace ohm
{
namespace sys
{



class ThreadBase;

class ThreadBaseImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ThreadBaseImpl (ThreadBase * thread_ptr);
   virtual        ~ThreadBaseImpl ();
   
   int            run ();
   
   bool           wait_for_death (double timeout);
   void           kill ();
   
   MPTaskID       _task_id;
   ThreadBase *   _thread_ptr;
   MPSemaphoreID  _wait_id;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static OSStatus
                  dispatch_and_run (void * parameter);
   
   void           init ();
   void           restore ();



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
