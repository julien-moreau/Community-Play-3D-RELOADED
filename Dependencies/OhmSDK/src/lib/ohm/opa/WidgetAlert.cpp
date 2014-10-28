/*****************************************************************************

        WidgetAlert.cpp
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

#include "ohm/opa/BundleView.h"
#include "ohm/opa/WidgetAlert.h"
#include "ohm/opa/WidgetPushButton.h"
#include "ohm/opa/private/FrameDialogBack.h"
#include "ohm/opa/WidgetStack.h"
#include "ohm/opa/WidgetSpacer.h"
#include "ohm/opa/WidgetLabel.h"

#include "ohm/opal/Frame.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/Sizable.h"

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

WidgetAlert::~WidgetAlert ()
{
}



/*
==============================================================================
Name : create
==============================================================================
*/

WidgetAlertSPtr WidgetAlert::create (opa::BundleView & view)
{
   assert (&view != 0);
   
   WidgetAlertSPtr ret_val_sptr = WidgetAlertSPtr (new WidgetAlert (view));
   
   return ret_val_sptr;
}



/*
==============================================================================
Name : set_title
==============================================================================
*/

void  WidgetAlert::set_title (const std::string & title)
{
   _widget_dialog_sptr->set_title (title);
}



/*
==============================================================================
Name : set_error
==============================================================================
*/

void  WidgetAlert::set_error (const std::string & error)
{
   _widget_error_sptr->set_label (error);
}



/*
==============================================================================
Name : set_explanation
==============================================================================
*/

void  WidgetAlert::set_explanation (const std::string & explanation)
{
   _widget_explanation_sptr->set_label (explanation);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetAlert::WidgetAlert (opa::BundleView & view)
{
   _widget_dialog_sptr = WidgetDialog::create (view);
   
   fill_content ();
   fill_footer ();
}



/*
==============================================================================
Name : handle_push_button_event
==============================================================================
*/

void  WidgetAlert::handle_push_button_event (WidgetPushButton::Event & event)
{
   bool sne_flag = _widget_sne_sptr->get_state () == WidgetCheckBox::State_CHECKED;
   
   Event widget_event (*this);
   widget_event._type = Event::Type_OK;
   widget_event._show_next_time_flag = sne_flag;
   
   _widget_dialog_sptr->close ();
   
   _delegate.param_ref_signal (widget_event);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : fill_content
==============================================================================
*/

void  WidgetAlert::fill_content ()
{
   WidgetStackSPtr widget_h_stack_sptr = _widget_dialog_sptr->set_content <WidgetStack> ();
   widget_h_stack_sptr->set_orientation (Orientation_VERTICAL);
   
   {
      _widget_error_sptr = widget_h_stack_sptr->push <WidgetLabel> ();
   }

   {
      _widget_explanation_sptr = widget_h_stack_sptr->push <WidgetLabel> ();
      //_widget_explanation_sptr->set_type (WidgetLabel::Type_SMALL);
   }
}



/*
==============================================================================
Name : fill_footer
==============================================================================
*/

void  WidgetAlert::fill_footer ()
{
   WidgetStackSPtr widget_v_stack_sptr = _widget_dialog_sptr->set_footer <WidgetStack> ();
   widget_v_stack_sptr->set_orientation (Orientation_VERTICAL);
   widget_v_stack_sptr->set_width_auto_parent ();
   widget_v_stack_sptr->set_height_auto_parent ();
   
   {
      WidgetSpacerSPtr widget_spacer_sptr
            = widget_v_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_vertical_weight (1.f);
   }
   
   WidgetStackSPtr widget_h_stack_sptr = widget_v_stack_sptr->push <WidgetStack> ();
   widget_h_stack_sptr->set_orientation (Orientation_HORIZONTAL);
   widget_h_stack_sptr->set_width_auto_parent ();
   
   {
      WidgetSpacerSPtr widget_spacer_sptr
            = widget_v_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_vertical_weight (1.f);
   }

   {
      _widget_sne_sptr = widget_h_stack_sptr->push <WidgetCheckBox> ();
      
      _widget_sne_sptr->set_state (WidgetCheckBox::State_CHECKED);
   }
   
   {
      WidgetLabelSPtr widget_label_sptr
         = widget_h_stack_sptr->push <WidgetLabel> ();
      
      //widget_label_sptr->set_type (WidgetLabel::Type_CARVED);
      widget_label_sptr->set_width_auto_content ();
      widget_label_sptr->set_label ("Show Next Time");
   }
   
   {
      WidgetSpacerSPtr widget_spacer_sptr
            = widget_h_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_horizontal_weight (1.f);
   }
   
   {
      WidgetPushButtonSPtr widget_push_button_sptr
         = widget_h_stack_sptr->push <WidgetPushButton> ();
      
      widget_push_button_sptr->set_width (75.f);
      widget_push_button_sptr->set_label ("OK");
      widget_push_button_sptr->add_key_bind (
         opal::Key (opal::KeyCode_RETURN)
      );
/*    widget_push_button_sptr->add_key_bind (
         opal::Key (opal::Key::KeyCode_NUMERIC_KEYPAD_ENTER)
      );
*/    
      widget_push_button_sptr->bind <
         WidgetAlert,
         &WidgetAlert::handle_push_button_event
      > (*this);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
