/*****************************************************************************

        PrimSticker.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 59357

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

#include "ohm/opa/private/PrimSticker.h"

#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/AutoContextState.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/Cache.h"
#include "ohm/lang/fnc.h"
#include "ohm/util/trace.h"

#include <cassert>
#include <string>


namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimSticker::PrimSticker ()
:  _state (*this, &PrimSticker::invalidate_on_change)
,  _signaled_flag (*this, &PrimSticker::invalidate_on_change)
,  _color_non_signaled (*this, &PrimSticker::invalidate_on_change)
,  _color_signaled (*this, &PrimSticker::invalidate_on_change)
,  _color_label (*this, &PrimSticker::label_changed)
,  _label (*this, &PrimSticker::label_changed)
,  _font (*this, &PrimSticker::label_changed)
,  _str_offset (*this, &PrimSticker::invalidate_on_change)

,  _area_string ()
,  _mask ()
{
   _font.init (std::pair <std::string, float> (Constants::_default_font_0, 8.f));
   _color_label.init (opal::Color::_black);
}



/*
==============================================================================
Name : display
==============================================================================
*/

void PrimSticker::display (ohm::opak::GraphicContext & context)
{
   using namespace opak;
   
   Primitive::Auto auto_prim_widget (*this, context);
   
   if (_mask.empty ())
   {
      _mask = opal::ImageManager::use_instance ().get_sprung_image (
         context.get (), "ohm.widget.sticker.mask.png", Rect (7, 8, 75 - 2 * 7, 20 - 2 * 8)
      );
   }

   const opa::Size   size = _size.get_cur_value ();
   bool              enabled_flag = _enabled_flag.get_cur_value ();
   bool              signaled_flag = _signaled_flag.get_cur_value ();
   State             state = _state.get_cur_value ();
   opal::Color       color = (
      signaled_flag ?
      _color_signaled.get_cur_value () :
      _color_non_signaled.get_cur_value ()
   );
   
   if (state == State_ROLLOVER)
   {
      color = opal::Color::interpolate (color, opal::Color::_white, .2f);
   }
   else if (state == State_CLICKED)
   {
      color = opal::Color::interpolate (color, opal::Color::_black, .2f);  
   }
   
   if (!enabled_flag)
   {
      context.get ().set_alpha (.5f);
   }
   
   bool transform_borders_flag = true;
   bool hinted_flag = false;  // we should be already hinted
   
   PrimitiveHelper::display (
      context, _mask, color, opa::Rect (opa::Point::_zero, size),
      transform_borders_flag, hinted_flag
   );
   
   {
      opa::Rect image_bounds = _area_string.get_image_bounds ();
      float str_offset = _str_offset.get_cur_value ();
      
      opa::Point pt (
         0.5f * (size._width - image_bounds._size._width),
         str_offset
      );
      
      PrimitiveHelper::display (
         context, _area_string, pt, 0, 0, opal::Color::_none, opal::Color::_none
      );
   }
   
   context.get ().set_alpha (1.f);
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimSticker::~PrimSticker ()
{
}



/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void PrimSticker::do_size_changed ()
{
   Sizable::do_size_changed ();

   _area_string.set_bounds (_size.get_cur_value ());
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : label_changed
==============================================================================
*/

void PrimSticker::label_changed ()
{
   const opak::FontProxy & font_proxy = _font.get_cur_value ();
   
   if (!font_proxy.empty ())
   {  
      std::string label = _label.get_cur_value ();
      opal::Scripture & scripture = _area_string.use_scripture ();
      
      _area_string.begin_edit ();
      scripture.begin_edit ();
      scripture.erase (0, scripture.size ());
      scripture.set_state_color (_color_label.get_cur_value ());
      scripture.clear_state_font ();
      scripture.push_state_font (font_proxy);
      scripture.insert (0, label);
      scripture.end_edit ();
      _area_string.end_edit ();

      invalidate_all ();
   }
}



/*
==============================================================================
Name : invalidate_on_change
==============================================================================
*/

void PrimSticker::invalidate_on_change ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

