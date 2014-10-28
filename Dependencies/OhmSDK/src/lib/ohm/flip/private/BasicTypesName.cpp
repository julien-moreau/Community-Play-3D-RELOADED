/*****************************************************************************

        BasicTypesName.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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

#include "ohm/flip/private/BasicTypesName.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

bool BasicTypesName::is_basic_type (const char * name_0)
{
   bool basic_type_flag = false;
   
   basic_type_flag |= (name_0 == _object_ref_0);
   basic_type_flag |= (name_0 == _array_0);
   basic_type_flag |= (name_0 == _collection_0);
   basic_type_flag |= (name_0 == _bool_0);
   basic_type_flag |= (name_0 == _int64_0);
   basic_type_flag |= (name_0 == _enum_0);
   basic_type_flag |= (name_0 == _float64_0);
   basic_type_flag |= (name_0 == _blob_0);
   basic_type_flag |= (name_0 == _cue_0);
   
   return basic_type_flag;
}
   


const char * BasicTypesName::_object_ref_0 = "ohm.flip.ObjectRef";
const char * BasicTypesName::_array_0 = "ohm.flip.Array";
const char * BasicTypesName::_collection_0 = "ohm.flip.Collection";
const char * BasicTypesName::_bool_0 = "ohm.flip.Bool";
const char * BasicTypesName::_int64_0 = "ohm.flip.Int64";
const char * BasicTypesName::_enum_0 = "ohm.flip.Enum";
const char * BasicTypesName::_float64_0 = "ohm.flip.Float64";
const char * BasicTypesName::_blob_0 = "ohm.flip.Blob";
const char * BasicTypesName::_cue_0 = "ohm.flip.Cue";



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
