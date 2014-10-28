/*****************************************************************************

        ObjectManager.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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

#include "ohm/flip/private/ObjectManager.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/ObjectTravelAgency.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/TravelerObjectInit.h"
#include "ohm/flip/private/TravelerObjectRestore.h"
#include "ohm/flip/private/TravelerSlaughterWitness.h"
#include "ohm/flip/private/TravelerSerialKiller.h"

#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/txt/fnc.h"
#include "ohm/util/trace.h"

#if defined (ohm_flip_EXTENDED_DEBUG_INFO)
#include "ohm/txt/out/PrintNa.h"
#endif

#include <typeinfo>

#include <cassert>



#undef OHM_FLIP_OBJECT_MANAGER_DEBUG_FLAG
#undef OHM_FLIP_OBJECT_MANAGER_BIN_CHECK_FLAG

#if defined (OHM_FLIP_OBJECT_MANAGER_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_OBJECT_MANAGER_DEBUG_FLAG is defined in shipping level code !
   #endif
#endif

#if defined (OHM_FLIP_OBJECT_MANAGER_BIN_CHECK_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_OBJECT_MANAGER_BIN_CHECK_FLAG is defined in shipping level code !
   #endif
#endif

#if defined (OHM_FLIP_OBJECT_MANAGER_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_OBJECT_MANAGER_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define om_TRACE(msg_0) ohm_util_TRACE_DEBUG(msg_0)
   #define om_TRACE1(fmt_0, arg1)   ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define om_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define om_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define om_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define om_TRACE(msg_0) (void (0))
   #define om_TRACE1(fmt_0, arg1)   (void (0))
   #define om_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define om_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define om_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

ObjectManager::iterator::iterator ()
:  _it ()
{
}



/*
==============================================================================
Name : iterator::dtor
==============================================================================
*/

ObjectManager::iterator::~iterator ()
{
}



/*
==============================================================================
Name : iterator::operator ++ (pre)
==============================================================================
*/

ObjectManager::iterator &  ObjectManager::iterator::operator ++ ()
{
   ++_it;
   
   return *this;
}



/*
==============================================================================
Name : iterator::operator !=
==============================================================================
*/

bool  ObjectManager::iterator::operator != (const iterator & other) const
{
   return _it != other._it;
}



/*
==============================================================================
Name : iterator::get_ref
==============================================================================
*/

flip::Ref   ObjectManager::iterator::get_ref () const
{
   return _it->first;
}



/*
==============================================================================
Name : iterator::get_object_ptr
==============================================================================
*/

