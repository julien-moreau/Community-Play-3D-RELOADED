/*****************************************************************************

        TravelerSerialKiller.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63179

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

#include "ohm/flip/private/TravelerSerialKiller.h"
#include "ohm/flip/private/ArrayMaster.h"
#include "ohm/flip/private/BasicType.h"
#include "ohm/flip/private/CollectionMaster.h"
#include "ohm/flip/private/DocumentBase.h"

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

TravelerSerialKiller::TravelerSerialKiller (DocumentBase & document)
:  _object_manager (document.use_object_manager ())
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TravelerSerialKiller::~TravelerSerialKiller ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

void  TravelerSerialKiller::operator () (flip::Ref ref)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   assert (obj_ptr != 0);
   
   ArrayMaster * array_ptr = dynamic_cast <ArrayMaster *> (obj_ptr);
   
   if (array_ptr != 0)
   {
      array_ptr->clear ();
   }
   
   CollectionMaster * collection_ptr = dynamic_cast <CollectionMaster *> (obj_ptr);
   
   if (collection_ptr != 0)
   {
      collection_ptr->clear ();
   }
   
   BasicType * basic_ptr = dynamic_cast <BasicType *> (obj_ptr);
   
   if (basic_ptr != 0)
   {
      basic_ptr->backup ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
