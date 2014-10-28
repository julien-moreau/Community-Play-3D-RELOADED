/*****************************************************************************

        WidgetImage.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 56929

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

#include "ohm/opa/WidgetImage.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/private/Typesetter.h"

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

WidgetImage::~WidgetImage ()
{
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
==============================================================================
*/

void  WidgetImage::set_width (float width)
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

void  WidgetImage::set_width_auto_content ()
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

void  WidgetImage::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetImage::get_width ()
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

bool  WidgetImage::is_width_auto () const
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

void  WidgetImage::set_height (float height)
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

void  WidgetImage::set_height_auto_content ()
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

void  WidgetImage::set_height_auto_parent (float weight)
{
   _height.set_ext ();
   set_vertical_weight (weight);
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetImage::get_height ()
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

bool  WidgetImage::is_height_auto () const
{
   return _height.is_auto ();
}



/*
==============================================================================
Name : set_path
==============================================================================
*/

void  WidgetImage::set_path (const std::string & path)
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

const std::string &  WidgetImage::get_path () const
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

WidgetImage::WidgetImage ()
:  _prim_aptr ()
,  _width (*this)
,  _height (*this)
,  _path ()
{
   _prim_aptr = new opak::Image;
   _prim_aptr->set_name ("WidgetImage::_prim_aptr");
   
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetImage::do_get_min_bbox (Rect & min_bbox)
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

void  WidgetImage::do_get_margin (Margin & margin)
{
   margin = opa::Margin (4, 1, 4, 2);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetImage::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetImage::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   // nothing
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetImage::do_enable (bool enabled_flag)
{
   _prim_aptr->_opacity = .3f;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetImage::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetImage::do_activate (bool activate_flag)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetImage::update_metrics ()
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



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
