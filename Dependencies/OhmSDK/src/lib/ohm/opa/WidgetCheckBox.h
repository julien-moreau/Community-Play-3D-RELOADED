/*****************************************************************************

        WidgetCheckBox.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
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



#if ! defined (ohm_opa_WidgetCheckBox_HEADER_INCLUDED)
#define  ohm_opa_WidgetCheckBox_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/prim/PrimCheckBox.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class WidgetCheckBox;
typedef mem::SharedPtr <WidgetCheckBox>   WidgetCheckBoxSPtr;

class WidgetCheckBox
:  public WidgetBase
,  public WidgetCreationHelper <WidgetCheckBox>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum State
   {
                  State_UNCHECKED = 0,
                  State_CHECKED,
                  State_MIXED,
   };
   
   class Event
   {
   public:
                  Event (WidgetCheckBox & widget) : _widget (widget) {}
                  
      WidgetCheckBox &
                  _widget;
      State       _cur_state;
      State       _suggested_state;
   };
   
   virtual        ~WidgetCheckBox ();

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           handle_event_default (Event & event);
   
   void           set_state (State state);
   State          get_state () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetCheckBox ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetCheckBox);
   
   // opal::FrameSignal delegates
   void           req_gesture_entered (opal::FrameSignal::Event & event);
   void           req_gesture_exited (opal::FrameSignal::Event & event);
   void           req_gesture_signal (opal::FrameSignal::Event & event);

   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimCheckBox>
                  _prim_aptr;
   
   State          _state;
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetCheckBox (const WidgetCheckBox &other);
   WidgetCheckBox &  operator = (const WidgetCheckBox &other);
   bool           operator == (const WidgetCheckBox &other);
   bool           operator != (const WidgetCheckBox &other);

}; // class WidgetCheckBox



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetCheckBox.hpp"



#endif   // ohm_opa_WidgetCheckBox_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
