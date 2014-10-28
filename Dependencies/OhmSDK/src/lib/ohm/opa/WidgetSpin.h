/*****************************************************************************

        WidgetSpin.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 53326

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



#if ! defined (ohm_opa_WidgetSpin_HEADER_INCLUDED)
#define  ohm_opa_WidgetSpin_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/prim/PrimSpin.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class FrameSpin;

class WidgetSpin;
typedef mem::SharedPtr <WidgetSpin> WidgetSpinSPtr;

class WidgetSpin
:  public WidgetBase
,  public WidgetCreationHelper <WidgetSpin>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (WidgetSpin & widget) : _widget (widget) {}

      WidgetSpin &
                  _widget;
      int         _delta;
   };
   
   virtual        ~WidgetSpin ();

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetSpin ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetSpin);

   // opal::FrameSignal delegates
   void           req_rollover_begin (opal::FrameSignal::Event & event);
   void           req_rollover_end (opal::FrameSignal::Event & event);
   void           req_gesture_entered (opal::FrameSignal::Event & event);
   void           req_gesture_exited (opal::FrameSignal::Event & event);
   void           req_gesture_signal (opal::FrameSignal::Event & event);
   void           req_gesture_timer (opal::FrameSignal::Event & event);
   
   void           signal (int abs_delta, opal::FrameSignal::Event & event);
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_up_aptr;
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_down_aptr;

   opak::AutoPrimPtr <PrimSpin>
                  _prim_aptr;
   
   util::Delegate _delegate;
   int            _abs_val;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetSpin (const WidgetSpin &other);
   WidgetSpin &   operator = (const WidgetSpin &other);
   bool           operator == (const WidgetSpin &other) const;
   bool           operator != (const WidgetSpin &other) const;

}; // class WidgetSpin



}  // namespace opa
}  // namespace ohm



#include "ohm/opa/WidgetSpin.hpp"



#endif   // ohm_opa_WidgetSpin_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