Object * ObjectManager::iterator::get_object_ptr () const
{
   return _it->second._object_ptr;
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ObjectManager::ObjectManager (DocumentBase & document, archi::Int32 user_id, archi::Int32 thread_id)
:  _document (document)
,  _user_id (user_id)
,  _thread_id (thread_id)

,  _cur_object_ref (0)
,  _in_create_flag (false)

,  _ref_info_pool ()

,  _ref_object_info_map ()
,  _recycle_bin ()

,  _root_ptr (0)
{
   _cur_object_ref = SeedGenerator::get (user_id, thread_id);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ObjectManager::~ObjectManager ()
{
}



/*
==============================================================================
Name : assign_ref
Description :
   Called from Object::ctor exclusively, assign a ref for Object 'ptr' and
   give it its 'parent_ptr'
Note :
   The ref cannot already be RESIDENT
   The ref cannot also be RECYCLED, as in this case the object would have
   been unrecycled using 'restore_bin_from_pool'
==============================================================================
*/

flip::Ref   ObjectManager::assign_ref (Object * & parent_ptr, ClassDescriptionMaster * & cdm_ptr, Object * ptr)
{
   assert (_in_create_flag);
   
   // did you forget to declare a member or inheritance ?
   assert (!_ref_info_pool.empty ());
   
   RefInfo ref_info = _ref_info_pool.front ();
   _ref_info_pool.pop_front ();
   
   om_TRACE2 ("OBJ NEW %1m16rw0X% %2%", ref_info._ref, ptr);
   
   // not resident nor recycled
   assert (_ref_object_info_map.find (ref_info._ref) == _ref_object_info_map.end ());
   
   ObjectInfo object_info;
   object_info._object_ptr = ptr;
   object_info._state = ObjectState_RESIDENT;
   
   _ref_object_info_map [ref_info._ref] = object_info;
   
   if (ref_info._parent_ref != 0)
   {
      assert (_ref_object_info_map.find (ref_info._parent_ref) != _ref_object_info_map.end ());
      parent_ptr = _ref_object_info_map [ref_info._parent_ref]._object_ptr;
   }
   else
   {
      parent_ptr = 0;
   }
   
   cdm_ptr = ref_info._cdm_ptr;
   
   assert (ref_info._ref != 0);
   
   if (ref_info._ref == 1)
   {
      _root_ptr = ptr;
   }
   
   return ref_info._ref;
}



/*
==============================================================================
Name : unassign_ref
Description :
   Called from Object::ctor exclusively, deassign the ref. Can only occur
   when the object is is RECYCLED state.
Source :
   empty_recycle_bin
      cdm.release
         ~T (which will cascade dtor for its compounds, using C++)
            unassign_ref (for each object)
==============================================================================
*/

void  ObjectManager::unassign_ref (flip::Ref ref)
{
   assert (ref != 0);
   
   om_TRACE1 ("OBJ DEL %1m16rw0X%", ref);
   
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   assert (it != _ref_object_info_map.end ());

   ObjectInfo & object_info = it->second;
   assert (object_info._state == ObjectState_RECYCLED);
   
   _ref_object_info_map.erase (it);
}



/*
==============================================================================
Name : get_planned_ref
Description :
   Returns the next free unassigned ref
==============================================================================
*/

flip::Ref   ObjectManager::get_planned_ref ()
{
   assert (_ref_object_info_map.find (_cur_object_ref) == _ref_object_info_map.end ());
   
   return _cur_object_ref;
}



/*
==============================================================================
Name : create_root
Source :
   Only when creating skeleton file.
Description :
   Create root object by finding its cdm, and filling default params.
   The root class must be unique. If it is not, it will assert and return 0.
   If the root class is not unique, then the client must use custom cdm
   version of this method.
==============================================================================
*/

Object * ObjectManager::create_root ()
{
   ClassDescriptionMaster * cdm_ptr = ClassDescManager::use_instance ().find_root ();
   
   // either root class does not exists or is not unique
   
   if (cdm_ptr == 0)
   {
      ohm_util_TRACE_PANIC ("If there are multiple root classes, use the cdm version of this method");
   }
   
   assert (cdm_ptr != 0);
   
   Object * object_ptr = 0;
   
   if (cdm_ptr != 0)
   {
      object_ptr = create_root (*cdm_ptr);
   }
   
   return object_ptr;
}



/*
==============================================================================
Name : create_root
Source :
   Only when creating skeleton file.
==============================================================================
*/

Object * ObjectManager::create_root (ClassDescriptionMaster & cdm)
{
   bool root_flag = cdm.is_root ();
   
   if (!root_flag)
   {
      ohm_util_TRACE_PANIC ("Template parameter is not a root class");
   }
   
   assert (root_flag);
   
   Object * object_ptr = create (
      cdm,
      0, // parent
      1  // root ref
   );
   
   TravelerObjectInit traveler (_document);
   
   ObjectTravelAgency travel_agency (_document);
   travel_agency.travel (
      object_ptr->get_ref (),
      ObjectTravelAgency::Flag_DEPTH_FIRST,
      traveler
   );
   
   return object_ptr;
}



/*
==============================================================================
Name : release_root
Source :
   When preparing to dtor the document.
==============================================================================
*/

void  ObjectManager::release_root ()
{
   assert (_root_ptr != 0);
   
   Ref root_ref = _root_ptr->get_ref ();
   assert (root_ref != 0);
   
   {
      TravelerSerialKiller traveler (_document);
   
      ObjectTravelAgency travel_agency (_document);
      travel_agency.travel (
         root_ref,
         ObjectTravelAgency::Flag_REVERSE
            | ObjectTravelAgency::Flag_FOLLOW_CONTAINERS,
         traveler
      );
   }
   
   {
      TravelerObjectRestore traveler (_document);
      
      ObjectTravelAgency travel_agency (_document);
      travel_agency.travel (
         root_ref,
         ObjectTravelAgency::Flag_REVERSE
            | ObjectTravelAgency::Flag_DEPTH_FIRST,
         traveler
      );
   }
   
   recycle (_root_ptr);
   
   // don't set _root_ptr to 0, we need it to be present
   // for the observers
}



/*
==============================================================================
Name : check_creation
Description :
   Check that creating an object of class 'cdm' at master 'ref' is possible.
   It mainly checks that the future formed object won't have refs that would
   collide with already assigned refs.
==============================================================================
*/

int   ObjectManager::check_creation (ClassDescriptionMaster & cdm, flip::Ref ref, bool tolerant_flag)
{
   if (ref == 0) return Err_VM_BAD_ACCESS;
   
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   
   if (it != _ref_object_info_map.end ())
   {
      ObjectInfo & object_info = it->second;
      
      if (object_info._state == ObjectState_RESIDENT)
      {
         assert (tolerant_flag);
         return Err_VM_REF_EXISTS;
      }
   }
   
   // it was either never created, or it is recycled
   
   int err = check_ref_info_pool (cdm, ref, tolerant_flag);
      
   return err;
}


   
/*
==============================================================================
Name : create
Description :
   This is similar to the in-place operator new. An object is allocated but
   is yet assigned a special ref already :
   - the ref is already known since the document is being read
   - the ref is the "planned ref" in the case of a container
   - the ref is already present
   This occurs only for the "master object" (that is not for a compound
   object)
==============================================================================
*/

Object * ObjectManager::create (ClassDescriptionMaster & cdm, Object * parent_ptr, flip::Ref ref)
{
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   
   Object * object_ptr = 0;
   
   bool in_recycle_bin_flag = false;
   
   if (it != _ref_object_info_map.end ())
   {
      ObjectInfo & object_info = it->second;
      assert (object_info._state == ObjectState_RECYCLED);
      
      in_recycle_bin_flag = true;
   }
   
   make_ref_info_pool (cdm, ref, parent_ptr);
   
   if (!in_recycle_bin_flag)
   {
      archi::UByte * ptr = new archi::UByte [cdm.get_size_of ()];
      
      om_TRACE1 ("OBJ CRE %1%", ptr);
      
      _in_create_flag = true;
      object_ptr = cdm.create (ptr, _document);
      _in_create_flag = false;
      
      assert (_ref_info_pool.empty ());
      
      if (cdm.is_root ())
      {
         _root_ptr = object_ptr;
      }
   }
   else
   {
      object_ptr = it->second._object_ptr;
      assert (object_ptr != 0);
      assert (object_ptr->get_ref () == ref);
      assert (!cdm.is_root ());
      
      // make some checks to ensure that the object is getting
      // correctly reused from the recycle bin
      
      ObjectPtrList::iterator rit = find_recycle_bin (object_ptr);
      
      assert (rit != _recycle_bin.end ());
      assert (&cdm == &object_ptr->get_cdm ());
      if (parent_ptr != &object_ptr->get_parent <Object> ())
      {
         object_ptr->change_parent (parent_ptr);
      }
      
      _recycle_bin.erase (rit);
      
      // use the ref pool to transfer from bin to object map
      
      restore_bin_from_pool ();
   }
   
   return object_ptr;
}


   
/*
==============================================================================
Name : create (with arguments)
Description :
   This is similar to the in-place operator new. An object is allocated but
   is yet assigned a special ref already :
   - the ref is already known since the document is being read
   - the ref is the "planned ref" in the case of a container
   - the ref is already present
   This occurs only for the "master object" (that is not for a compound
   object)
==============================================================================
*/

Object * ObjectManager::create (ClassDescriptionMaster & cdm, Object * parent_ptr, flip::Ref ref, Args & args)
{
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   
   Object * object_ptr = 0;
   
   // not possible for arg version
   assert (it == _ref_object_info_map.end ());
   
   make_ref_info_pool (cdm, ref, parent_ptr);
   
   archi::UByte * ptr = new archi::UByte [cdm.get_size_of ()];
   
   om_TRACE1 ("OBJ CRE %1%", ptr);
   
   _in_create_flag = true;
   object_ptr = cdm.create (ptr, _document, args);
   _in_create_flag = false;
   
   assert (_ref_info_pool.empty ());
   
   if (cdm.is_root ())
   {
      _root_ptr = object_ptr;
   }
   
   return object_ptr;
}


   
/*
==============================================================================
Name : can_recycle
Description :
   Check if we can recycle this object.
   This actually checks to see if all containers are empty.
==============================================================================
*/

bool  ObjectManager::can_recycle (Object * object_ptr)
{
   flip::Ref ref = object_ptr->get_ref ();
   
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   
   assert (it != _ref_object_info_map.end ());
   
   ObjectInfo & object_info = it->second;
   assert (object_info._state == ObjectState_RESIDENT);
   
   // check if containers are empty
   TravelerSlaughterWitness traveler (_document);
   
   // logically we don't really need it to be reverse...
   
   ObjectTravelAgency travel_agency (_document);
   travel_agency.travel (
      object_ptr->get_ref (),
      ObjectTravelAgency::Flag_REVERSE
         | ObjectTravelAgency::Flag_FOLLOW_CONTAINERS,
      traveler
   );
   
   bool can_release_flag = traveler.is_total_mass_murder ();
   
   return can_release_flag;
}


   
/*
==============================================================================
Name : recycle
Description :
   Do the actual recycle of an object.
   Objects are not erased immediatly, there are marked as RECYCLED, for a
   later real removal after 'POST'.
   This let us ensure that :
   - in 'POST' client still have access to the old object
   - multiple insert/erase in a transaction are fast operations
Note :
   Every assigned ref in _ref_object_info_map corresponding to the master
   object 'object_ptr' are marked as recycled.
   Only the master object is pushed in the recycle bin.
   The order in which the object are recycled is important.
==============================================================================
*/

void  ObjectManager::recycle (Object * object_ptr)
{
   flip::Ref ref = object_ptr->get_ref ();
   
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   assert (it != _ref_object_info_map.end ());
   
   om_TRACE1 ("OBJ REC %1%", object_ptr);
   
   TravelerObjectCollector traveler;
   ObjectTravelAgency travel_agency (_document);
   travel_agency.travel (
      object_ptr->get_ref (),
      0,
      traveler
   );
   
   for (size_t i = 0 ; i < traveler._ref_arr.size () ; ++i)
   {
      flip::Ref sub_ref = traveler._ref_arr [i];
      if (i == 0) assert (sub_ref == ref);
      
      RefObjectInfoMap::iterator sit = _ref_object_info_map.find (sub_ref);
      assert (sit != _ref_object_info_map.end ());
      
      ObjectInfo & object_info = sit->second;
      assert (object_info._state == ObjectState_RESIDENT);
      
      object_info._state = ObjectState_RECYCLED;
   }
   
   // recycle bin must keep the order in which the object were recycled,
   // to ensure that will be released in the same order
   
   _recycle_bin.push_back (object_ptr);
}



/*
==============================================================================
Name : empty_recycle_bin
Description :
   after 'POST', actually erase the object that were in the recycle bin.
   This will dtor the master object, which itself through cdm.release will
   unassign every ref from the master object.
==============================================================================
*/

void  ObjectManager::empty_recycle_bin ()
{
   // recycle bin must keep the order in which the object were recycled,
   // to ensure that will be released in the same order
   
   ObjectPtrList::iterator it = _recycle_bin.begin ();
   const ObjectPtrList::iterator it_end = _recycle_bin.end ();
   
   for (; it != it_end ; ++it)
   {
      Object * object_ptr = *it;
      
      assert (!object_ptr->is_inited ());
      
      ClassDescriptionMaster & cdm = object_ptr->get_cdm ();
      
      om_TRACE1 ("OBJ REL %1%", object_ptr);
      
      cdm.release (object_ptr);
      
      archi::UByte * ptr = reinterpret_cast <archi::UByte *> (object_ptr);
      
      // non-aligned ptr problem ?
      // check that when using multiple inheritance, Object inheritance
      // comes *first*
      delete [] ptr;
      
      if (cdm.is_root ())
      {
         _root_ptr = 0;
      }
   }
   
   _recycle_bin.clear ();
   
   // all object left must be resident

#if defined (OHM_FLIP_OBJECT_MANAGER_BIN_CHECK_FLAG)
   {
      RefObjectInfoMap::const_iterator it = _ref_object_info_map.begin ();
      const RefObjectInfoMap::const_iterator it_end = _ref_object_info_map.end ();
      
      for (; it != it_end ; ++it)
      {
         const ObjectInfo & object_info = it->second;
         assert (object_info._state == ObjectState_RESIDENT);
      }
   }
#endif   
}


   
/*
==============================================================================
Name : is_recycle_bin_empty
==============================================================================
*/

bool  ObjectManager::is_recycle_bin_empty ()
{
   bool empty_flag = _recycle_bin.empty ();
   
#if defined (OHM_FLIP_OBJECT_MANAGER_BIN_CHECK_FLAG)
   if (empty_flag)
   {
      // all object left must be resident

      {
         RefObjectInfoMap::const_iterator it = _ref_object_info_map.begin ();
         const RefObjectInfoMap::const_iterator it_end = _ref_object_info_map.end ();
         
         for (; it != it_end ; ++it)
         {
            const ObjectInfo & object_info = it->second;
            assert (object_info._state == ObjectState_RESIDENT);
         }
      }
   }
#endif

   return empty_flag;
}



/*
==============================================================================
Name : get_object
Description :
   In 'POST' returns the possible recycled or resident object matching 'ref'
   Otherwise returns the possible *resident* object matching 'ref'
==============================================================================
*/

Object * ObjectManager::get_object (flip::Ref ref)
{
   Object * ret_val = 0;
   
   if (ref != 0)
   {
      RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
      
      if (it != _ref_object_info_map.end ())
      {
         ObjectInfo & object_info = it->second;
         
         if (object_info._state == ObjectState_RESIDENT)
         {
            ret_val = object_info._object_ptr;
         }
         else if (
            (object_info._state == ObjectState_RECYCLED)
            && (_document.is_in_post ())
            )
         {
            // in POST, also include objects from the recycle bin
            
            ret_val = object_info._object_ptr;
         }
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get_root
Description :
   Returns the root object
==============================================================================
*/

Object * ObjectManager::get_root ()
{
   // root can be null
   
   return _root_ptr;
}



/*
==============================================================================
Name : begin
Description :
   beging iterator to iterate through ObjectManager objects.
   Must not be in 'POST'
==============================================================================
*/

ObjectManager::iterator ObjectManager::begin ()
{
   assert (!_document.is_in_post ());
   
   iterator it;
   it._it = _ref_object_info_map.begin ();
   
   return it;
}



/*
==============================================================================
Name : end
Description :
   end iterator to iterate through ObjectManager objects.
   Must not be in 'POST'
==============================================================================
*/

ObjectManager::iterator ObjectManager::end ()
{
   assert (!_document.is_in_post ());
   
   iterator it;
   it._it = _ref_object_info_map.end ();
   
   return it;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  ObjectManager::trace () const
{
#if ! defined (ohm_flip_SERVER)
   ohm_util_TRACE_INFO ("*** ObjectManager ***");
   
   RefObjectInfoMap::const_iterator it = _ref_object_info_map.begin ();
   const RefObjectInfoMap::const_iterator it_end = _ref_object_info_map.end ();
   
   for (; it != it_end ; ++it)
   {
//    flip::Ref ref = it->first;
      
      const ObjectInfo & object_info = it->second;
      
      Object * object_ptr = object_info._object_ptr;
      
      ohm_util_TRACE_INFO1 ("  OBJ %1m16rw0X%", object_ptr->get_ref ());
      
      ohm_util_TRACE_INDENT (2);
      
      object_ptr->trace ();
      
      ohm_util_TRACE_INDENT (-2);
   }
   
   trace_recycle_bin ();
#endif
}



/*
==============================================================================
Name : trace_recycle_bin
==============================================================================
*/

void  ObjectManager::trace_recycle_bin () const
{
#if ! defined (ohm_flip_SERVER)
   ohm_util_TRACE_INFO ("*** ObjectManager Recycle Bin ***");
   
   ObjectPtrList::const_iterator it = _recycle_bin.begin ();
   const ObjectPtrList::const_iterator it_end = _recycle_bin.end ();
   
   for (; it != it_end ; ++it)
   {
      Object * object_ptr = *it;
      
      ClassDescriptionMaster & cdm = object_ptr->get_cdm ();
      
      ohm_util_TRACE_INFO1 ("  OBJ %1m16rw0X%", object_ptr->get_ref ());
      ohm_util_TRACE_INFO1 ("    %1%", cdm.get_name ());
   }
#endif
}


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : find_recycle_bin
Description :
   Find a master object in the recycle bin.
==============================================================================
*/

ObjectManager::ObjectPtrList::iterator ObjectManager::find_recycle_bin (Object * object_ptr)
{
   // long linear search
   
   ObjectPtrList::iterator it = _recycle_bin.begin ();
   const ObjectPtrList::iterator it_end = _recycle_bin.end ();
   
   for (; it != it_end ; ++it)
   {
      if (*it == object_ptr)
      {
         break;
      }
   }
   
   return it;
}



/*
==============================================================================
Name : restore_bin_from_pool
Description :
   Restore to RESIDENT objects for which the master object was in the recycle
   bin. The restored objects must have been marked as RECYCLED.
==============================================================================
*/

void  ObjectManager::restore_bin_from_pool ()
{
   RefInfoList::const_iterator it = _ref_info_pool.begin ();
   const RefInfoList::const_iterator it_end = _ref_info_pool.end ();
   
   for (; it != it_end ; ++it)
   {
      const RefInfo & ref_info = *it;
      flip::Ref sub_ref = ref_info._ref;
      
      RefObjectInfoMap::iterator sit = _ref_object_info_map.find (sub_ref);
      assert (sit != _ref_object_info_map.end ());
      
      ObjectInfo & object_info = sit->second;
      assert (object_info._state == ObjectState_RECYCLED);
      
      object_info._state = ObjectState_RESIDENT;
   }
   
   _ref_info_pool.clear ();
}



/*
==============================================================================
Name : make_ref_info_pool
Description :
   Make a ref pool for 'cdm'.
   The ref info includes :
   - target ref
   - parent of target ref
   - cdm of target ref
==============================================================================
*/

void  ObjectManager::make_ref_info_pool (ClassDescriptionMaster & cdm, flip::Ref ref, Object * parent_ptr)
{
   // now we are going to parse the cdm from ref to build a table
   // that each assign_ref will use to bind a ref to a ptr and its cdm
   
   assert (ref != 0);
   assert (!_in_create_flag);
   
   bool root_flag = cdm.is_root ();
   assert ((parent_ptr == 0) || !root_flag);
   
   flip::Ref parent_ref = 0;
   
   if (parent_ptr != 0)
   {
      parent_ref = parent_ptr->get_ref ();
   }
   
   make_ref_info_pool_recur (&cdm, ref, parent_ref);
   
   assert (!_ref_info_pool.empty ());
   
   bool update_flag
      = (SeedGenerator::to_user_id (ref) == _user_id)
      && (SeedGenerator::to_thread_id (ref) == _thread_id)
   ;
   
   if (update_flag)
   {
      flip::Ref next_ref = _ref_info_pool.back ()._ref + 1;
      
      // next_ref might be the reconstruction of an old object
      
      if (next_ref > _cur_object_ref)
      {
         _cur_object_ref = next_ref;
      }
   }
}
   
   

/*
==============================================================================
Name : make_ref_info_pool_recur
==============================================================================
*/

void  ObjectManager::make_ref_info_pool_recur (ClassDescriptionMaster * cdm_ptr, flip::Ref & ref, flip::Ref parent_ref)
{
   assert (ref != 0);
   
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   
   if (it != _ref_object_info_map.end ())
   {
      ObjectInfo & object_info = it->second;
      
      assert (object_info._state == ObjectState_RECYCLED);
   }
   
   RefInfo ref_info;
   ref_info._ref = ref;
   ref_info._parent_ref = parent_ref;
   ref_info._cdm_ptr = cdm_ptr;
   
   _ref_info_pool.push_back (ref_info);
   
   flip::Ref sub_parent_ref = ref;
   ++ref;
   
   int ref_class_id = cdm_ptr->get_class_id ();
   
   // did you forget to declare the class ?
   assert (ref_class_id > 0);
   
   // do not recurse in arrays or object refs
   // and basic types don't have internal refs)
   
   if (ref_class_id >= ClassDescManager::NBR_BasicTypeId_TOTAL)
   {
      const ClassDescriptionMaster::VarInfoArr & var_info_arr
         = cdm_ptr->get_var_info_arr ();
      
      ClassDescriptionMaster::VarInfoArr::const_iterator it = var_info_arr.begin ();
      const ClassDescriptionMaster::VarInfoArr::const_iterator it_end = var_info_arr.end ();
      
      flip::Ref & sub_ref = ref;
      
      for (; it != it_end ; ++it)
      {
         ClassDescriptionMaster * sub_cdm_ptr = it->_cdm_ptr;
         
         make_ref_info_pool_recur (sub_cdm_ptr, sub_ref, sub_parent_ref);
      }
   }
}



/*
==============================================================================
Name : check_ref_info_pool
==============================================================================
*/

int   ObjectManager::check_ref_info_pool (ClassDescriptionMaster & cdm, flip::Ref ref, bool tolerant_flag)
{
   // now we are going to parse the cdm from ref to build a table
   // that each assign_ref will use to bind a ref to a ptr and its cdm
   
   if (ref == 0)
   {
      assert (tolerant_flag);
      return Err_VM_BAD_ACCESS;
   }
   
   assert (!_in_create_flag);
   
   flip::Ref parent_ref = 0;
   
   int err = check_ref_info_pool_recur (&cdm, ref, parent_ref, tolerant_flag);
   
   return err;
}
   
   

/*
==============================================================================
Name : check_ref_info_pool_recur
==============================================================================
*/

int   ObjectManager::check_ref_info_pool_recur (ClassDescriptionMaster * cdm_ptr, flip::Ref & ref, flip::Ref parent_ref, bool tolerant_flag)
{
   if (ref == 0)
   {
      assert (tolerant_flag);
      return Err_VM_BAD_ACCESS;
   }
   
   RefObjectInfoMap::iterator it = _ref_object_info_map.find (ref);
   
   if (it != _ref_object_info_map.end ())
   {
      ObjectInfo & object_info = it->second;
      
      if (object_info._state == ObjectState_RESIDENT)
      {
         assert (tolerant_flag);
         return Err_VM_REF_EXISTS;
      }
   }
   
   flip::Ref sub_parent_ref = ref;
   ++ref;
   
   int ref_class_id = cdm_ptr->get_class_id ();
   
   // did you forget to declare the class ?
   if (ref_class_id <= 0)
   {
      assert (tolerant_flag);
      return Err_VM_BAD_TYPE;
   }
   
   // do not recurse in arrays or object refs
   // and basic types don't have internal refs)
   
   if (ref_class_id >= ClassDescManager::NBR_BasicTypeId_TOTAL)
   {
      const ClassDescriptionMaster::VarInfoArr & var_info_arr
         = cdm_ptr->get_var_info_arr ();
      
      ClassDescriptionMaster::VarInfoArr::const_iterator it = var_info_arr.begin ();
      const ClassDescriptionMaster::VarInfoArr::const_iterator it_end = var_info_arr.end ();
      
      flip::Ref & sub_ref = ref;
      
      for (; it != it_end ; ++it)
      {
         ClassDescriptionMaster * sub_cdm_ptr = it->_cdm_ptr;
         
         int err = check_ref_info_pool_recur (sub_cdm_ptr, sub_ref, sub_parent_ref, tolerant_flag);
         
         if (err != Err_OK)
         {
            assert (tolerant_flag);
            return err;
         }
      }
   }
   
   return Err_OK;
}

   

/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
