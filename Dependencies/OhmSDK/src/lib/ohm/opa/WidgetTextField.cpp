/*****************************************************************************

        WidgetTextField.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 70452

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetTextField.h"
#include "ohm/opa/frame/FrameTextField.h"

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

WidgetTextField::~WidgetTextField ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
      
      // stop the loop so that the primitive can be removed
      _prim_aptr->_caret_color.stop ();
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : reset_active_color
==============================================================================
*/

void  WidgetTextField::reset_active_color (const opal::Color & color)
{
   _active_color = color;
   
   if (_active_flag)
   {
      set_scripture_active ();
   }
}



/*
==============================================================================
Name : reset_deactive_color
==============================================================================
*/

void  WidgetTextField::reset_deactive_color (const opal::Color & color)
{
   _deactive_color = color;
   
   if (!_active_flag)
   {
      set_scripture_deactive ();
   }
}



/*
==============================================================================
Name : set_style_static
==============================================================================
*/

void  WidgetTextField::set_style_static ()
{
   _prim_bkg_aptr->_opacity = 0.f;
   set_scripture_deactive ();
   
   _mutate_label_flag = false;
   
   _frame_aptr->deactivate ();
}



/*
==============================================================================
Name : set_style_search
==============================================================================
*/

void  WidgetTextField::set_style_search ()
{
   _prim_bkg_aptr->_image = "ohm.widget.searchfield.base.png";
   _prim_bkg_aptr->_blend_mode = opak::BlendMode_MULTIPLY;
   _prim_bkg_aptr->_rect = opa::Rect (20, 0, 1, 19);

   _prim_bkg2_aptr->_image = "ohm.widget.searchfield.base.png";
   _prim_bkg2_aptr->_rect = opa::Rect (20, 0, 1, 19);
   _prim_bkg2_aptr->_opacity = 0.f;
   
   opa::Point origin (21, 2);
   
   // bug here
   
   _frame_aptr->set_origin (origin);
   _prim_aptr->_origin = origin;
   
   if (!_active_flag)
   {
      set_scripture_deactive ();
   }

   _style_search_flag = true;
   _mutate_label_flag = true;
}



/*
==============================================================================
Name : set_style_mutate_label
==============================================================================
*/

void  WidgetTextField::set_style_mutate_label ()
{
   _prim_bkg_aptr->_opacity = 0.f;
   
   if (!_active_flag)
   {
      set_scripture_deactive ();
   }
   
   _mutate_label_flag = true;
   
   _frame_aptr->activate ();
}



/*
==============================================================================
Name : set_style_double_click_edit
==============================================================================
*/

void  WidgetTextField::set_style_double_click_edit ()
{
   set_style_mutate_label ();
   
   _mutate_label_flag = false;
   
   FrameTextField::Config config = _frame_aptr->get_config ();

   config._edit_on_double_click_flag = true;

   _frame_aptr->set_config (config);
}



/*
==============================================================================
Name : set_style_dont_commit_on_outside_click
==============================================================================
*/

void  WidgetTextField::set_style_dont_commit_on_outside_click ()
{
   FrameTextField::Config config = _frame_aptr->get_config ();

   config._commit_on_outside_click_flag = false;

   _frame_aptr->set_config (config);
}



/*
==============================================================================
Name : set_opt_style_outline
==============================================================================
*/

void  WidgetTextField::set_opt_style_outline (bool outline_flag)
{
   _prim_bkg_outline_aptr->_opacity = outline_flag ? .1f : 0.f;
}



/*
==============================================================================
Name: set_opt_no_copy_to_clipboard
==============================================================================
*/

void  WidgetTextField::set_opt_no_copy_to_clipboard ()
{
   FrameTextField::Config config = _frame_aptr->get_config ();
   config._allow_copy_to_clipboard_flag = false;
   
   _frame_aptr->set_config (config);
}



/*
==============================================================================
Name: set_mode_multiline
==============================================================================
*/

void  WidgetTextField::set_mode_multiline ()
{
   _frame_aptr->set_mode (opa::FrameTextField::Mode_FIXED_WIDTH);
}



/*
==============================================================================
Name : set_width
==============================================================================
*/

void  WidgetTextField::set_width (float width)
{
   _width.set (width);
   set_horizontal_weight (0.f);
}

   
   
