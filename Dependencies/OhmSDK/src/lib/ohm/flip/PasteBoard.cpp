/*****************************************************************************

        PasteBoard.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70444

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

#include "ohm/flip/PasteBoard.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/ClassDescriptionMaster.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/ObjectRefMaster.h"

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

PasteBoard::~PasteBoard ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

PasteBoard::PasteBoard ()
{
   clear ();
}



/*
==============================================================================
Name : begin
==============================================================================
*/

PasteBoard::iterator PasteBoard::begin ()
{
   return _mold_sptr_list.begin ();
}



/*
==============================================================================
Name : end
==============================================================================
*/

PasteBoard::iterator PasteBoard::end ()
{
   return _mold_sptr_list.end ();
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  PasteBoard::empty () const
{
   return _mold_sptr_list.empty ();
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  PasteBoard::clear ()
{
   _mold_sptr_list.clear ();
}



/*
==============================================================================
Name : add
==============================================================================
*/

ObjectMold & PasteBoard::add (Object & object)
{
   ObjectMoldSPtr object_mold_sptr (new ObjectMold (object));
   _mold_sptr_list.push_back (object_mold_sptr);
   
   return *object_mold_sptr;
}



/*
==============================================================================
Name : parse
==============================================================================
*/

/*void   PasteBoard::paste ()
{
   if (type == Type_COLLECTION_INSERT)
   {
      it = parent.insert (cdm_ptr);
      ref = it.ref ();
   }
   else if (type == Type_SET_INT64)
   {
      Int64 * val_ptr = ;
      val_ptr->operator = (val);
   }
}*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
