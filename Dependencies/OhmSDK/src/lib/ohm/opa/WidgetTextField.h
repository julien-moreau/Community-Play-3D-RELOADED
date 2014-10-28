/*****************************************************************************

        WidgetTextField.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 68727

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



#if ! defined (ohm_opa_WidgetTextField_HEADER_INCLUDED)
#define  ohm_opa_WidgetTextField_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/base/WidgetKeyboardFocusBase.h"
#include "ohm/opa/frame/FrameTextField.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/TextField.h"
#include "ohm/opak/SprungImage.h"
#include "ohm/opak/LayerClip.h"
#include "ohm/opak/LayerTransparency.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opa
{


class FrameTextField;

class WidgetTextField;
typedef mem::SharedPtr <WidgetTextField>  WidgetTextFieldSPtr;

class WidgetTextField
:  public WidgetBase
,  public WidgetKeyboardFocusBase
,  public WidgetCreationHelper <WidgetTextField>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef txt::unicode::Char (*CharProc) (txt::unicode::Char c);
   
   class Event
   {
   public:
      enum Type
      {
                  Type_BEGIN = 0,
                  Type_CHANGE,
                  Type_COMMIT,
                  Type_REVERT,
      };
      
                  Event (WidgetTextField & widget) : _widget (widget) {}
      
      WidgetTextField &
                  _widget;
      
      Type        _type;
      std::string _begin_str; // string before the begin of the edit
      std::string _previous_str; 
      std::string _suggested_str;
      opal::Key   _key; // for commit / revert
   };
   
   virtual        ~WidgetTextField ();
   
   void           reset_active_color (const opal::Color & color = _default_active_color);
   void           reset_deactive_color (const opal::Color & color = _default_deactive_color);
   
   void           set_style_static ();
   void           set_style_search ();
   void           set_style_mutate_label ();
   void           set_style_double_click_edit ();
   void           set_style_dont_commit_on_outside_click ();
   
   void           set_opt_style_outline (bool outline_flag = true);
   void           set_opt_no_copy_to_clipboard ();
   
   void           set_mode_multiline ();
   
   void           set_width (float width);
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();
   
   void           set_char_proc (CharProc char_proc);
   static txt::unicode::Char
                  char_proc_password (txt::unicode::Char c);
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   void           set_text (const std::string & text);
   std::string    get_text () const;
   
   void           select_all ();
   void           start_edit ();
   void           cancel_edit ();
   
   static opal::Color
                  _default_active_color;
   static opal::Color
                  _default_deactive_color;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetTextField ();

   // WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);

   // WidgetKeyboardFocusBase
   virtual opal::Frame &
                  do_get_keyboard_focus_sensitive_frame ();

   // delegates
   void           handle_frame_text_field_event (FrameTextField::Event & event);


   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetTextField);
   
   void           set_scripture_active ();
   void           set_scripture_deactive ();
   
   void           animate_active ();
   void           animate_deactive ();
   
   opal::AutoFramePtr <FrameTextField>
                  _frame_aptr;
   opak::AutoPrimPtr <opak::LayerTransparency>
                  _layer_transp_aptr;
   opak::AutoPrimPtr <opak::LayerClip>
                  _layer_clip_aptr;
   opak::AutoPrimPtr <opak::SprungImage>
                  _prim_bkg_outline_aptr;
   opak::AutoPrimPtr <opak::SprungImage>
                  _prim_bkg_aptr;
   opak::AutoPrimPtr <opak::SprungImage>
                  _prim_bkg2_aptr;
   opak::AutoPrimPtr <opak::SprungImage>
                  _prim_focus_aptr;
   opak::AutoPrimPtr <opak::TextField>
                  _prim_aptr;
   
   opal::Scripture
                  _scripture;
   opal::Scripture
                  _processed_scripture;
   std::string    _previous_str;
   std::string    _begin_str;

   WidgetMetricAuto
                  _width;
   float          _height;
   
   bool           _style_search_flag;
   bool           _mutate_label_flag;
   
   bool           _active_flag;
   opal::Color    _active_color;
   opal::Color    _deactive_color;
   
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetTextField (const WidgetTextField &other);
   WidgetTextField & operator = (const WidgetTextField &other);
   bool           operator == (const WidgetTextField &other) const;
   bool           operator != (const WidgetTextField &other) const;

}; // class WidgetTextField



}  // namespace opa
}  // namespace ohm



#include "ohm/opa/WidgetTextField.hpp"



#endif   // ohm_opa_WidgetTextField_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

