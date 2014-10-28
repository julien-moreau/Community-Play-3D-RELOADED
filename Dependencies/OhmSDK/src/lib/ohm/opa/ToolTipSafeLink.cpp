/*****************************************************************************

        ToolTipSafeLink.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50207

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

#include "ohm/opa/ToolTipSafeLink.h"
#include "ohm/opa/BundleView.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ToolTipSafeLink::ToolTipSafeLink (void * object_ptr)
:  _object_ptr (object_ptr)
,  _bundle_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ToolTipSafeLink::~ToolTipSafeLink ()
{
   try
   {
      if (_bundle_ptr != 0)
      {
         _bundle_ptr->release_tool_tip (*this);
         
         _bundle_ptr = 0;
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : get_obj_ptr
==============================================================================
*/

void *   ToolTipSafeLink::get_obj_ptr () const
{
   return _object_ptr;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ToolTipSafeLink::bind (BundleView & bundle)
{
   assert (&bundle != 0);
   
   _bundle_ptr = &bundle;
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  ToolTipSafeLink::unbind (BundleView & bundle)
{
   assert (_bundle_ptr != 0);
   assert (&bundle != 0);
   
   _bundle_ptr = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
