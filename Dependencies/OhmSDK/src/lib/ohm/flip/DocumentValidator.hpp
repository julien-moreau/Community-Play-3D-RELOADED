/*****************************************************************************

        DocumentValidator.hpp
        Copyright (c) 2009 Ohm Force

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



#if defined (ohm_flip_DocumentValidator_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentValidator code header.
#endif
#define  ohm_flip_DocumentValidator_CURRENT_CODEHEADER

#if ! defined (ohm_flip_DocumentValidator_CODEHEADER_INCLUDED)
#define  ohm_flip_DocumentValidator_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <algorithm>
#include <cassert>


namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
DocumentValidator <T>::DocumentValidator ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
DocumentValidator <T>::~DocumentValidator ()
{
}



/*
==============================================================================
Name : request_validate
==============================================================================
*/

template <class T>
void  DocumentValidator <T>::request_validate (ValidationReportWriter & report, archi::Int32 user_id, Object & obj)
{
   Object * obj_ptr = &obj;
   
   T * root_ptr = dynamic_cast <T *> (obj_ptr);
   assert (root_ptr != 0);
   
   validate (report, user_id, *root_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_DocumentValidator_CODEHEADER_INCLUDED

#undef ohm_flip_DocumentValidator_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
