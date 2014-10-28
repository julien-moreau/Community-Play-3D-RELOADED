/*****************************************************************************

        WidgetLabelLink.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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



#if ! defined (ohm_opa_WidgetLabelLink_HEADER_INCLUDED)
#define  ohm_opa_WidgetLabelLink_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/prim/PrimLabel.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opak/AutoPrimPtr.h"



namespace ohm
{
namespace opal
{
class Typesetter;
}



namespace opa
{


class WidgetLabelLink;
typedef mem::SharedPtr <WidgetLabelLink>  WidgetLabelLinkSPtr;

class WidgetLabelLink
:  public WidgetBase
,  public WidgetCreationHelper <WidgetLabelLink>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      enum Type
      {
                  Type_SIGNAL = 0,
      };
      
                  Event (WidgetLabelLink & widget, Type type) : _widget (widget), _type (type) {}
      
      WidgetLabelLink &
                  _widget;
      Type        _type;
   };
   
   virtual        ~WidgetLabelLink ();
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();
   bool           is_width_auto () const;
   
   void           set_label (const std::string & label);
   const std::string &
                  get_label () const;

   void           set_flush_factor (float flush_factor);
   float          get_flush_factor () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetLabelLink ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetLabelLink);
   
   void           update_metrics ();

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           handle_rollover_begin (opal::FrameSignal::Event & event);
   void           handle_rollover_end (opal::FrameSignal::Event & event);
#endif
   void           handle_gesture_signal (opal::FrameSignal::Event & event);
   
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimLabel>
                  _prim_aptr;
   float          _ascent;
   float          _descent;

   WidgetMetricAuto
                  _width;
   std::string    _label;
   float          _flush_factor;
   opal::Scripture
                  _scripture;
   
   util::Delegate _delegate;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetLabelLink (const WidgetLabelLink &other);
   WidgetLabelLink & operator = (const WidgetLabelLink &other);
   bool           operator == (const WidgetLabelLink &other);
   bool           operator != (const WidgetLabelLink &other);

}; // class WidgetLabelLink



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetLabelLink.hpp"



#endif   // ohm_opa_WidgetLabelLink_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
