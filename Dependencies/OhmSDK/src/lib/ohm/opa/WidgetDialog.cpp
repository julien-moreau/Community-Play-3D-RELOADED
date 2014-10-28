/*****************************************************************************

        WidgetDialog.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69886

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
#include "ohm/opa/EventBundleView.h"
#include "ohm/opa/WidgetDialog.h"
#include "ohm/opa/WidgetPushButton.h"
#include "ohm/opa/private/FrameDialogBack.h"
#include "ohm/opa/WidgetStack.h"
#include "ohm/opa/WidgetSpacer.h"
#include "ohm/opa/WidgetLabel.h"

#include "ohm/opal/Frame.h"
#include "ohm/opal/Screens.h"
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

WidgetDialog::~WidgetDialog ()
{
}



/*
==============================================================================
Name : create
==============================================================================
*/

WidgetDialogSPtr WidgetDialog::create (opa::BundleView & view)
{
   assert (&view != 0);
   
   WidgetDialogSPtr ret_val_sptr = WidgetDialogSPtr (new WidgetDialog (view));
   
   return ret_val_sptr;
}



/*
==============================================================================
Name : set_title
Description :
   Helper function that set a label in the header of the dialog
==============================================================================
*/

void  WidgetDialog::set_title (const std::string & title)
{
   WidgetStackSPtr widget_v_stack_sptr = set_header <WidgetStack> ();
   widget_v_stack_sptr->set_orientation (Orientation_VERTICAL);
   widget_v_stack_sptr->set_width_auto_parent ();
   widget_v_stack_sptr->set_height_auto_parent ();
   
   {
      WidgetSpacerSPtr widget_spacer_sptr
         = widget_v_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_vertical_weight (1.f);
   }
      
   WidgetLabelSPtr widget_label_sptr = widget_v_stack_sptr->push <WidgetLabel> ();
   widget_label_sptr->set_label (title);
   
   {
      WidgetSpacerSPtr widget_spacer_sptr
         = widget_v_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_vertical_weight (1.f);
   }
      
   invalidate_layout ();
}



/*
==============================================================================
Name : add_cancel_ok_buttons
Description :
   Helper function that set the default cancel and ok buttons in the
   dialog footer.
==============================================================================
*/

void  WidgetDialog::add_cancel_ok_buttons (const std::string & cancel_label, const std::string & ok_label, bool cancel_active_flag, bool ok_active_flag)
{
   WidgetStackSPtr widget_v_stack_sptr = set_footer <WidgetStack> ();
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
         = widget_h_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_horizontal_weight (1.f);
   }
      
   for (int i = Event::Type_CANCEL ; i <= Event::Type_OK ; ++i)
   {
      Event::Type type = static_cast <Event::Type> (i);
      
      WidgetPushButtonSPtr widget_push_button_sptr
         = widget_h_stack_sptr->push <WidgetPushButton> ();
      
      widget_push_button_sptr->set_width (75.f);
      
      switch (type)
      {
      case Event::Type_CANCEL:
         widget_push_button_sptr->set_label (cancel_label);
         widget_push_button_sptr->add_key_bind (
            opal::Key (opal::KeyCode_ESCAPE)
         );
         widget_push_button_sptr->enable (cancel_active_flag);
         _frame_key_aptr->add_key (
            opal::Key (opal::KeyCode_ESCAPE)
         );
         break;

      case Event::Type_OK:
         widget_push_button_sptr->set_label (ok_label);
         widget_push_button_sptr->add_key_bind (
            opal::Key (opal::KeyCode_RETURN)
         );
         widget_push_button_sptr->enable (ok_active_flag);
/*       widget_push_button_sptr->add_key_bind (
            opal::Key (opal::Key::KeyCode_NUMERIC_KEYPAD_ENTER)
         );
 */      _frame_key_aptr->add_key (
            opal::Key (opal::KeyCode_RETURN)
         );
/*       _frame_key_aptr->add_key (
            opal::Key (opal::KeyCode_NUMERIC_KEYPAD_ENTER)
         );
 */      break;
      
      default:
         assert (false);
         break;
      }
      
      widget_push_button_sptr->set_data (type);
      
      widget_push_button_sptr->bind <
         WidgetDialog,
         &WidgetDialog::handle_push_button_event
      > (*this);
   }     

   {
      WidgetSpacerSPtr widget_spacer_sptr
            = widget_v_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_vertical_weight (1.f);
   }
}



/*
==============================================================================
Name : add_ok_button
Description :
   Helper function that set one default ok button in the dialog footer.
==============================================================================
*/