/*
==============================================================================
Name : set_width_auto_parent
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual parent width.
==============================================================================
*/

void  WidgetTextField::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name: get_width
==============================================================================
*/

float WidgetTextField::get_width ()
{
   if (_width.need_update ())
   {
      _width.update (0.f);
   }
   
   return _width.get ();
}

   

/*
==============================================================================
Name : set_char_proc
==============================================================================
*/

void  WidgetTextField::set_char_proc (CharProc char_proc)
{
   _frame_aptr->set_char_proc (char_proc);
}



/*
==============================================================================
Name : char_proc_password
==============================================================================
*/

txt::unicode::Char   WidgetTextField::char_proc_password (txt::unicode::Char c)
{
// return 0x2022; // BULLET
   return 0x266B; // BEAMED EIGHTH NOTES
}


   
/*
==============================================================================
Name : set_text
==============================================================================
*/

void  WidgetTextField::set_text (const std::string & text)
{
   // primitive is set in this call
   _frame_aptr->set (text);
}



/*
==============================================================================
Name : get_text
==============================================================================
*/

std::string WidgetTextField::get_text () const
{
   return _scripture.get_utf8_str ();
}


   
/*
==============================================================================
Name : select_all
==============================================================================
*/

void  WidgetTextField::select_all ()
{
   _frame_aptr->select_all ();
}



/*
==============================================================================
Name : start_edit
==============================================================================
*/

void  WidgetTextField::start_edit ()
{
   _frame_aptr->start_edit ();
}



/*
==============================================================================
Name : cancel_edit
==============================================================================
*/

void  WidgetTextField::cancel_edit ()
{
   _frame_aptr->cancel_edit ();
}



