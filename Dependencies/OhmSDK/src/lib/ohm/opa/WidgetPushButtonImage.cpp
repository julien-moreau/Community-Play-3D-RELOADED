/*****************************************************************************

        WidgetPushButtonImage.cpp
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 59681

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

#include "ohm/opa/WidgetPushButtonImage.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/Bitmap.h"

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

WidgetPushButtonImage::~WidgetPushButtonImage ()
{
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
==============================================================================
*/

void  WidgetPushButtonImage::set_width (float width)
{
   _width.set (width);
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name : set_width_auto_content
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual content width.
==============================================================================
*/

void  WidgetPushButtonImage::set_width_auto_content ()
{
   _width.set_auto ();
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

void  WidgetPushButtonImage::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetPushButtonImage::get_width ()
{
   if (_width.need_update ())
   {
      update_metrics ();
   }
   
   return _width.get ();
}



/*
==============================================================================
Name : is_width_auto
Note :
   Used by decorators (e.g. WidgetPushButton)
==============================================================================
*/

bool  WidgetPushButtonImage::is_width_auto () const
{
   return _width.is_auto ();
}



/*
==============================================================================
Name : set_height
Description :
   Fix the height of 'this', independent of actual content height.
==============================================================================
*/

void  WidgetPushButtonImage::set_height (float height)
{
   _height.set (height);
   set_vertical_weight (0.f);
}



/*
==============================================================================
Name : set_height_auto_content
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual content height.
==============================================================================
*/

void  WidgetPushButtonImage::set_height_auto_content ()
{
   _height.set_auto ();
   set_vertical_weight (0.f);
}



/*
==============================================================================
Name : set_height_auto_parent
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual parent height.
==============================================================================
*/

void  WidgetPushButtonImage::set_height_auto_parent (float weight)
{
   _height.set_ext ();
   set_vertical_weight (weight);
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetPushButtonImage::get_height ()
{
   if (_height.need_update ())
   {
      update_metrics ();
   }
   
   return _height.get ();
}



/*
==============================================================================
Name : is_height_auto
Note :
   Used by decorators (e.g. WidgetPushButton)
==============================================================================
*/

bool  WidgetPushButtonImage::is_height_auto () const
{
   return _height.is_auto ();
}



/*
==============================================================================
Name : set_path
==============================================================================
*/

void  WidgetPushButtonImage::set_path (const std::string & path)
{
   if (_path != path)
   {
      _path = path;

      _prim_aptr->_image = path;
      
      _width.invalidate ();
      _height.invalidate ();
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name : get_path
==============================================================================
*/

const std::string &  WidgetPushButtonImage::get_path () const
{
   return _path;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetPushButtonImage::WidgetPushButtonImage ()
:  _prim_aptr ()
,  _width (*this)
,  _height (*this)
,  _path ()
{
   _prim_aptr = new opak::Image;
   _prim_aptr->set_name ("WidgetPushButtonImage::_prim_aptr");
   
   _frame_aptr = new opal::FrameSignal;
   _frame_aptr->bind_gesture_signal <
      WidgetPushButtonImage,
      &WidgetPushButtonImage::handle_gesture_signal
   > (*this);
   
   set_embeddable (*_prim_aptr);
   set_embeddable (*_frame_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetPushButtonImage::do_get_min_bbox (Rect & min_bbox)
{
   if (_width.need_update () || _height.need_update ())
   {
      update_metrics ();
   }
   
   min_bbox = opa::Rect (
      0, 0,
      _width.get (), _height.get ()
   );
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetPushButtonImage::do_get_margin (Margin & margin)
{
   margin = opa::Margin (4, 1, 4, 2);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetPushButtonImage::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
   _frame_aptr->set_origin (origin);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetPushButtonImage::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetPushButtonImage::do_enable (bool enabled_flag)
{
   _prim_aptr->_opacity = enabled_flag ? 1.f : .3f;
   _frame_aptr->activate (enabled_flag);
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetPushButtonImage::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetPushButtonImage::do_activate (bool activate_flag)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: handle_gesture_signal
==============================================================================
*/

void WidgetPushButtonImage::handle_gesture_signal (opal::FrameSignal::Event & event)
{
   assert (event._event_ptr != 0);
   
   Event widget_event (*this, *event._event_ptr);
   
   _delegate.param_ref_signal (widget_event);
}



/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetPushButtonImage::update_metrics ()
{
   if (_width.need_update () || _height.need_update ())
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
      
      opal::Bitmap bitmap;
      
      bool ok_flag = il.load (bitmap, _path);
      assert (ok_flag);
      
      if (ok_flag)
      {
         float width = float (bitmap.get_width ());
         float height = float (bitmap.get_height ());
         
         _width.update (width);
         _height.update (height);
      }
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

