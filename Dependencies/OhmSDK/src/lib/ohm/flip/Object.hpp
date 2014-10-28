/*****************************************************************************

        Object.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70515

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



#if defined (ohm_flip_Object_CURRENT_CODEHEADER)
   #error Recursive inclusion of Object code header.
#endif
#define  ohm_flip_Object_CURRENT_CODEHEADER

#if ! defined (ohm_flip_Object_CODEHEADER_INCLUDED)
#define  ohm_flip_Object_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/StdCtlMsg.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_parent
==============================================================================
*/

template <class T>
T &   Object::get_parent ()
{
   // client is calling the ref way too much early
   // it is only available (and all the ancestors)
   // at the 'init' stage
   // or this is root
   
   Object * parent_ptr = _parent_order.first;
   
   assert (parent_ptr != 0);
   
   T * ptr = dynamic_cast <T *> (parent_ptr);
   
   // bad casted type
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name : get_parent
==============================================================================
*/

template <class T>
const T &   Object::get_parent () const
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   // client is calling the ref way too much early
   // it is only available (and all the ancestors)
   // at the 'init' stage
   // or this is root
   
   const Object * parent_ptr = _parent_order.first;
   
   assert (parent_ptr != 0);
   
   const T * ptr = dynamic_cast <const T *> (parent_ptr);
   
   // bad casted type
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name : get_old_parent
==============================================================================
*/

template <class T>
T &   Object::get_old_parent ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   // client is calling the ref way too much early
   // it is only available (and all the ancestors)
   // at the 'init' stage
   // or this is root
   
   Object * parent_ptr = _old_parent_order.first;
   
   assert (parent_ptr != 0);
   
   T * ptr = dynamic_cast <T *> (parent_ptr);
   
   // bad casted type
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name : get_old_parent
==============================================================================
*/

template <class T>
const T &   Object::get_old_parent () const
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   // client is calling the ref way too much early
   // it is only available (and all the ancestors)
   // at the 'init' stage
   // or this is root
   
   const Object * parent_ptr = _old_parent_order.first;
   
   assert (parent_ptr != 0);
   
   const T * ptr = dynamic_cast <const T *> (parent_ptr);
   
   // bad casted type
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name : get_ancestor_ptr
Description :
   Returns a pointer to the first ancestor of type 'T' in the ancestor chain
   or the null pointer.
==============================================================================
*/

template <class T>
T *   Object::get_ancestor_ptr ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   Object * obj_ptr = this;
   
   T * ptr = 0;
   
   while (obj_ptr != 0)
   {
      ptr = dynamic_cast <T *> (obj_ptr);
      
      if (ptr != 0)
      {
         break;
      }
      
      obj_ptr = obj_ptr->_parent_order.first;
   }
   
   return ptr;
}



/*
==============================================================================
Name : get_ancestor_ptr
Description :
   Returns a pointer to the first ancestor of type 'T' in the ancestor chain
   or the null pointer.
==============================================================================
*/

template <class T>
const T *   Object::get_ancestor_ptr () const
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   const Object * obj_ptr = this;
   
   const T * ptr = 0;
   
   while (obj_ptr != 0)
   {
      ptr = dynamic_cast <const T *> (obj_ptr);
      
      if (ptr != 0)
      {
         break;
      }
      
      obj_ptr = obj_ptr->_parent_order.first;
   }
   
   return ptr;
}



/*
==============================================================================
Name : get_ancestor
Description :
   Find the first ancestor of type 'T' in the ancestor chain.
==============================================================================
*/

template <class T>
T &   Object::get_ancestor ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   T * ptr = get_ancestor_ptr <T> ();
      
   // no 'T' in ancestor chain
   assert (ptr != 0);

   return *ptr;
}



/*
==============================================================================
Name : get_ancestor
Description :
   Find the first ancestor of type 'T' in the ancestor chain.
==============================================================================
*/

template <class T>
const T &   Object::get_ancestor () const
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   const T * ptr = get_ancestor_ptr <T> ();
      
   // no 'T' in ancestor chain
   assert (ptr != 0);

   return *ptr;
}



/*
==============================================================================
Name : get_old_ancestor_ptr
Description :
   Returns a pointer to the first ancestor of type 'T' in the ancestor chain
   or the null pointer.
==============================================================================
*/

template <class T>
T *   Object::get_old_ancestor_ptr ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   Object * obj_ptr = this;
   
   T * ptr = 0;
   
   while (obj_ptr != 0)
   {
      ptr = dynamic_cast <T *> (obj_ptr);
      
      if (ptr != 0)
      {
         break;
      }
      
      obj_ptr = obj_ptr->_old_parent_order.first;
   }
   
   return ptr;
}



/*
==============================================================================
Name : get_old_ancestor
Description :
   Find the first ancestor of type 'T' in the ancestor chain.
==============================================================================
*/

template <class T>
T &   Object::get_old_ancestor ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   T * ptr = get_old_ancestor_ptr <T> ();
   
   // no 'T' in ancestor chain
   assert (ptr != 0);

   return *ptr;
}



/*
==============================================================================
Name : get_ancestor_level_ptr
==============================================================================
*/

template <class T>
T *   Object::get_ancestor_level_ptr (int level)
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   // client is calling the ref way too much early
   // it is only available (and all the ancestors)
   // at the 'init' stage
   // or this is root
   
   Object * obj_ptr = this;
   
   for (int i = 0 ; i < level ; ++i)
   {
      obj_ptr = obj_ptr->_parent_order.first;
   }
   
   assert (obj_ptr != 0);

   T * ptr = dynamic_cast <T *> (obj_ptr);
   
   return ptr;
}



/*
==============================================================================
Name : get_ancestor_level
==============================================================================
*/

template <class T>
T &   Object::get_ancestor_level (int level)
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   T * ptr = get_ancestor_level_ptr <T> (level);
   
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name : get_old_ancestor_level_ptr
==============================================================================
*/

template <class T>
T *   Object::get_old_ancestor_level_ptr (int level)
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   // client is calling the ref way too much early
   // it is only available (and all the ancestors)
   // at the 'init' stage
   // or this is root
   
   Object * obj_ptr = this;
   
   for (int i = 0 ; i < level ; ++i)
   {
      obj_ptr = obj_ptr->_old_parent_order.first;
   }
   
   assert (obj_ptr != 0);

   T * ptr = dynamic_cast <T *> (obj_ptr);
   
   return ptr;
}



/*
==============================================================================
Name : get_old_ancestor_level
==============================================================================
*/

template <class T>
T &   Object::get_old_ancestor_level (int level)
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   T * ptr = get_old_ancestor_level_ptr <T> (level);
   
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name: is_ancestor
==============================================================================
*/

template <class T>
bool Object::is_ancestor (Object & obj) const
{
   T * obj_ptr = obj.get_ancestor_ptr <T> ();
   
   const T * self_ptr = dynamic_cast <const T*> (this);
   
   return (self_ptr == obj_ptr);
}



/*
==============================================================================
Name : reset_data
Note :
   ptr might be null
==============================================================================
*/

template <class T>
void  Object::reset_data (T * ptr)
{
   return _id_ptr_map.set <T> (ptr);
}



/*
==============================================================================
Name : get_data
==============================================================================
*/

template <class T>
T *   Object::get_data () const
{
   return _id_ptr_map.get <T> ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_Object_CODEHEADER_INCLUDED

#undef ohm_flip_Object_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
