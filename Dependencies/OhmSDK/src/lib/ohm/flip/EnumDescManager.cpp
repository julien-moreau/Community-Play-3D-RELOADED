/*****************************************************************************

        EnumDescManager.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62509

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

#include "ohm/flip/EnumDescManager.h"
#include "ohm/flip/EnumDescription.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

EnumDescManager::~EnumDescManager ()
{
}



/*
==============================================================================
Name : declare
==============================================================================
*/

void  EnumDescManager::declare (EnumDescriptionMaster & enum_description)
{
   use_instance ().do_declare (enum_description);
}



/*
==============================================================================
Name : use_instance
==============================================================================
*/

EnumDescManager & EnumDescManager::use_instance ()
{
   static EnumDescManager instance;
   
   return instance;
}



/*
==============================================================================
Name : get
==============================================================================
*/

EnumDescriptionMaster & EnumDescManager::get (int enum_id)
{
   assert (enum_id > 0);
   --enum_id;
   assert (size_t (enum_id) < _enum_id_enum_desc_arr.size ());
   
   return *_enum_id_enum_desc_arr [enum_id];
}



/*
==============================================================================
Name : find
Note :
   We don't need a fast function, since this is only used to resolve
   class names from stored files to class desc
==============================================================================
*/

EnumDescriptionMaster * EnumDescManager::find (const std::string & enum_name)
{
   for (size_t i = 0 ; i < _enum_id_enum_desc_arr.size () ; ++i)
   {
      EnumDescriptionMaster * cdm_ptr = _enum_id_enum_desc_arr [i];
      
      if (cdm_ptr->get_name () == enum_name)
      {
         return cdm_ptr;
      }
   }
   
   return 0;
}



/*
==============================================================================
Name : post_check
==============================================================================
*/

void  EnumDescManager::post_check ()
{
   for (size_t i = 0 ; i < _enum_id_enum_desc_arr.size () ; ++i)
   {
      EnumDescriptionMaster * cdm_ptr = _enum_id_enum_desc_arr [i];
      
      cdm_ptr->post_check ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

EnumDescManager::EnumDescManager ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_declare
==============================================================================
*/

void  EnumDescManager::do_declare (EnumDescriptionMaster & enum_description)
{
   enum_description.set_enum_id (_enum_id_enum_desc_arr.size () + 1);
   
   _enum_id_enum_desc_arr.push_back (&enum_description);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
