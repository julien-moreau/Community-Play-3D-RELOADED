/*****************************************************************************

        PrimComboBox.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55650

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

#include "ohm/opa/prim/PrimComboBox.h"
#include "ohm/opa/private/AspectWidget.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimComboBox
==============================================================================
*/

PrimComboBox::PrimComboBox ()
{
}



/*
==============================================================================
Name: ~PrimComboBox
==============================================================================
*/

PrimComboBox::~PrimComboBox ()
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimComboBox::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetComboBox & aspect_combo_box = aspect._combo_box;
   
   opa::Rect rect;
   rect._size = _size.get_cur_value ();
   
   opak::use (aspect_combo_box._button_arr [AspectWidgetComboBox::State_OFF])
      .rect (rect)
      .margin (1, 1, 1, 1)
      .hint ()
      .display (context)
   ;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void  PrimComboBox::do_size_changed ()
{
   // add shadows
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect;
   rect._size = size;
   
   rect += opa::Margin (1, 1, 1, 1);
   
   set_bounding_box (rect);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