void  WidgetDialog::add_ok_button (const std::string & ok_label, bool ok_active_flag)
{
   WidgetStackSPtr widget_v_stack_sptr = set_footer <WidgetStack> ();
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
         = widget_h_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_horizontal_weight (1.f);
   }
   
   
   {
      WidgetPushButtonSPtr widget_push_button_sptr
         = widget_h_stack_sptr->push <WidgetPushButton> ();
      
      widget_push_button_sptr->set_width (75.f);
      
      widget_push_button_sptr->set_label (ok_label);
      widget_push_button_sptr->add_key_bind (
         opal::Key (opal::KeyCode_RETURN)
      );
      widget_push_button_sptr->enable (ok_active_flag);
      _frame_key_aptr->add_key (
         opal::Key (opal::KeyCode_RETURN)
      );
      
      widget_push_button_sptr->set_data (Event::Type_OK);
      
      widget_push_button_sptr->bind <
         WidgetDialog,
         &WidgetDialog::handle_push_button_event
      > (*this);
   }     

   {
      WidgetSpacerSPtr widget_spacer_sptr
            = widget_v_stack_sptr->push <WidgetSpacer> ();
      widget_spacer_sptr->set_vertical_weight (1.f);
   }
}



/*
==============================================================================
Name : shake
==============================================================================
*/

void  WidgetDialog::shake ()
{
   float x = _layer_aptr->_origin._x;
   
   double dtime = .05f;
   float mag = 10.f;
   opak::TransitionFunction tf = opak::ease_in_ease_out;
   
   _layer_aptr->_origin._x
      .stop ()
      .push (x + mag, dtime, tf)
      .push (x - .9f * mag, dtime, tf)
      .push (x + .8f * mag, dtime, tf)
      .push (x - .6f * mag, dtime, tf)
      .push (x + .3f * mag, dtime, tf)
      .push (x, dtime, tf);
}



/*
==============================================================================
Name : close
==============================================================================
*/

void  WidgetDialog::close ()
{
   opa::BundleView & view = get_ancestor <BundleView> ();
   
   _layer_aptr->_visibility = false;
   
   view.unembed (*this);
}



/*
==============================================================================
Name : update_layout
==============================================================================
*/

