/*****************************************************************************

        PrimMenuItem.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69920

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

#include "ohm/opa/prim/PrimMenuItem.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/FontManager.h"

#include <cassert>



namespace ohm
{
namespace opa
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimMenuItem
==============================================================================
*/

PrimMenuItem::PrimMenuItem ()
:  _rollover_flag (*this, &PrimMenuItem::this_changed)
,  _arrow_flag (*this, &PrimMenuItem::this_changed)
,  _mark (*this, &PrimMenuItem::this_changed)
,  _label (*this, &PrimMenuItem::this_changed)
,  _section_flag (*this, &PrimMenuItem::this_changed)
{
   _enabled_flag.init (true);
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimMenuItem::display (opak::GraphicContext & context)
{
   opak::Primitive::Auto auto_prim_widget (*this, context);
   
   opa::Size size = _size.get_cur_value ();
   const MarkType mark = _mark.get_cur_value ();
   const bool arrow_flag = _arrow_flag.get_cur_value ();
   const bool enabled_flag = _enabled_flag.get_cur_value ();
   const bool rollover_flag = enabled_flag && _rollover_flag.get_cur_value ();
   
   const std::string & label = _label.get_cur_value ();
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetMenu & aspect_menu = aspect._menu;
   
   opa::Rect rect;
   rect._size = _size.get_cur_value ();
   context.apply (rect);

   if (rollover_flag)
   {
      context.get ().draw (aspect_menu._item_back, rect);
   }
   
   if (mark != MarkType_NONE)
   {
      opa::Point pt (
         rect._origin._x + 9.f,
         rect._origin._y + 5.f
      );
      
      int s = rollover_flag ? 1 : 0;
      
      if (mark == MarkType_CHECK)
      {
         context.get ().draw (aspect_menu._item_mark_check_arr [s], pt);
      }
      else if (mark == MarkType_CHECK_MULTI)
      {
         context.get ().draw (aspect_menu._item_mark_check_multi_arr [s], pt);
      }
      else
      {
         assert (false);
      }
   }
   
   if (arrow_flag)
   {
      opa::Point pt (
         rect._origin._x + rect._size._width - 13.f,
         rect._origin._y + 5.f
      );
      
      int s = rollover_flag ? 1 : 0;
      
      context.get ().draw (aspect_menu._item_sub_arrow_arr [s], pt);
   }
      
   {
      float indent = 23.f;
      
      bool section_flag = _section_flag.get_cur_value ();
      
      if (section_flag)
      {
         indent = 12.f;
      }
      
      opa::Point pt (
         rect._origin._x + indent,
         rect._origin._y + 12.f
      );
      
      int s = rollover_flag ? 1 : 0;
      int e = enabled_flag ? 1 : 0;
      
      opal::LabelString label_string;
      
      label_string.begin_edit ();

      opal::Scripture & scripture = label_string.use_scripture ();
      scripture.clear ();
      scripture.push_state_font (
         opal::FontManager::use_instance ().get_font (Constants::_default_font_0, 11.f)
      );
      scripture.set_state_color (aspect_menu._item_text_arr [s][e]);
      scripture.insert (0, label);

      label_string.end_edit ();
      
      context.get ().save_graphic_state ();
      context.get ().concat_translate (pt._x, pt._y);
      
      label_string.draw (context.get ());
      
      context.get ().restore_graphic_state ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimMenuItem
==============================================================================
*/

PrimMenuItem::~PrimMenuItem ()
{
}



/*
==============================================================================
Name: do_enabled_flag_changed
==============================================================================
*/

void PrimMenuItem::do_enabled_flag_changed ()
{
   invalidate_all ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: this_changed
==============================================================================
*/

void PrimMenuItem::this_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

