/*****************************************************************************

        AspectBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/AspectBase.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

AspectBase::AspectBase ()
:  _first_flag (true)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectBase::~AspectBase ()
{
}



/*
==============================================================================
Name : make
Description :
   Make the aspect by colorizing bitmaps into images.
==============================================================================
*/

/*void   AspectBase::make (opak::GraphicContext & context)
{
   // default impl : nothing
   
   // Typical CLIENT skeleton
   
   const SomeAspect & aspect = context.get <SomeAspect> ();
   
   if (aspect.update (_some_internal_key))
   {
      make_some_part (aspect);
      make_some_other_part (aspect);
   }
}*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
Description :
   Init the aspect by loading colorizable base bitmaps and loading non
   colorized images.
==============================================================================
*/

/*void   AspectBase::do_init (GraphicContext & context)
{
   // default impl : nothing
   
   // Typical impl skeleton ; load_base_elements is STATIC
   
   load_base_elements ();
   
   opal::ImageManager & im = opal::ImageManager::use_instance ();

   // non colorized ressources

   _image = im.get_image (context.get (), "some_image");
   _sprung_image = im.get_sprung_image (
      context.get (), "some_sprung_image"
      opa::Rect (xxx, xxx, xxx, xxx)
   );
}*/



/*
==============================================================================
Name : need_make
Description :
   Returns true first time called.
==============================================================================
*/

bool  AspectBase::need_make (GraphicContext & context)
{
   if (!_first_flag)
   {
      return false;
   }
   else
   {
      do_init (context);
      
      _first_flag = false;
      
      return true;
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