void  WidgetDialog::update_layout ()
{
   opa::Rect header_min_bbox;
   opa::Rect content_min_bbox;
   opa::Rect footer_min_bbox;
   
   float width = 0.f;
   
   for (int i = 0 ; i < 2 ; ++i)
   {
      width = 0.f;
      
      if (_header_widget_sptr.get () != 0)
      {
         _header_widget_sptr->get_min_bbox (header_min_bbox);
      }
      
      if (_content_widget_sptr.get () != 0)
      {
         _content_widget_sptr->get_min_bbox (content_min_bbox);
      }
      
      if (_footer_widget_sptr.get () != 0)
      {
         _footer_widget_sptr->get_min_bbox (footer_min_bbox);
      }

      lang::max_in_place (width, header_min_bbox._size._width);
      lang::max_in_place (width, content_min_bbox._size._width);
      lang::max_in_place (width, footer_min_bbox._size._width);
      
      Size bounds_header = header_min_bbox._size;
      Size bounds_content = content_min_bbox._size;
      Size bounds_footer = footer_min_bbox._size;
      
      if (_header_widget_sptr.get () != 0)
      {
         if (_header_widget_sptr->get_horizontal_weight () > 0.f)
         {
            bounds_header._width = width;
         }
         
         if (_header_widget_sptr->get_vertical_weight () > 0.f)
         {
            bounds_header._height = _header_height;
         }
      }
      
      if (_content_widget_sptr.get () != 0)
      {
         if (_content_widget_sptr->get_horizontal_weight () > 0.f)
         {
            bounds_content._width = width;
         }
      }
      
      if (_footer_widget_sptr.get () != 0)
      {
         if (_footer_widget_sptr->get_horizontal_weight () > 0.f)
         {
            bounds_footer._width = width;
         }
         
         if (_footer_widget_sptr->get_vertical_weight () > 0.f)
         {
            bounds_footer._height = _footer_height;
         }
      }
      
      bool retrigger_layout_flag = false;
      
      if (_header_widget_sptr.get () != 0)
      {
         retrigger_layout_flag |= _header_widget_sptr->layout (bounds_header);
      }
      
      if (_content_widget_sptr.get () != 0)
      {
         retrigger_layout_flag |= _content_widget_sptr->layout (bounds_content);
      }
      
      if (_footer_widget_sptr.get () != 0)
      {
         retrigger_layout_flag |= _footer_widget_sptr->layout (bounds_footer);
      }
      
      if (!retrigger_layout_flag)
      {
         break;
      }
   }
   
   opa::Size bounds;
   bounds._width = width + 2.f * _content_padding_x;
   bounds._height
      = _header_height
      + content_min_bbox._size._height
      + 2.f * _content_padding_y
      + _footer_height
   ;
   
   set_bounds (bounds, get_transition_time (), get_transition_function ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetDialog::WidgetDialog (opa::BundleView & view)
:  _frame_aptr ()
,  _frame_back_aptr ()
,  _frame_dialog_aptr ()
,  _frame_header_aptr ()
,  _frame_content_aptr ()
,  _frame_footer_aptr ()
,  _layer_aptr ()
,  _prim_dialog_aptr ()
,  _layer_header_aptr ()
,  _layer_content_aptr ()
,  _layer_footer_aptr ()
{
   _frame_aptr = new opal::Frame ("WidgetDialog::frame");
   _frame_aptr->set_bounds (view.get_bounds ());
   
   _frame_key_aptr = new opal::FrameKey ();
   _frame_key_aptr->set_bounds (view.get_bounds ());
   _frame_key_aptr->set_spatial ();
   _frame_key_aptr->bind <
      WidgetDialog,
      &WidgetDialog::handle_key_event
   > (*this);
   _frame_aptr->insert_top (*_frame_key_aptr);
   
   _frame_back_aptr = new FrameDialogBack (*this);
   _frame_back_aptr->set_bounds (view.get_bounds ());
   _frame_aptr->insert_top (*_frame_back_aptr);
   
   _frame_block_aptr = new FrameDialogBlock (*this);
   _frame_aptr->insert_top (*_frame_block_aptr);
   
   _frame_dialog_aptr = new opal::Frame ("WidgetDialog::dialog");
   _frame_aptr->insert_top (*_frame_dialog_aptr);
   
   _frame_header_aptr = new opal::Frame ("WidgetDialog::header");
   _frame_dialog_aptr->insert_top (*_frame_header_aptr);

   _frame_content_aptr = new opal::Frame ("WidgetDialog::content");
   _frame_dialog_aptr->insert_top (*_frame_content_aptr);

   _frame_footer_aptr = new opal::Frame ("WidgetDialog::footer");
   _frame_dialog_aptr->insert_top (*_frame_footer_aptr);


   _layer_aptr = new opak::Layer;
   _layer_aptr->set_name ("WidgetDialog::layer");
   
   _prim_dialog_aptr = new opa::PrimDialog;
   _prim_dialog_aptr->set_name ("WidgetDialog::back");
   _layer_aptr->push_back (*_prim_dialog_aptr);

   _layer_header_aptr = new LayerWidget;
   _layer_header_aptr->set_name ("WidgetDialog::header");
   _layer_aptr->push_back (*_layer_header_aptr);
   _layer_header_aptr->_label_style = AspectWidgetLabel::Style_SECTION;
   
   _layer_content_aptr = new opak::Layer;
   _layer_content_aptr->set_name ("WidgetDialog::content");
   _layer_aptr->push_back (*_layer_content_aptr);

   _layer_footer_aptr = new opak::Layer;
   _layer_footer_aptr->set_name ("WidgetDialog::footer");
   _layer_aptr->push_back (*_layer_footer_aptr);
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (NBR_Slot_TOTAL);

   set_slot (Slot_HEADER, *_frame_header_aptr);
   set_slot (Slot_HEADER, *_layer_header_aptr);

   set_slot (Slot_CONTENT, *_frame_content_aptr);
   set_slot (Slot_CONTENT, *_layer_content_aptr);

   set_slot (Slot_FOOTER, *_frame_footer_aptr);
   set_slot (Slot_FOOTER, *_layer_footer_aptr);
   
   view.embed (*this, opa::BundleView::Slot_DIALOG);
   
   bind <
      WidgetDialog, 
      &WidgetDialog::handle_event_default
   > (*this);
   
   Bundle::bind <
      WidgetDialog, 
      opa::EventBundleView, 
      &WidgetDialog::handle_bundle_view_event
   > ();
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetDialog::do_get_min_bbox (Rect & min_bbox)
{
   assert (false);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetDialog::do_get_margin (Margin & margin)
{
   assert (false);
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetDialog::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _layer_aptr->_origin.stop ().push (origin, tt, tf);

   _frame_dialog_aptr->set_origin (origin);
   
   _frame_block_aptr->set_origin (origin);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetDialog::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   opa::Rect header_rect;
   header_rect._origin._x = _header_padding_x;
   header_rect._size._width = bounds._width - 2.f * _header_padding_x;
   header_rect._size._height = _header_height;
   
   opa::Rect content_rect;
   content_rect._origin._x = _content_padding_x;
   content_rect._origin._y = _header_height + _content_padding_y;
   content_rect._size._width = bounds._width - 2.f * _content_padding_x;
   content_rect._size._height
      = bounds._height - _header_height - _footer_height
      - 2.f * _content_padding_y;
   
   opa::Rect footer_rect;
   footer_rect._origin._x = _content_padding_x;
   footer_rect._origin._y = bounds._height - _footer_height;
   footer_rect._size._width = bounds._width - 2.f * _content_padding_x;
   footer_rect._size._height = _footer_height;
   
   _frame_block_aptr->set_bounds (bounds);
   
   _frame_dialog_aptr->set_bounds (bounds);

   _frame_header_aptr->set_origin (header_rect._origin);
   _frame_header_aptr->set_bounds (header_rect._size);

   _frame_content_aptr->set_origin (content_rect._origin);
   _frame_content_aptr->set_bounds (content_rect._size);

   _frame_footer_aptr->set_origin (footer_rect._origin);
   _frame_footer_aptr->set_bounds (footer_rect._size);
   
   _prim_dialog_aptr->_size.stop ().push (bounds, tt, tf);
   
   _layer_header_aptr->_origin.stop ().push (header_rect._origin, tt, tf);
   _layer_content_aptr->_origin.stop ().push (content_rect._origin, tt, tf);
   _layer_footer_aptr->_origin.stop ().push (footer_rect._origin, tt, tf);
   
   opa::BundleView & view = get_ancestor <BundleView> ();

   opa::Size size = view.get_bounds ();

   std::vector <opa::Rect> rect_arr = opal::Screens::use_instance ().get_layout ();
   assert (!rect_arr.empty ());

#if 1
   opa::Size main_size = rect_arr [0]._size;
   
   lang::min_in_place (size._width, main_size._width);
   lang::min_in_place (size._height, main_size._height);

#elif 0  
   // for now : special case for double full screen
   
   if (rect_arr.size () >= 2)
   {
      opa::Rect full = rect_arr [0] | rect_arr [1];
      
      if (full._size == size)
      {
         size = rect_arr.front ()._size;
      }
   }
#endif
   
   opa::Point origin (
      floor ((size._width - bounds._width) * .5f),
      floor ((size._height - bounds._height) * .33f)
   );
   
   do_set_origin (origin, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetDialog::do_enable (bool enabled_flag)
{
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetDialog::do_show (bool show_flag)
{
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetDialog::do_activate (bool activate_flag)
{
}



/*
==============================================================================
Name : handle_event_default
==============================================================================
*/

void  WidgetDialog::handle_event_default (Event & event)
{
   close ();
}



/*
==============================================================================
Name: handle_key_event
==============================================================================
*/

void WidgetDialog::handle_key_event (opal::FrameKey::Event & event)
{
   opal::Event * opal_event_ptr = event._event_ptr;
   assert (opal_event_ptr != 0);
   
   if (opal_event_ptr != 0)
   {  
      Event widget_event (*this);
      opal::KeyCode key_code = opal_event_ptr->_key.get_key_code ();
      
      if (key_code == opal::KeyCode_ESCAPE)
      {
         widget_event._type = Event::Type_CANCEL;
         _delegate.param_ref_signal (widget_event);
      }
      else if (key_code == opal::KeyCode_RETURN)
      {
         widget_event._type = Event::Type_OK;
         _delegate.param_ref_signal (widget_event);
      }     
   }
   
}



/*
==============================================================================
Name : handle_push_button_event
==============================================================================
*/

void  WidgetDialog::handle_push_button_event (WidgetPushButton::Event & event)
{
   WidgetPushButton & widget = event._widget;
   
   Event::Type type = widget.get_data <Event::Type> ();
   
   Event widget_event (*this);
   widget_event._type = type;
   
   _delegate.param_ref_signal (widget_event);
}



/*
==============================================================================
Name: process_bundle_view_event
==============================================================================
*/

void WidgetDialog::handle_bundle_view_event (opa::EventBundleView & event)
{
   assert (event._type == opa::EventBundleView::Type_LAYOUT_CHANGED);

   opa::BundleView & bundle_view = get_ancestor <opa::BundleView> ();

   const opa::Size & size = bundle_view.get_bounds ();

   _frame_aptr->set_bounds (size);
   _frame_block_aptr->set_bounds (size);
   _frame_key_aptr->set_bounds (size);
   
   update_layout ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const float WidgetDialog::_header_height = 25.f;
const float WidgetDialog::_header_padding_x = 16.f;

const float WidgetDialog::_content_padding_x = 16.f;
const float WidgetDialog::_content_padding_y = 16.f;

const float WidgetDialog::_footer_height = 50.f;
   



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
