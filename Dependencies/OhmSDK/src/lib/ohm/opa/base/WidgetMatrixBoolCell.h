/*****************************************************************************

        WidgetMatrixBoolCell.h
        Copyright (c) 2010 Ohm Force

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



#if ! defined (ohm_opa_WidgetMatrixBoolCell_HEADER_INCLUDED)
#define  ohm_opa_WidgetMatrixBoolCell_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/prim/PrimMatrixBoolCell.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class WidgetMatrixBoolCell;
typedef mem::SharedPtr <WidgetMatrixBoolCell>   WidgetMatrixBoolCellSPtr;

class WidgetMatrixBoolCell
:  public WidgetBase
,  public WidgetCreationHelper <WidgetMatrixBoolCell>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (WidgetMatrixBoolCell & widget) : _widget (widget) {}
                  
      WidgetMatrixBoolCell &
                  _widget;
      bool        _cur_state_flag;
      bool        _suggested_state_flag;
   };
   
   virtual        ~WidgetMatrixBoolCell ();

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           handle_event_default (Event & event);
   
   void           set_state (bool state_flag);
   bool           get_state () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetMatrixBoolCell ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetMatrixBoolCell);
   
   // opal::FrameSignal delegates
   void           req_gesture_entered (opal::FrameSignal::Event & event);
   void           req_gesture_exited (opal::FrameSignal::Event & event);
   void           req_gesture_signal (opal::FrameSignal::Event & event);

   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimMatrixBoolCell>
                  _prim_aptr;
   
   bool           _state_flag;
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetMatrixBoolCell (const WidgetMatrixBoolCell &other);
   WidgetMatrixBoolCell &  operator = (const WidgetMatrixBoolCell &other);
   bool           operator == (const WidgetMatrixBoolCell &other);
   bool           operator != (const WidgetMatrixBoolCell &other);

}; // class WidgetMatrixBoolCell



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetMatrixBoolCell.hpp"



#endif   // ohm_opa_WidgetMatrixBoolCell_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
