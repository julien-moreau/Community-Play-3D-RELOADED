/*****************************************************************************

        WidgetMusicTime.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67157

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

#include "ohm/opa/WidgetMusicTime.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opa/prim/PrimCheckBox.h"

#include "ohm/opal/base/FrameSignal.h"
#include "ohm/txt/out/Print.h"
#include "ohm/txt/ascii/ascii_fnc.h"

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

WidgetMusicTime::~WidgetMusicTime ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name: enable_start_from_1
==============================================================================
*/

void  WidgetMusicTime::enable_start_from_1 ()
{
   _start_from_1_flag = true;
}



/*
==============================================================================
Name: set_time
==============================================================================
*/

void  WidgetMusicTime::set_time (int bar, int beat, int rem, archi::Float64 tick)
{
   _bar = bar;
   _beat = beat;
   _rem = rem;
   _tick = tick;
   
   txt::out::Print printer;
   
   if (_start_from_1_flag)
   {
      bar += 1;
      beat += 1;
      rem += 1;
      tick += 1;
   }
   
   printer.print ("%1d%").arg (bar);
   _widget_field_sptr_arr [Field_BAR]->set_label (printer);

   printer.print ("%1d%").arg (beat);
   _widget_field_sptr_arr [Field_BEAT]->set_label (printer);

   printer.print ("%1d%").arg (rem);
   _widget_field_sptr_arr [Field_REM]->set_label (printer);
   
   bool tick_int_flag = lang::abs (math::round_int (tick) - tick) < 1e-8f;
   
   printer.print ("%1d%%2%").arg (math::round_int (tick)).arg (tick_int_flag ? "" : "*");
   _widget_field_sptr_arr [Field_TICK]->set_label (printer);
}



/*
==============================================================================
Name: get_time_bar
==============================================================================
*/

long  WidgetMusicTime::get_time_bar () const
{
   return _bar;
}



/*
==============================================================================
Name: get_time_beat
==============================================================================
*/

int   WidgetMusicTime::get_time_beat () const
{
   return _beat;
}



/*
==============================================================================
Name: get_time_rem
==============================================================================
*/

int   WidgetMusicTime::get_time_rem () const
{
   return _rem;
}



/*
==============================================================================
Name: get_time_tick
==============================================================================
*/

archi::Float64 WidgetMusicTime::get_time_tick () const
{
   return _tick;
}



/*
==============================================================================
Name: req_edit
==============================================================================
*/

void  WidgetMusicTime::req_edit (int field)
{
   if (!_editing_flag)
   {
      long bar = _bar;
      int beat = _beat;
      int rem = _rem;
      archi::Float64 tick = _tick;
      
      if (_start_from_1_flag)
      {
         bar += 1;
         beat += 1;
         rem += 1;
         tick += 1;
      }
      
      txt::out::Print printer;
      printer.print ("%1% %2% %3% %4%").arg (bar).arg (beat).arg (rem).arg (tick);
      
      _widget_text_field_sptr->set_text (printer);
      _widget_text_field_sptr->start_edit ();
      _widget_text_field_sptr->show ();

      _editing_flag = true;
   }
}



/*
==============================================================================
Name: req_start
==============================================================================
*/

bool  WidgetMusicTime::req_start ()
{
   _event._type = Event::Type_RELATIVE_START;
   _event._ok_flag = false;
   _event._bar = 0;
   _event._beat = 0;
   _event._rem = 0;
   _event._tick = 0;
   
   _delegate.param_ref_signal (_event);
   
   return _event._ok_flag;
}



/*
==============================================================================
Name: req_change_value
==============================================================================
*/

void  WidgetMusicTime::req_change_value (int field, float y_delta)
{
   _event._type = Event::Type_RELATIVE_TRACK;
   
   if (field < Field_TICK)
   {
      y_delta *= .1f;
   }
   else
   {
      y_delta *= 2.f;
   }
   
   switch (field)
   {
   case Field_BAR:
      _event._bar = math::round_int (y_delta);
      break;

   case Field_BEAT:
      _event._beat = math::round_int (y_delta);
      break;

   case Field_REM:
      _event._rem = math::round_int (y_delta);
      break;

   case Field_TICK:
      _event._tick = y_delta;
      break;
   }
   
   _delegate.param_ref_signal (_event);
}



/*
==============================================================================
Name: req_commit
==============================================================================
*/

void  WidgetMusicTime::req_commit ()
{
   _event._type = Event::Type_RELATIVE_COMMIT;
   
   _delegate.param_ref_signal (_event);
}



/*
==============================================================================
Name: req_revert
==============================================================================
*/

