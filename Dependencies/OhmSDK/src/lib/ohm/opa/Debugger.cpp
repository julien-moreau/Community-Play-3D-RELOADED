/*****************************************************************************

        Debugger.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66419

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Debugger.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Debugger::~Debugger ()
{
}



/*
==============================================================================
Name : enable_frame_break
==============================================================================
*/

void  Debugger::enable_frame_break (bool enable_flag)
{
   use_instance ().do_enable_frame_break (enable_flag);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - remove_from_parents
==============================================================================
*/

void  Debugger::break_on (opal::Frame & frame, void (opal::Frame::*method) ())
{
   use_instance ().do_break_on (frame, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - insert_top
   - insert_bottom
   - remove
==============================================================================
*/

void  Debugger::break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Frame & frame))
{
   use_instance ().do_break_on (frame, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - propagate_children
   - handle_event
==============================================================================
*/

void  Debugger::break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Event & event))
{
   use_instance ().do_break_on (frame, method);
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opal::Frame & frame, void (opal::Frame::*method) ()) const
{
   if (_breaking_frame_flag)
   {
      const FramePtrMethodVoidPairList::const_iterator it_end
         = _frame_ptr_method_void_pair_list.end ();
      
      const FramePtrMethodVoidPairList::const_iterator it = std::find (
         _frame_ptr_method_void_pair_list.begin (),
         it_end,
         std::make_pair (&frame, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opal::Frame & frame, void (opal::Frame::*method) (opal::Frame &)) const
{
   if (_breaking_frame_flag)
   {
      const FramePtrMethodFramePairList::const_iterator it_end
         = _frame_ptr_method_frame_pair_list.end ();
      
      const FramePtrMethodFramePairList::const_iterator it = std::find (
         _frame_ptr_method_frame_pair_list.begin (),
         it_end,
         std::make_pair (&frame, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opal::Frame & frame, void (opal::Frame::*method) (opal::Event &)) const
{
   if (_breaking_frame_flag)
   {
      const FramePtrMethodEventPairList::const_iterator it_end
         = _frame_ptr_method_event_pair_list.end ();
      
      const FramePtrMethodEventPairList::const_iterator it = std::find (
         _frame_ptr_method_event_pair_list.begin (),
         it_end,
         std::make_pair (&frame, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : enable_events_trace
==============================================================================
*/

void  Debugger::enable_events_trace (bool enable_flag)
{
   use_instance ().do_enable_events_trace (enable_flag);
}



/*
==============================================================================
Name : is_tracing_events
==============================================================================
*/

bool  Debugger::is_tracing_events () const
{
   return _tracing_events_flag;
}



/*
==============================================================================
Name : enable_frame_trace
==============================================================================
*/

void  Debugger::enable_frame_trace (bool enable_flag)
{
   use_instance ().do_enable_frame_trace (enable_flag);
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  Debugger::trace (opal::Event::Type type, bool enable_flag)
{
   use_instance ().do_trace (type, enable_flag);
}



/*
==============================================================================
Name : is_tracing
==============================================================================
*/

bool  Debugger::is_tracing (opal::Event::Type type) const
{
   if (_tracing_frame_flag)
   {
      return _tracing_event_type_arr [type];
   }
   
   return false;
}



/*
==============================================================================
Name : enable_prim_break
==============================================================================
*/

void  Debugger::enable_prim_break (bool enable_flag)
{
   use_instance ().do_enable_prim_break (enable_flag);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - do_push_back
==============================================================================
*/

void  Debugger::break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *))
{
   use_instance ().do_break_on (primitive, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - do_remove
==============================================================================
*/

void  Debugger::break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *, bool))
{
   use_instance ().do_break_on (primitive, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - display_children
==============================================================================
*/

void  Debugger::break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::GraphicContext &))
{
   use_instance ().do_break_on (primitive, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - do_destroy
==============================================================================
*/

void  Debugger::break_on (opak::Primitive & primitive, void (opak::Primitive::*method) ())
{
   use_instance ().do_break_on (primitive, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - do_remove_from_parents
==============================================================================
*/

void  Debugger::break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (bool))
{
   use_instance ().do_break_on (primitive, method);
}



/*
==============================================================================
Name : break_on
Note :
   supports only :
   - do_attach
   - do_detach
==============================================================================
*/

void  Debugger::break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (opak::Primitive &))
{
   use_instance ().do_break_on (primitive, method);
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *)) const
{
   if (_breaking_prim_flag)
   {
      const PrimPtrContainerMethodPrimPairList::const_iterator it_end
         = _prim_ptr_container_method_prim_pair_list.end ();
      
      const PrimPtrContainerMethodPrimPairList::const_iterator it = std::find (
         _prim_ptr_container_method_prim_pair_list.begin (),
         it_end,
         std::make_pair (&primitive, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *, bool)) const
{
   if (_breaking_prim_flag)
   {
      const PrimPtrContainerMethodPrimBoolPairList::const_iterator it_end
         = _prim_ptr_container_method_prim_bool_pair_list.end ();
      
      const PrimPtrContainerMethodPrimBoolPairList::const_iterator it = std::find (
         _prim_ptr_container_method_prim_bool_pair_list.begin (),
         it_end,
         std::make_pair (&primitive, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::GraphicContext &)) const
{
   if (_breaking_prim_flag)
   {
      const PrimPtrContainerMethodGraphicContextPairList::const_iterator it_end
         = _prim_ptr_container_method_graphic_context_pair_list.end ();
      
      const PrimPtrContainerMethodGraphicContextPairList::const_iterator it = std::find (
         _prim_ptr_container_method_graphic_context_pair_list.begin (),
         it_end,
         std::make_pair (&primitive, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opak::Primitive & primitive, void (opak::Primitive::*method) ()) const
{
   if (_breaking_prim_flag)
   {
      const PrimPtrMethodVoidPairList::const_iterator it_end
         = _prim_ptr_method_void_pair_list.end ();
      
      const PrimPtrMethodVoidPairList::const_iterator it = std::find (
         _prim_ptr_method_void_pair_list.begin (),
         it_end,
         std::make_pair (&primitive, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opak::Primitive & primitive, void (opak::Primitive::*method) (bool)) const
{
   if (_breaking_prim_flag)
   {
      const PrimPtrMethodBoolPairList::const_iterator it_end
         = _prim_ptr_method_bool_pair_list.end ();
      
      const PrimPtrMethodBoolPairList::const_iterator it = std::find (
         _prim_ptr_method_bool_pair_list.begin (),
         it_end,
         std::make_pair (&primitive, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : has_break
==============================================================================
*/

bool  Debugger::has_break (opak::Primitive & primitive, void (opak::Primitive::*method) (opak::Primitive &)) const
{
   if (_breaking_prim_flag)
   {
      const PrimPtrMethodPrimPairList::const_iterator it_end
         = _prim_ptr_method_prim_pair_list.end ();
      
      const PrimPtrMethodPrimPairList::const_iterator it = std::find (
         _prim_ptr_method_prim_pair_list.begin (),
         it_end,
         std::make_pair (&primitive, method)
      );
      
      return (it != it_end);
   }
   
   return false;
}



/*
==============================================================================
Name : enable_prim_trace
==============================================================================
*/

void  Debugger::enable_prim_trace (bool enable_flag)
{
   use_instance ().do_enable_prim_trace (enable_flag);
}



/*
==============================================================================
Name : trace_prim_display
==============================================================================
*/

void  Debugger::trace_prim_display (bool enable_flag)
{
   use_instance ().do_trace_prim_display (enable_flag);
}



/*
==============================================================================
Name : is_tracing_prim_display
==============================================================================
*/

bool  Debugger::is_tracing_prim_display () const
{
   if (_tracing_prim_flag)
   {
      return _tracing_prim_display_flag;
   }
   
   return false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Debugger::Debugger ()
:  _breaking_frame_flag (false)
,  _frame_ptr_method_void_pair_list ()
,  _frame_ptr_method_frame_pair_list ()
,  _frame_ptr_method_event_pair_list ()

,  _tracing_events_flag (false)
,  _tracing_frame_flag (true)
/*,   _tracing_event_type_arr ()*/

,  _breaking_prim_flag (false)
,  _prim_ptr_container_method_prim_pair_list ()
,  _prim_ptr_container_method_prim_bool_pair_list ()
,  _prim_ptr_container_method_graphic_context_pair_list ()
,  _prim_ptr_method_void_pair_list ()
,  _prim_ptr_method_bool_pair_list ()
,  _prim_ptr_method_prim_pair_list ()

,  _tracing_prim_flag (true)
,  _tracing_prim_display_flag (false)
{
   for (int i = 0 ; i < opal::Event::Type_NBR_ELT ; ++i)
   {
      _tracing_event_type_arr [i] = false;
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_enable_frame_break
==============================================================================
*/

void  Debugger::do_enable_frame_break (bool enable_flag)
{
   _breaking_frame_flag = enable_flag;
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opal::Frame & frame, void (opal::Frame::*method) ())
{
   _frame_ptr_method_void_pair_list.push_back (std::make_pair (&frame, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Frame & frame))
{
   _frame_ptr_method_frame_pair_list.push_back (std::make_pair (&frame, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Event & event))
{
   _frame_ptr_method_event_pair_list.push_back (std::make_pair (&frame, method));
}



/*
==============================================================================
Name : do_enable_events_trace
==============================================================================
*/

void  Debugger::do_enable_events_trace (bool enable_flag)
{
   _tracing_events_flag = enable_flag;
}



/*
==============================================================================
Name : do_enable_trace
==============================================================================
*/

void  Debugger::do_enable_frame_trace (bool enable_flag)
{
   _tracing_frame_flag = enable_flag;
}



/*
==============================================================================
Name : do_trace
==============================================================================
*/

void  Debugger::do_trace (opal::Event::Type type, bool enable_flag)
{
   assert (type >= 0);
   assert (type < opal::Event::Type_NBR_ELT);
   
   _tracing_event_type_arr [type] = enable_flag;
}



/*
==============================================================================
Name : do_enable_prim_break
==============================================================================
*/

void  Debugger::do_enable_prim_break (bool enable_flag)
{
   _breaking_prim_flag = enable_flag;
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *))
{
   _prim_ptr_container_method_prim_pair_list.push_back (std::make_pair (&primitive, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *, bool))
{
   _prim_ptr_container_method_prim_bool_pair_list.push_back (std::make_pair (&primitive, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::GraphicContext &))
{
   _prim_ptr_container_method_graphic_context_pair_list.push_back (std::make_pair (&primitive, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opak::Primitive & primitive, void (opak::Primitive::*method) ())
{
   _prim_ptr_method_void_pair_list.push_back (std::make_pair (&primitive, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (bool))
{
   _prim_ptr_method_bool_pair_list.push_back (std::make_pair (&primitive, method));
}



/*
==============================================================================
Name : do_break_on
==============================================================================
*/

void  Debugger::do_break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (opak::Primitive &))
{
   _prim_ptr_method_prim_pair_list.push_back (std::make_pair (&primitive, method));
}



/*
==============================================================================
Name : do_enable_prim_trace
==============================================================================
*/

void  Debugger::do_enable_prim_trace (bool enable_flag)
{
   _tracing_prim_flag = enable_flag;
}



/*
==============================================================================
Name : do_trace_prim_display
==============================================================================
*/

void  Debugger::do_trace_prim_display (bool enable_flag)
{
   _tracing_prim_display_flag = enable_flag;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
