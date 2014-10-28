/*****************************************************************************

        RunLoopGuiImpl.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70970

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



#if ! defined (ohm_task_RunLoopGuiImpl_HEADER_INCLUDED)
#define  ohm_task_RunLoopGuiImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>



namespace ohm
{
namespace task
{



class RunLoopGui;
class AcceleratorInterface;

class RunLoopGuiImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (*CrumbProc) (const ::MSG &, void *);
   
                  RunLoopGuiImpl (RunLoopGui & parent);
   virtual        ~RunLoopGuiImpl ();
   
   void           bind (AcceleratorInterface & accelerator);
   void           unbind (AcceleratorInterface & accelerator);
   
   void           bind_crumb (CrumbProc crumb_proc, void * user_data);
   
   void           run ();
   void           stop ();
   void           signal ();
   double         get_time ();

   bool           is_processing_timer_message () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           handle_crumb_event (::LRESULT dispatch_ret, const ::MSG & msg);
   
   static ::LRESULT CALLBACK
                  win_proc_static (::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam);

   RunLoopGui &   _parent;
   const ::UINT   _custom_msg_index;
   
   bool           _stop_flag;
   AcceleratorInterface *
                  _accelerator_ptr;

   conc::AtomicInt <int>
                  _custom_msg_count;
   ::HWND         _hwnd;
   ::UINT         _current_processed_msg;    // WM_NULL if none
   
   CrumbProc      _crumb_proc;
   void *         _crumb_user_data;
   
   static const wchar_t
                  _window_class_name [];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RunLoopGuiImpl ();
                  RunLoopGuiImpl (const RunLoopGuiImpl &other);
   RunLoopGuiImpl &
                  operator = (const RunLoopGuiImpl &other);
   bool           operator == (const RunLoopGuiImpl &other);
   bool           operator != (const RunLoopGuiImpl &other);

}; // class RunLoopGuiImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/win32/RunLoopGuiImpl.hpp"



#endif   // ohm_task_RunLoopGuiImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
