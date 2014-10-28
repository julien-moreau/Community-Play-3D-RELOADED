/*****************************************************************************

        ShaderFloat4.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 26998

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

#include "ohm/opal/shader/ShaderFloat4.h"
#include "ohm/opal/private/FragmentProgramManager.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Temporary variable
==============================================================================
*/

ShaderFloat4::ShaderFloat4 ()
:  ShaderOperatable (FragmentProgramManager::use_instance ().get_temporary_name ())
,  r (get_name () + ".r")
,  g (get_name () + ".g")
,  b (get_name () + ".b")
,  a (get_name () + ".a")
,  x (get_name () + ".x")
,  y (get_name () + ".y")
{
   FragmentProgramManager::use_instance ().declare_temporary (get_name ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ShaderFloat4::~ShaderFloat4 ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

void  ShaderFloat4::operator = (const ShaderFloat4 &other)
{
   this->ShaderOperatable::operator = (other);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Predefined variable
==============================================================================
*/

ShaderFloat4::ShaderFloat4 (const std::string & name)
:  ShaderOperatable (name)
,  r (get_name () + ".r")
,  g (get_name () + ".g")
,  b (get_name () + ".b")
,  a (get_name () + ".a")
,  x (get_name () + ".x")
,  y (get_name () + ".y")
{
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
