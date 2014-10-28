/*****************************************************************************

        ShaderOperatable.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 27044

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

#include "ohm/opal/shader/ShaderOperatable.h"
#include "ohm/opal/shader/ShaderFloat4.h"
#include "ohm/opal/shader/ShaderTexture.h"
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
==============================================================================
*/

ShaderOperatable::ShaderOperatable (const std::string & name)
:  ShaderVar (name)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ShaderOperatable::~ShaderOperatable ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

void  ShaderOperatable::operator = (const ShaderOperatable &other)
{
   FragmentProgramManager::use_instance ().exec_mov (get_name (), other.get_name ());
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

void  ShaderOperatable::operator += (const ShaderVar &other)
{
   FragmentProgramManager::use_instance ().exec_add (get_name (), get_name (), other.get_name ());
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

void  ShaderOperatable::operator += (float scalar)
{
   FragmentProgramManager::use_instance ().exec_add (get_name (), get_name (), scalar);
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

void  ShaderOperatable::operator *= (const ShaderVar &other)
{
   FragmentProgramManager::use_instance ().exec_mul (get_name (), get_name (), other.get_name ());
}



/*
==============================================================================
Name : operator *=
==============================================================================
*/

void  ShaderOperatable::operator *= (float scalar)
{
   FragmentProgramManager::use_instance ().exec_mul (get_name (), get_name (), scalar);
}



/*
==============================================================================
Name : sin
==============================================================================
*/

void  ShaderOperatable::sin (const ShaderOperatable &other)
{
   FragmentProgramManager::use_instance ().exec_sin (get_name (), other.get_name ());
}



/*
==============================================================================
Name : cos
==============================================================================
*/

void  ShaderOperatable::cos (const ShaderOperatable &other)
{
   FragmentProgramManager::use_instance ().exec_cos (get_name (), other.get_name ());
}



/*
==============================================================================
Name : sample
==============================================================================
*/

void  ShaderOperatable::sample (const ShaderTexture & texture, const ShaderFloat4 & tex_coords)
{
   FragmentProgramManager::use_instance ().exec_tex (get_name (), tex_coords.get_name (), texture.get_name ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
