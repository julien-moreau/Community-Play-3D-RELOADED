/*****************************************************************************

        ConductStack.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38817

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

#include "ohm/opal/private/ConductStack.h"
#include "ohm/opal/base/ConductBase.h"

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

ConductStack::ConductStack ()
:  _type_info_ptr_arr ()
,  _pos_flag_arr ()
{
   reset ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConductStack::~ConductStack ()
{
   assert (_pos_flag_arr.size () == 1);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

ConductStack & ConductStack::operator = (const ConductStack & other)
{
   _type_info_ptr_arr = other._type_info_ptr_arr;
   _pos_flag_arr = other._pos_flag_arr;
   
   return *this;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void  ConductStack::reset ()
{
   _type_info_ptr_arr.clear ();
   _pos_flag_arr.clear ();
   
   PosFlag pos_flag;
   pos_flag._pos = 0;
   pos_flag._spatial_flag = false;
   _pos_flag_arr.push_back (pos_flag);
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  ConductStack::save ()
{
   assert (_pos_flag_arr.size () > 0);
   
   size_t size = _type_info_ptr_arr.size ();
   
   bool spatial_flag = _pos_flag_arr.back ()._spatial_flag;
   
   PosFlag pos_flag;
   pos_flag._pos = size;
   pos_flag._spatial_flag = spatial_flag;

   _pos_flag_arr.push_back (pos_flag);
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  ConductStack::restore ()
{
   assert (_pos_flag_arr.size () > 1);
   
   size_t size = _pos_flag_arr.back ()._pos;
   
   _type_info_ptr_arr.resize (size);
   
   _pos_flag_arr.pop_back ();
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  ConductStack::invalidate ()
{
   assert (!_pos_flag_arr.empty ());
   
   if (_pos_flag_arr.back ()._spatial_flag)
   {
      TypeInfoPtrArr::iterator it = _type_info_ptr_arr.begin ();
      const TypeInfoPtrArr::iterator it_end = _type_info_ptr_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         TypeInfoPtr & type_info_ptr = *it;
         
         ConductBase * conduct_ptr = type_info_ptr._conduct_ptr;
         
         if (conduct_ptr->is_spatial ())
         {
            conduct_ptr->invalidate ();
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
