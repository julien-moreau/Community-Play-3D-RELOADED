/*****************************************************************************

        RunLoopGui.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59159

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



#if ! defined (ohm_task_RunLoopGui_HEADER_INCLUDED)
#define  ohm_task_RunLoopGui_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/RunLoopBase.h"
#include "ohm/util/SingletonBase.h"



namespace ohm
{
namespace task
{



class RunLoopGuiImpl;

class RunLoopGui
:  public RunLoopBase
,  public util::SingletonBase <RunLoopGui>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~RunLoopGui ();
   
   virtual double get_time ();
   
   void           quit ();
   
   void           notify_pre_gui_event ();
   void           notify_post_gui_event ();
   
   void           notify_post_external_modality ();
   
   RunLoopGuiImpl &
                  use_impl ();


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   friend class RunLoopGuiImpl;

                  RunLoopGui ();
   
   // RunLoopBase
   virtual bool   do_run ();
   virtual void   do_stop ();
   virtual void   do_signal ();
   virtual void   do_update_time ();
   
   // called from impl
   void           req_process ();


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (RunLoopGui);

   std::auto_ptr <RunLoopGuiImpl>
                  _impl_aptr;
   double         _time_s;
   bool           _processing_gui_event_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RunLoopGui (const RunLoopGui &other);
   RunLoopGui &   operator = (const RunLoopGui &other);
   bool           operator == (const RunLoopGui &other);
   bool           operator != (const RunLoopGui &other);

}; // class RunLoopGui



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/RunLoopGui.hpp"



#endif   // ohm_task_RunLoopGui_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