void  WidgetMusicTime::req_revert ()
{
   _event._type = Event::Type_RELATIVE_REVERT;
   _event._bar = 0;
   _event._beat = 0;
   _event._rem = 0;
   _event._tick = 0;
   
   _delegate.param_ref_signal (_event);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMusicTime::WidgetMusicTime ()
:  _frame_aptr ()
,  _prim_aptr ()
,  _bar (0)
,  _beat (0)
,  _rem (0)
,  _tick (0)
,  _start_from_1_flag (false)
,  _editing_flag (false)
,  _event (*this)
{
   _frame_aptr = new opal::Frame;
   
   for (int i = 0 ; i < Field_NBR_ELT ; ++i)
   {
      _frame_field_aptr_arr [i] = new FrameMusicTime (*this, i);
      _frame_aptr->insert_top (*_frame_field_aptr_arr [i]);
   }

   _prim_aptr = new PrimMusicTime;
   _prim_aptr->set_name ("PrimMusicTime");

   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);

   set_nbr_slot (1);
   set_slot (0, *_frame_aptr);
   set_slot (0, *_prim_aptr);
   
   for (int i = 0 ; i < Field_NBR_ELT ; ++i)
   {
      opa::WidgetLabelSPtr & widget_field_sptr = _widget_field_sptr_arr [i];
      
      widget_field_sptr = WidgetLabel::create ();
      widget_field_sptr->set_flush_factor (.5f);
      embed (*widget_field_sptr);
   }
   
   _widget_text_field_sptr = opa::WidgetTextField::create ();
   
   embed (*_widget_text_field_sptr);
   
   _widget_text_field_sptr->bind <
      WidgetMusicTime,
      &WidgetMusicTime::handle_text_field_event
   > (*this);
   
   _widget_text_field_sptr->hide ();

}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetMusicTime::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = opa::Rect (0, -14, 180, 14 + 7);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetMusicTime::do_get_margin (Margin & margin)
{
   margin = opa::Margin (4, 4, 4, 6);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetMusicTime::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetMusicTime::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   
   _prim_aptr->_size.stop ().push (bounds, tt, tf);
   
   opa::Rect rect;
   rect._size = bounds;
   rect._size._width *= .25f;
   
   rect._origin._y = 1;
   
   for (int i = 0 ; i < Field_NBR_ELT ; ++i)
   {
      _frame_field_aptr_arr [i]->set_origin (rect._origin);
      _frame_field_aptr_arr [i]->set_bounds (rect._size);
      
      _widget_field_sptr_arr [i]->set_rect (rect, tt, tf);
      rect._origin._x += bounds._width * .25f;
   }
   
   _widget_text_field_sptr->set_rect (opa::Rect (opa::Point::_zero, bounds), tt, tf);
   _widget_text_field_sptr->set_width (bounds._width);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetMusicTime::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetMusicTime::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetMusicTime::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: handle_text_field_event
==============================================================================
*/

void  WidgetMusicTime::handle_text_field_event (opa::WidgetTextField::Event & event)
{
   assert (&event != 0);
   
   if (_editing_flag)
   {
      if (event._type == opa::WidgetTextField::Event::Type_COMMIT)
      {
         std::string str = event._suggested_str;
         
         _event._type = Event::Type_ABSOLUTE;
         _event._ok_flag = true;
         
         to_music_time (_event._bar, _event._beat, _event._rem, _event._tick, event._suggested_str);
         
         _delegate.param_ref_signal (_event);
         
         _editing_flag = false;
         _widget_text_field_sptr->hide ();
      } 
      else if (event._type  == opa::WidgetTextField::Event::Type_REVERT)
      {
         _editing_flag = false;
         _widget_text_field_sptr->hide ();
      }
   }
}



/*
==============================================================================
Name: to_music_time
==============================================================================
*/

void  WidgetMusicTime::to_music_time (long & bar, int & beat, int & rem, archi::Float64 & tick, const std::string & str)
{
   std::string str_arr [4];
   
   size_t pos = 0;
   
   for (int i = 0 ; i < 4 ; ++i)
   {
      size_t pos2 = str.find_first_of (" \t\r\n", pos);
      
      if (pos2 != std::string::npos)
      {
         str_arr [i] = str.substr (pos, pos2 - pos);
         
         pos = str.find_first_not_of (" \t\r\n", pos2);
         
         if (pos == std::string::npos)
         {
            break;
         }
      }
      else
      {
         str_arr [i] = str.substr (pos);
         
         break;
      }
   }
   
   bar = 0;
   beat = 0;
   rem = 0;
   tick = 0.0;
   
   if (_start_from_1_flag)
   {
      bar += 1;
      beat += 1;
      rem += 1;
      tick += 1.0;
   }
   
   bool ok_flag = true;
   
   for (int i = 0 ; i < 3 ; ++i)
   {
      archi::Int64 val = 0;
      
      if (!str_arr [i].empty ())
      {
         ok_flag = txt::ascii::conv_str_to_int64 (val, str_arr [i].c_str ());
         
         if (ok_flag)
         {
            switch (i)
            {
            case 0:
               bar = long (val);
               break;

            case 1:
               beat = int (val);
               break;

            case 2:
               rem = int (val);
               break;
            }
         }
         else
         {
            break;
         }
      }
   }
   
   if (ok_flag)
   {
      if (!str_arr [3].empty ())
      {
         double val = 0.0;
         
         ok_flag = txt::ascii::conv_str_to_double (val, str_arr [3].c_str ());
         
         if (ok_flag)
         {
            tick = val;
         }
      }
   }
   
   if (_start_from_1_flag)
   {
      bar -= 1;
      beat -= 1;
      rem -= 1;
      tick -= 1.0;
   }
   
   lang::max_in_place (bar, long (0));
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