opal::Color WidgetTextField::_default_active_color (255, 0);
opal::Color WidgetTextField::_default_deactive_color (255, 171);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetTextField::WidgetTextField ()
:  WidgetBase ()
,  _width (*this)
,  _height (14 + 5)
,  _style_search_flag (false)
,  _mutate_label_flag (false)
,  _active_color (_default_active_color)
,  _deactive_color (_default_deactive_color)
,  _active_flag (false)
{
   opa::Point tf_origin (3.f, 2.f);
   
   _frame_aptr = new FrameTextField (_scripture, &_processed_scripture);
   _frame_aptr->set_origin (tf_origin);

   _frame_aptr->bind_edit <
      WidgetTextField,
      &WidgetTextField::handle_frame_text_field_event
   > (*this);
   
   _frame_aptr->add_key_bind_commit (opal::Key (opal::KeyCode_RETURN));
// _frame_aptr->add_key_bind_commit (opal::Key (opal::KeyCode_NUMERIC_KEYPAD_ENTER));

   _frame_aptr->add_key_bind_revert (opal::Key (opal::KeyCode_ESCAPE));
   
   set_embeddable (*_frame_aptr);
   
   _layer_transp_aptr = new opak::LayerTransparency;
   _layer_transp_aptr->set_name ("WidgetTextField::transparency");
   set_embeddable (*_layer_transp_aptr);
   
   _layer_clip_aptr = new opak::LayerClip;
   _layer_clip_aptr->set_name ("WidgetTextField::clip");
   _layer_transp_aptr->push_back (*_layer_clip_aptr);
   
   _prim_bkg_outline_aptr = new opak::SprungImage;
   _prim_bkg_outline_aptr->set_name ("WidgetTextField::_prim_bkg_outline_aptr");
   _prim_bkg_outline_aptr->_image = "ohm.widget.textfield.outline.png";
   _prim_bkg_outline_aptr->_rect = opa::Rect (2, 1, 1, 17);
   _prim_bkg_outline_aptr->_opacity = 0.f;
   
   _prim_bkg_aptr = new opak::SprungImage;
   _prim_bkg_aptr->set_name ("WidgetTextField::_prim_bkg_aptr");
   _prim_bkg_aptr->_image = "ohm.widget.textfield.base.png";
   _prim_bkg_aptr->_rect = opa::Rect (2, 2, 1, 16);

   _prim_bkg2_aptr = new opak::SprungImage;
   _prim_bkg2_aptr->set_name ("WidgetTextField::_prim_bkg2_aptr");
   
   _prim_focus_aptr = new opak::SprungImage;
   _prim_focus_aptr->set_name ("WidgetTextField::_prim_focus_aptr");
   _prim_focus_aptr->_opacity = 0.f;
   
   _prim_aptr = new opak::TextField;
   _prim_aptr->set_name ("WidgetTextField::_prim_aptr");
   _prim_aptr->_origin = tf_origin;
   _prim_aptr->_selection_color
      = opal::Color (255, 255, 213, 0);
   
   _layer_clip_aptr->push_back (*_prim_bkg_outline_aptr);
   _layer_clip_aptr->push_back (*_prim_bkg_aptr);
   _layer_clip_aptr->push_back (*_prim_bkg2_aptr);
   _layer_clip_aptr->push_back (*_prim_focus_aptr);
   _layer_clip_aptr->push_back (*_prim_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetTextField::do_get_min_bbox (Rect & min_bbox)
{
   if (_width.need_update ())
   {
      _width.update (0.f);
   }
   
   min_bbox = Rect (0, -14, _width.get (), _height);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetTextField::do_get_margin (Margin & margin)
{
   margin = opa::Margin (10, 6, 10, 6);
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetTextField::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   Point pt = origin;
   
   if (_style_search_flag)
   {
      _frame_aptr->set_origin (pt + opa::Point (21, 2));
   }
   else
   {
      _frame_aptr->set_origin (pt + opa::Point (3, 2));
   }

   _layer_transp_aptr->_origin.stop ().push (pt, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetTextField::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);

   _layer_clip_aptr->_clip_size.stop ().push (bounds, tt, tf);
   _prim_bkg_outline_aptr->_size.stop ().push (bounds, tt, tf);
   _prim_bkg_aptr->_size.stop ().push (bounds, tt, tf);
   _prim_bkg2_aptr->_size.stop ().push (bounds, tt, tf);
   _prim_focus_aptr->_size.stop ().push (bounds, tt, tf);
   _prim_aptr->_size.stop ().push (bounds + opa::Size (0, 3), tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetTextField::do_enable (bool enabled_flag)
{
   _layer_transp_aptr->_opacity = enabled_flag ? 1.f : .30f;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetTextField::do_show (bool show_flag)
{
   _layer_transp_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetTextField::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*
==============================================================================
Name : do_get_keyboard_focus_sensitive_frame
==============================================================================
*/

opal::Frame &  WidgetTextField::do_get_keyboard_focus_sensitive_frame ()
{
   assert (_frame_aptr.get () != 0);
   
   return *_frame_aptr;
}



/*
==============================================================================
Name : handle_frame_text_field_event
==============================================================================
*/

void  WidgetTextField::handle_frame_text_field_event (FrameTextField::Event & event)
{
   bool edit_dbl_click_flag = _frame_aptr->get_config ()._edit_on_double_click_flag;
   
   switch (event._type)
   {
   case FrameTextField::Event::Type_BEGIN:
      {
         animate_active ();
         
         _prim_focus_aptr->_opacity = 1.f;
         _prim_aptr->_caret_color
            .stop ()
            .push (opal::Color (1.f, 0.f), 0.5, opak::inverse_step)
            .push (opal::Color (0.f, 0.f), 0.5, opak::inverse_step)
            .loop ();

         _begin_str = get_text ();
         _previous_str = _begin_str;
         
         Event widget_event (*this);
         widget_event._type = Event::Type_BEGIN;
         widget_event._begin_str = _begin_str;
         widget_event._previous_str = _begin_str;
         widget_event._suggested_str = _begin_str;
         widget_event._key = event._key;
         
         _delegate.param_ref_signal (widget_event);
      }
      break;

   case FrameTextField::Event::Type_COMMIT:
      {
         if (edit_dbl_click_flag)
         {
            animate_deactive ();
         }
         
         std::string cur_text = get_text ();
         
         Event widget_event (*this);
         widget_event._type = Event::Type_COMMIT;
         widget_event._begin_str = _begin_str;
         widget_event._previous_str = _previous_str;
         widget_event._suggested_str = cur_text;
         widget_event._key = event._key;
         
         _delegate.param_ref_signal (widget_event);
         
         if (widget_event._suggested_str != cur_text)
         {
            set_text (widget_event._suggested_str);
         }
         
         _previous_str = widget_event._suggested_str;
      }
      break;

   case FrameTextField::Event::Type_REVERT:
      {
         if (edit_dbl_click_flag || _mutate_label_flag)
         {
            animate_deactive ();
         }
         
         Event widget_event (*this);
         widget_event._type = Event::Type_REVERT;
         widget_event._begin_str = _begin_str;
         widget_event._previous_str = _previous_str;
         widget_event._suggested_str = _previous_str;
         widget_event._key = event._key;
         
         _delegate.param_ref_signal (widget_event);
      }
      break;

   case FrameTextField::Event::Type_END:
      _prim_focus_aptr->_opacity = 0.f;
      _prim_aptr->_caret_color = opal::Color (0.f, 0.f);
      break;

   case FrameTextField::Event::Type_SCRIPTURE_CHANGED:
      {
         std::string cur_text = get_text ();
         
         Event widget_event (*this);
         widget_event._type = Event::Type_CHANGE;
         widget_event._begin_str = _begin_str;
         widget_event._previous_str = _previous_str;
         widget_event._suggested_str = cur_text;
         widget_event._key = event._key;
         
         _delegate.param_ref_signal (widget_event);
         
         if (widget_event._suggested_str != cur_text)
         {
            set_text (widget_event._suggested_str);
            
            lang::min_in_place (event._first_caret_pos, _processed_scripture.size ());
            lang::min_in_place (event._second_caret_pos, _processed_scripture.size ());
         }
         
         _prim_aptr->_caret_first = event._first_caret_pos;
         _prim_aptr->_caret_second = event._second_caret_pos;
         _prim_aptr->_scripture = _processed_scripture;
         
         _previous_str = widget_event._suggested_str;
      }
         break;

   case FrameTextField::Event::Type_CARETS_POSITIONS_CHANGED:
         _prim_aptr->_caret_first = event._first_caret_pos;
         _prim_aptr->_caret_second = event._second_caret_pos;
      break;
   
   case FrameTextField::Event::Type_ENTERED:
      if (_mutate_label_flag)
      {
         animate_active ();
      }
      break;
   
   case FrameTextField::Event::Type_EXITED:
      if (_mutate_label_flag)
      {
         animate_deactive ();
      }
      break;

   case opa::FrameTextField::Event::Type_HEIGHT_CHANGED:
      {
         _height = event._height;
         lang::max_in_place (_height, 14.f + 5.f);
         
         invalidate_layout ();
      }
      break;
   
   default:
      // ignore
      break;
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_scripture_active
==============================================================================
*/

void  WidgetTextField::set_scripture_active ()
{
   _processed_scripture.set_state_color (_active_color);
   _processed_scripture.set_color (
      0, _processed_scripture.size (),
      _active_color
   );
   
   _prim_aptr->_scripture = _processed_scripture;
}



/*
==============================================================================
Name : set_scripture_deactive
==============================================================================
*/

void  WidgetTextField::set_scripture_deactive ()
{
   _processed_scripture.set_state_color (_deactive_color);
   _processed_scripture.set_color (
      0, _processed_scripture.size (),
      _deactive_color
   );
   
   _prim_aptr->_scripture = _processed_scripture;
}



/*
==============================================================================
Name : animate_active
==============================================================================
*/

void  WidgetTextField::animate_active ()
{
   if (!_active_flag)
   {
      set_scripture_active ();
      
      if (_style_search_flag)
      {
         _prim_bkg_aptr->_opacity.stop ().push (0.f, 0.125, opak::ease_in_ease_out);
         _prim_bkg2_aptr->_opacity.stop ().push (1.f, 0.125, opak::ease_in_ease_out);
      }
      else
      {
         _prim_bkg_aptr->_opacity.stop ().push (1.f, 0.125, opak::ease_in_ease_out);
      }
      
      _active_flag = true;
   }
}



/*
==============================================================================
Name : animate_deactive
==============================================================================
*/

void  WidgetTextField::animate_deactive ()
{
   if (_active_flag)
   {
      set_scripture_deactive ();

      if (_style_search_flag)
      {
         _prim_bkg_aptr->_opacity.stop ().push (1.f, 0.125, opak::ease_in_ease_out);
         _prim_bkg2_aptr->_opacity.stop ().push (0.f, 0.125, opak::ease_in_ease_out);
      }
      else
      {
         _prim_bkg_aptr->_opacity.stop ().push (0.f, 0.125, opak::ease_in_ease_out);
      }
      
      _active_flag = false;
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

