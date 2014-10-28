/*****************************************************************************

        WidgetMusicTime.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66675

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



#if ! defined (ohm_opa_WidgetMusicTime_HEADER_INCLUDED)
#define  ohm_opa_WidgetMusicTime_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/prim/PrimMusicTime.h"
#include "ohm/opa/frame/FrameMusicTime.h"
#include "ohm/opal/base/FrameTracker.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opa/WidgetTextField.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class WidgetMusicTime;
typedef mem::SharedPtr <WidgetMusicTime>  WidgetMusicTimeSPtr;

class WidgetMusicTime
:  public WidgetBase
,  public WidgetCreationHelper <WidgetMusicTime>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      enum Type
      {
                  Type_ABSOLUTE = 0,
                  Type_RELATIVE_START,
                  Type_RELATIVE_TRACK,
                  Type_RELATIVE_COMMIT,
                  Type_RELATIVE_REVERT,
      };
      
                  Event (WidgetMusicTime & widget) : _widget (widget) {}
      
      WidgetMusicTime &
                  _widget;
      Type        _type;
      bool        _ok_flag;   // Type_RELATIVE_START
      long        _bar;
      int         _beat;
      int         _rem;
      archi::Float64
                  _tick;
   };
   
   virtual        ~WidgetMusicTime ();
   
   void           enable_start_from_1 ();
   void           set_time (int bar, int beat, int rem, archi::Float64 tick);
   long           get_time_bar () const;
   int            get_time_beat () const;
   int            get_time_rem () const;
   archi::Float64 get_time_tick () const;
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   // from frames
   void           req_edit (int field);
   bool           req_start ();
   void           req_change_value (int field, float y_delta);
   void           req_commit ();
   void           req_revert ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetMusicTime ();

   // WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetMusicTime);
   
   enum Field
   {
                  Field_BAR = 0,
                  Field_BEAT,
                  Field_REM,
                  Field_TICK,
                  
                  Field_NBR_ELT
   };
   
   void           handle_text_field_event (opa::WidgetTextField::Event & event);
   void           to_music_time (long & bar, int & beat, int & rem, archi::Float64 & tick, const std::string & str);

   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opal::AutoFramePtr <FrameMusicTime>
                  _frame_field_aptr_arr [Field_NBR_ELT];
   opak::AutoPrimPtr <PrimMusicTime>
                  _prim_aptr;
   
   util::Delegate _delegate;
   long           _bar;
   int            _beat;
   int            _rem;
   archi::Float64 _tick;
   bool           _start_from_1_flag;

   opa::WidgetLabelSPtr
                  _widget_field_sptr_arr [Field_NBR_ELT];

   opa::WidgetTextFieldSPtr
                  _widget_text_field_sptr;
   bool           _editing_flag;

   Event          _event;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetMusicTime (const WidgetMusicTime &other);
   WidgetMusicTime & operator = (const WidgetMusicTime &other);
   bool           operator == (const WidgetMusicTime &other);
   bool           operator != (const WidgetMusicTime &other);

}; // class WidgetMusicTime



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetMusicTime.hpp"



#endif   // ohm_opa_WidgetMusicTime_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
