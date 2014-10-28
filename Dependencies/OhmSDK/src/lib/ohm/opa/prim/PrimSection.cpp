/*****************************************************************************

        PrimSection.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54636

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

#include "ohm/opa/prim/PrimSection.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opak/base/PrimitiveHelper.h"

#include <cassert>



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

PrimSection::PrimSection ()
:  _type (*this, &PrimSection::type_changed)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void PrimSection::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetSection & aspect_section = aspect._section;
   
   Size size = _size.get_cur_value ();
   Type type = _type.get_cur_value ();
   
   opal::SprungImage image;
   
   switch (type)
   {
   case Type_HEADER:
      image = aspect_section._header;
      break;

   case Type_CONTENT:
      image = aspect_section._content;
      break;

   case Type_FOOTER:
      image = aspect_section._footer;
      break;
   }
   
   opak::use (image)
      .rect (Point::_zero, size)
      .scale_border ()
      .hint ()
      .display (context)
   ;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimSection::~PrimSection ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : type_changed
==============================================================================
*/

void  PrimSection::type_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

