/*****************************************************************************

        ObjectRef.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/ObjectRef.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: declare
==============================================================================
*/

void ObjectRef::declare ()
{
   using namespace ohm::flip;
   
   ClassDescription <ObjectRef>::use ().set_name ("ohm.studio.ObjectRef");
   ClassDescription <ObjectRef>::use ().push_var_desc (&ObjectRef::_object_ref, "_object_ref");
   
   ClassDescManager::declare (ClassDescription <ObjectRef>::use ());
}



/*
==============================================================================
Name: ObjectRef
==============================================================================
*/

ObjectRef::ObjectRef (ohm::flip::DocumentBase & document)
:  ohm::flip::Object (document)
,  _object_ref (document)
{
}



/*
==============================================================================
Name: ~ObjectRef
==============================================================================
*/

ObjectRef::~ObjectRef ()
{
}



/*
==============================================================================
Name: operator <
==============================================================================
*/

bool ObjectRef::operator < (const ObjectRef & other)
{
   return get_object_ref () < other.get_object_ref ();
}



/*
==============================================================================
Name: ctor
==============================================================================
*/

void ObjectRef::ctor (ohm::flip::Args & args)
{
   args.pop (_object_ref);
}



/*
==============================================================================
Name: get_object_ref
==============================================================================
*/

ohm::flip::Ref ObjectRef::get_object_ref () const
{
   return _object_ref.get ();
}



/*
==============================================================================
Name: get_object_ptr
==============================================================================
*/

ohm::flip::Object * ObjectRef::get_object_ptr () const
{
   ohm::flip::Ref ref = 0;
   
   if (was_restored ())
   {
      ref = _object_ref.get_old_ref ();
   }
   else
   {
      ref = _object_ref.get ();
   }
   
   ohm::flip::Object * obj_ptr = use_document ().get_object (ref);
   
   return obj_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

