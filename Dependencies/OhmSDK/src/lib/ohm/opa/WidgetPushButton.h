/*****************************************************************************

        WidgetPushButton.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54712

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



#if ! defined (ohm_opa_WidgetPushButton_HEADER_INCLUDED)
#define  ohm_opa_WidgetPushButton_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimPushButton.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opa/base/WidgetPushButtonBase.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opa
{



class FrameToggle;
class WidgetLabel;

class WidgetPushButton;
typedef mem::SharedPtr <WidgetPushButton> WidgetPushButtonSPtr;

class WidgetPushButton
:  public WidgetBase
,  public WidgetCreationHelper <WidgetPushButton>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (WidgetPushButton & widget, opal::Event & event) : _widget (widget), _event (event) {}
                  
      WidgetPushButton &
                  _widget;
      opal::Event &
                  _event;
   };
   
   virtual        ~WidgetPushButton ();
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_label (const std::string & label);
   const std::string &
                  get_label () const;
   
   void           add_key_bind (const opal::Key & key);
   void           clear_key_binds ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetPushButton ();

   // WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   
   // delegates
   void           handle_base_event (WidgetPushButtonBase::Event & event);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetPushButton);

   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;

   WidgetPushButtonBaseSPtr
                  _widget_button_sptr;
   WidgetLabelSPtr
                  _widget_label_sptr;
   Point          _label_origin;
   
   util::Delegate _delegate;
   
   static float   _inner_hori_margin;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetPushButton (const WidgetPushButton &other);
   WidgetPushButton &   operator = (const WidgetPushButton &other);
   bool           operator == (const WidgetPushButton &other);
   bool           operator != (const WidgetPushButton &other);

}; // class WidgetPushButton



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetPushButton.hpp"



#endif   // ohm_opa_WidgetPushButton_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
