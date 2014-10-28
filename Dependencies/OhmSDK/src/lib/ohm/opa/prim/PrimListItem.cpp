/*****************************************************************************

        PrimListItem.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54611

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

#include "ohm/opa/prim/PrimListItem.h"
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
Name: PrimListItem
==============================================================================
*/

PrimListItem::PrimListItem ()
:  _selected_flag (*this, &PrimListItem::this_changed)
,  _label (*this, &PrimListItem::this_changed)
{
   _enabled_flag.init (true);
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimListItem::display (opak::GraphicContext & context)
{
   opak::Primitive::Auto auto_prim_widget (*this, context);
   
   opa::Size   size = _size.get_cur_value ();
   const bool  selected_flag = _selected_flag.get_cur_value ();
   
   const std::string & label = _label.get_cur_value ();
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetList & aspect_list = aspect._list;
   
   opa::Rect rect;
   rect._size = _size.get_cur_value ();
   context.apply (rect);

   if (selected_flag)
   {
      context.get ().draw (aspect_list._item_back, rect);
   }
   
   {
      opa::Point pt (
         rect._origin._x + 8.f,
         rect._origin._y + 12.f
      );
      
      int s = selected_flag ? AspectWidgetList::Selection_ON : AspectWidgetList::Selection_OFF;
      
      opal::LabelString label_string;
      
      label_string.begin_edit ();

      opal::Scripture & scripture = label_string.use_scripture ();
      scripture.clear ();
      scripture.push_state_font (
         opal::FontManager::use_instance ().get_font (Constants::_default_font_0, 11.f)
      );
      scripture.set_state_color (aspect_list._item_text_arr [s]);
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
Name: ~PrimListItem
==============================================================================
*/

PrimListItem::~PrimListItem ()
{
}



/*
==============================================================================
Name: do_enabled_flag_changed
==============================================================================
*/

void PrimListItem::do_enabled_flag_changed ()
{
   invalidate_all ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: this_changed
==============================================================================
*/

void PrimListItem::this_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

