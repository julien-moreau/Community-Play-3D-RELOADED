/*****************************************************************************

        WidgetPushButtonTiny.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61117

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



#if ! defined (ohm_opa_WidgetPushButtonTiny_HEADER_INCLUDED)
#define  ohm_opa_WidgetPushButtonTiny_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimPushButtonTiny.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opal/base/FrameKey.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opa
{



class WidgetPushButtonTiny;
typedef mem::SharedPtr <WidgetPushButtonTiny>   WidgetPushButtonTinySPtr;

class WidgetPushButtonTiny
:  public WidgetBase
,  public WidgetCreationHelper <WidgetPushButtonTiny>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (WidgetPushButtonTiny & widget, opal::Event & event) : _widget (widget), _event (event) {}
                  
      WidgetPushButtonTiny &
                  _widget;
      opal::Event &
                  _event;
   };
   
   virtual        ~WidgetPushButtonTiny ();
   
   void           set_type_cancel_sign ();
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           add_key_bind (const opal::Key & key);
   void           clear_key_binds ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetPushButtonTiny ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetPushButtonTiny);

   // opal::FrameSignal delegates
   void           req_rollover_begin (opal::FrameSignal::Event & event);
   void           req_rollover_end (opal::FrameSignal::Event & event);
   void           req_gesture_entered (opal::FrameSignal::Event & event);
   void           req_gesture_exited (opal::FrameSignal::Event & event);
   void           req_gesture_signal (opal::FrameSignal::Event & event);
   
   // opal::FrameKey delegates
   void           req_key (opal::FrameKey::Event & event);

   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_aptr;
   opal::AutoFramePtr <opal::FrameKey>
                  _frame_key_aptr;
   opak::AutoPrimPtr <PrimPushButtonTiny>
                  _prim_aptr;
   
   float          _width;
   
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetPushButtonTiny (const WidgetPushButtonTiny &other);
   WidgetPushButtonTiny &  operator = (const WidgetPushButtonTiny &other);
   bool           operator == (const WidgetPushButtonTiny &other);
   bool           operator != (const WidgetPushButtonTiny &other);

}; // class WidgetPushButtonTiny



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetPushButtonTiny.hpp"



#endif   // ohm_opa_WidgetPushButtonTiny_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
