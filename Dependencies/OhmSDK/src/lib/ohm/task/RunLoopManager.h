/*****************************************************************************

        RunLoopManager.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49379

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



#if ! defined (ohm_task_RunLoopManager_HEADER_INCLUDED)
#define  ohm_task_RunLoopManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/sys/ReadWriteLock.h"

#include <list>
#include <map>
#include <set>



namespace ohm
{
namespace task
{



class RunLoopBase;
class Timer;
class Event;

class RunLoopManager
:  public util::SingletonBase <RunLoopManager>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~RunLoopManager ();
   
   RunLoopBase &  get_current ();

   void           push_current (RunLoopBase & run_loop);
   void           pop_current (RunLoopBase & run_loop);
   
   void           timer_fired (const std::pair <RunLoopBase *, Timer *> & run_loop_timer);
   void           send (RunLoopBase * run_loop_ptr, Event & event);
      


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  RunLoopManager ();
   
   // called from 'RunLoopBase'



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (RunLoopManager);
   
   typedef std::set <RunLoopBase *> RunLoopPtrSet;
   typedef std::list <RunLoopBase *>   RunLoopPtrList;
   typedef std::map <sys::ThreadMgr::IdType, RunLoopPtrList> ThreadRunLoopMap;
   
   sys::ReadWriteLock
                  _run_loop_maps_rwlock;
   ThreadRunLoopMap
                  _thread_run_loop_map;
   RunLoopPtrSet  _run_loop_set;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RunLoopManager (const RunLoopManager &other);
   RunLoopManager &
                  operator = (const RunLoopManager &other);
   bool           operator == (const RunLoopManager &other);
   bool           operator != (const RunLoopManager &other);

}; // class RunLoopManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/private/RunLoopManager.hpp"



#endif   // ohm_task_RunLoopManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
