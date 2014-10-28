/*****************************************************************************

        AspectKeyedBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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

#include "ohm/opak/base/AspectKeyedBase.h"
#include "ohm/lang/fnc.h"

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

AspectKeyedBase::AspectKeyedBase ()
:  _key ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectKeyedBase::~AspectKeyedBase ()
{
}



/*
==============================================================================
Name : make
Description :
   Make the aspect by colorizing bitmaps into images.
==============================================================================
*/

/*void   AspectKeyedBase::make (const opak::GraphicContext & context)
{
   Typical CLIENT skeleton
   
   // generally using concrete class
   const AspectKeyedBase & aspect = context.get <AspectKeyedBase> ();
   
   if (aspect.update (_aspect_key))
   {
      make_some_part (aspect);
      make_some_other_part (aspect);
   }
}*/



/*
==============================================================================
Name : update
Description :
   Update the key and returns true if the client aspect needs to update
   itself. See 'make' for typical client skeleton
==============================================================================
*/

bool  AspectKeyedBase::update (Key & key) const
{  
   bool changed_flag = _key != key;
   
   if (changed_flag)
   {
      key = _key;
   }
   
   return changed_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use_key
Description :
   Returns the internal key.
   Typical used with util::BinaryStreamOutput to produce a serialized key
   from concrete class members.
==============================================================================
*/

AspectKeyedBase::Key &  AspectKeyedBase::use_key ()
{
   return _key;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
