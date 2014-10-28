/*****************************************************************************

        Object.cpp
        Copyright (c) 2007 Ohm Force

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

#include "ohm/flip/Object.h"
#include "ohm/flip/ClassDescription.h"

#if ! defined (ohm_flip_SERVER)
   #include "ohm/flip/DocumentClient.h"
#endif

#include "ohm/flip/DocumentServer.h"

#include "ohm/flip/PasteBoard.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/ObjectTravelAgency.h"
#include "ohm/flip/private/TravelerObjectSynchronize.h"
#include "ohm/flip/private/TravelerObjectCheckSynchronization.h"
#include "ohm/flip/private/SeedGenerator.h"

#include "ohm/txt/out/Print.h"
#include "ohm/util/trace.h"

#include <cassert>

#undef OHM_FLIP_OBJECT_DEBUG_FLAG
#undef OHM_FLIP_OBJECT_DEBUG_MORE_FLAG
#undef OHM_FLIP_OBJECT_MORE_CHECKS_FLAG

#if defined (OHM_FLIP_OBJECT_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_OBJECT_DEBUG_FLAG is defined in shipping level code !
   #endif
   
   #define ob_TRACE(msg_0) ohm_util_TRACE_DEBUG(msg_0)
   #define ob_TRACE1(fmt_0, arg1)   ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define ob_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define ob_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define ob_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define ob_TRACE(msg_0) (void (0))
   #define ob_TRACE1(fmt_0, arg1)   (void (0))
   #define ob_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define ob_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define ob_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))
#endif

#if defined (OHM_FLIP_OBJECT_DEBUG_MORE_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_OBJECT_DEBUG_MORE_FLAG is defined in shipping level code !
   #endif
   
   #define ob_TRACE_MORE(msg_0)  ohm_util_TRACE_DEBUG(msg_0)
   #define ob_TRACE_MORE1(fmt_0, arg1) ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define ob_TRACE_MORE2(fmt_0, arg1, arg2) ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define ob_TRACE_MORE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define ob_TRACE_MORE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define ob_TRACE_MORE(msg_0)  (void (0))
   #define ob_TRACE_MORE1(fmt_0, arg1) (void (0))
   #define ob_TRACE_MORE2(fmt_0, arg1, arg2) (void (0))
   #define ob_TRACE_MORE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define ob_TRACE_MORE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif

#if defined (OHM_FLIP_OBJECT_MORE_CHECKS_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_OBJECT_MORE_CHECKS_FLAG is defined in shipping level code !
   #endif
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Object::Object (DocumentBase & document)
:  _document (document)
,  _ref (0)
,  _cdm_ptr (0)

,  _old_parent_order (std::make_pair ((Object *) (0), size_t (-1)))
,  _parent_order (std::make_pair ((Object *) (0), size_t (-1)))

,  _old_inited_flag (false)
,  _inited_flag (false)

,  _inner_invalid_cnt (0)
,  _total_invalid_cnt (0)

,  _inner_conflict_cnt (0)
,  _total_conflict_cnt (0)

,  _id_ptr_map ()
{
   assert (&document != 0);
   
   _ref = _document.use_object_manager ().assign_ref (_parent_order.first, _cdm_ptr, this);
   assert (_ref != 0);
   
   _old_parent_order = _parent_order;

#if ! defined (NDEBUG)
   debug_check ();
#endif
   
   ob_TRACE1 ("OBJ CTR %1m16rw0X%", _ref);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Object::~Object ()
{
   try
   {
      ob_TRACE1 ("OBJ DTR %1m16rw0X%", _ref);
      
      assert (_ref != 0);
      assert (!_inited_flag);
      
      _document.use_object_manager ().unassign_ref (_ref);
      
      _ref = 0;
      _parent_order = std::make_pair ((Object *) (0), size_t (-1));
      _old_parent_order = _parent_order;
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : ctor
Description :
   This function is called when the object is locally created with arguments.
==============================================================================
*/

void  Object::ctor (Args & args)
{
   // nothing
   // in particular args.end () is checked after this call
}



/*
==============================================================================
Name : copy
Description :
   This function copy the whole information tree attached to this objet,
   as well as its destiny, that is the object properties from this
   object to root, following parents.
==============================================================================
*/

ObjectMold & Object::copy ()
{
   return PasteBoard::use_instance ().add (*this);
}



/*
==============================================================================
Name : signal
Description :
  Send a signal to other threads directed at this object.
  Return true iff signal was sent.
==============================================================================
*/

bool  Object::send_signal (archi::UInt32 type, DataSPtr data_sptr) const
{
   bool ok_flag = false;
   
#if defined (ohm_flip_SERVER)
   ohm_util_TRACE_ERROR ("signal are not supported on server");
   assert (false);
   
#else
   DocumentClient * document_ptr = dynamic_cast <DocumentClient *> (&_document);
   assert (document_ptr != 0);

   if (document_ptr != 0)
   {
      ok_flag = document_ptr->send_signal (_ref, type, data_sptr);
   }
   else
   {
      ohm_util_TRACE_ERROR ("document_ptr is null!");
   }

#endif
   
   return ok_flag;
}



/*
==============================================================================
Name : send_signal_server
Description :
  Send a signal to the server directed at this object.
  Return true iff signal was sent.
==============================================================================
*/

bool  Object::send_signal_server (archi::UInt32 type, DataSPtr data_sptr) const
{
   bool ok_flag = false;
   
#if defined (ohm_flip_SERVER)
   ohm_util_TRACE_ERROR ("signal are not supported on server");
   assert (false);
   
#else
   DocumentClient * document_ptr = dynamic_cast <DocumentClient *> (&_document);
   assert (document_ptr != 0);

   if (document_ptr != 0)
   {
      ok_flag = document_ptr->send_signal_server (_ref, type, data_sptr);
   }
   else
   {
      ohm_util_TRACE_ERROR ("document_ptr is null!");
   }

#endif
   
   return ok_flag;
}



/*
==============================================================================
Name : send_signal_client
Description :
  Send a signal to the client directed at this object.
  Return true iff signal was sent.
==============================================================================
*/

void  Object::send_signal_client (const std::string & to, archi::UInt32 type, const archi::UByte * data_ptr, archi::UInt32 data_size) const
{
   DocumentServer * document_ptr = dynamic_cast <DocumentServer *> (&_document);
   assert (document_ptr != 0);

   if (document_ptr != 0)
   {
      document_ptr->send_signal_client (to, _ref, type, data_ptr, data_size);
   }
   else
   {
      ohm_util_TRACE_ERROR ("Object::send_signal_client failed");
   }
}



/*
==============================================================================
Name : init
Description :
   Called and cascaded at the moment :
   - root is created
   - an object is inserted in a container
==============================================================================
*/

void  Object::init ()
{
   ob_TRACE1 ("OBJ INI %1m16rw0X%", _ref);
   
   assert (!_inited_flag);
   
   if (_old_inited_flag == _inited_flag)
   {
      _inited_flag = true;
      
      invalidate ();
   }
   else
   {
      _inited_flag = true;
      
      validate ();
   }
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  Object::restore ()
{
   ob_TRACE1 ("OBJ RES %1m16rw0X%", _ref);
   
   assert (_inited_flag);
   
   if (_old_inited_flag == _inited_flag)
   {
      _inited_flag = false;
      
      invalidate ();
   }
   else
   {
      _inited_flag = false;
      
      validate ();
   }
}



/*
==============================================================================
Name : order_changed
Description :
   This function is called when the object is in a container
   for which there is an explicit concept of order (Array) or an implicit
   concept of order (Collection)
   This notification is sent when the reordering operation took place so
   that the actual order in the container is valid.
   The order number itself is determined by the container, and is pushed
   every time the container size is changed, or a move took place.
   
(Old Note :
   Even in the case where the relative order (for example position 0) does
   not change, the object gets called, as internally the object might
   consider an expression like size () - order.)
   Which is now false, except if different observer defines different orders
==============================================================================
*/

void  Object::change_order (size_t order)
{
   if (_parent_order.second != order)
   {
      ob_TRACE2 ("OBJ ORD %1m16rw0X% %2%", _ref, order);
      
      if (_old_parent_order == _parent_order)
      {
         _parent_order.second = order;
         
         invalidate ();
         
         //_parent_order.second = order;
      }
      else
      {
         _parent_order.second = order;
         
         if (_old_parent_order == _parent_order)
         {
            validate ();
         }
      }
   }
}



/*
==============================================================================
Name : change_parent
Description :
   This function is called when the object is moved from one container to
   another.
==============================================================================
*/

void  Object::change_parent (Object * parent_ptr)
{
   assert (parent_ptr != 0);
   
   ob_TRACE2 ("OBJ PAR %1m16rw0X% / %2m16rw0X%", parent_ptr->get_ref (), _ref);
   
   assert (_parent_order.first != parent_ptr);
   
   // displace the invalidate cnt from old parent to new parent
   
   Object * cur_old_parent_ptr = _parent_order.first;
   
   cur_old_parent_ptr->propagate_validate (_total_invalid_cnt);
   cur_old_parent_ptr->propagate_resolve (_total_conflict_cnt);
   
   if (_old_parent_order == _parent_order)
   {
      ++_inner_invalid_cnt;
      ++_total_invalid_cnt;
   }

   _parent_order.first = parent_ptr;

   if (_old_parent_order == _parent_order)
   {
      assert (_inner_invalid_cnt > 0);
      assert (_total_invalid_cnt > 0);
      
      --_inner_invalid_cnt;
      --_total_invalid_cnt;
   }
   
   Object * cur_new_parent_ptr = _parent_order.first;
   cur_new_parent_ptr->propagate_invalidate (_total_invalid_cnt);
   cur_new_parent_ptr->propagate_conflict (_total_conflict_cnt);
}



/*
==============================================================================
Name : set_conflict
Description :
   This function is called when the object would have triggered a
   conflictuous state if its transaction would have been executed.
   It is called from the transaction manager when it receives a refused
   transaction.
   This is issued when the transaction would have triggered the following
   exceptions :
   Err_VM_VALUE_COLLISION
   Err_VM_ARRAY_KEY_EXISTS
   Err_VM_ARRAY_KEY_DOES_NOT_EXIST
   Err_VM_ARRAY_OBJECT_CANNOT_BE_RELEASED
   Err_VM_COLLECTION_OBJECT_EXISTS
   Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST
   Err_VM_COLLECTION_OBJECT_CANNOT_BE_RELEASED
   
Note :
   Do we need :
   Err_VM_BAD_ACCESS ?
==============================================================================
*/

void  Object::set_conflict ()
{
   ob_TRACE1 ("OBJ CFL %1m16rw0X%", _ref);
   
   conflict ();
}



/*
==============================================================================
Name : is_invalid
Description :
   Tells if the object (inner or subtree) is invalid or conflictuous
==============================================================================
*/

bool  Object::is_invalid () const
{
   return (_total_invalid_cnt > 0) || (_total_conflict_cnt > 0);
}



/*
==============================================================================
Name : has_conflict
Description :
   Tells if the object (inner or subtree) is conflictuous
==============================================================================
*/

bool  Object::has_conflict () const
{
   return (_total_conflict_cnt > 0);
}



/*
==============================================================================
Name : is_inited
==============================================================================
*/

bool  Object::is_inited () const
{
   return _inited_flag;
}



/*
==============================================================================
Name : was_inited
==============================================================================
*/

bool  Object::was_inited () const
{
   return (!_old_inited_flag) && _inited_flag;
}



/*
==============================================================================
Name : was_restored
==============================================================================
*/

bool  Object::was_restored () const
{
   return _old_inited_flag && (!_inited_flag);
}



/*
==============================================================================
Name : did_order_change
==============================================================================
*/

bool  Object::did_order_change () const
{
   // we consider that the order changes when parent change
   
   return _old_parent_order != _parent_order;
}



/*
==============================================================================
Name : did_parent_change
==============================================================================
*/

bool  Object::did_parent_change () const
{
   return _old_parent_order.first != _parent_order.first;
}



/*
==============================================================================
Name : get_order
==============================================================================
*/

size_t   Object::get_order () const
{
   return _parent_order.second;
}



/*
==============================================================================
Name : get_old_order
==============================================================================
*/

size_t   Object::get_old_order () const
{
   return _old_parent_order.second;
}



/*
==============================================================================
Name : backup
Description :
   This function request the inherited object to report a transaction that
   would put it back to ctor state.
==============================================================================
*/

void  Object::backup ()
{
   // nothing
}



/*
==============================================================================
Name : synchronize
Description :
   This function is called after :
   - the virtual machine is runned and invalidation/validation were sent
   - the observer were notified
   Its goal is to synchronize old & new values, and reset invalidation counts
   This function will not cascade itself, but the traveller will first use
   the invalid cnt to determine if it needs to continue to synchronize, and
   delete objects.
==============================================================================
*/

void  Object::synchronize ()
{
   // first the POD or containers have been called
   
   ob_TRACE_MORE1 ("OBJ SYN %1m16rw0X%", _ref);
   
   // we might have some direct compounds
   
   if ((_total_invalid_cnt > 0) || (_total_conflict_cnt > 0))
   {
      TravelerObjectSynchronize traveler (use_document ());
         
      ObjectTravelAgency travel_agency (use_document ());
      travel_agency.travel (
         get_ref (),
         ObjectTravelAgency::Flag_SKIP_FIRST,
         traveler
      );
   }
   
   // reset
   
   _old_parent_order = _parent_order;
   _old_inited_flag = _inited_flag;
   
   _inner_invalid_cnt = 0;
   _total_invalid_cnt = 0;

   _inner_conflict_cnt = 0;
   _total_conflict_cnt = 0;
   
   post_sync ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

void  Object::check_synchronized () const
{
   // depth first
   
   TravelerObjectCheckSynchronization traveler (use_document ());
      
   ObjectTravelAgency travel_agency (use_document ());
   travel_agency.travel (
      get_ref (),
      ObjectTravelAgency::Flag_SKIP_FIRST,
      traveler
   );

   assert (_total_invalid_cnt == 0);
   assert (_inner_invalid_cnt == 0);
   assert (_inner_conflict_cnt == 0);
   assert (_total_conflict_cnt == 0);
   assert (_old_parent_order == _parent_order);
   assert (_old_inited_flag == _inited_flag);
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

void  Object::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (_old_parent_order.first != _parent_order.first)
   {
      ohm_util_TRACE_DEBUG ("  PAR");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   if (_old_parent_order.second != _parent_order.second)
   {
      ohm_util_TRACE_DEBUG ("  ORD");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   if (_old_inited_flag != _inited_flag)
   {
      ohm_util_TRACE_DEBUG ("  INI");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   assert (invalid_cnt == 0);
}



/*
==============================================================================
Name : get_ref
==============================================================================
*/

Ref   Object::get_ref () const
{
   assert (_ref != 0);
   
   return _ref;
}



/*
==============================================================================
Name : get_user_id
==============================================================================
*/

archi::Int32   Object::get_user_id () const
{
   return SeedGenerator::to_user_id (get_ref ());
}



/*
==============================================================================
Name : get_user_id
==============================================================================
*/

bool  Object::is_self_created () const
{
   return get_user_id () == _document.get_user_id ();
}



/*
==============================================================================
Name : use_document
==============================================================================
*/

DocumentBase & Object::use_document () const
{
   return _document;
}



/*
==============================================================================
Name : erase_from_parent
==============================================================================
*/

void  Object::erase_from_parent ()
{
   Object * parent_ptr = _parent_order.first;
   
   assert (parent_ptr != 0);
   
   bool continue_flag = true;
   
   {
      ArrayMaster * array_ptr = dynamic_cast <ArrayMaster *> (parent_ptr);
      
      if (array_ptr != 0)
      {
         array_ptr->erase (_ref);
         
         continue_flag = false;
      }
   }
   
   {
      CollectionMaster * collection_ptr = dynamic_cast <CollectionMaster *> (parent_ptr);
      
      if (collection_ptr != 0)
      {
         collection_ptr->erase (_ref);
         
         continue_flag = false;
      }
   }
   
   assert (!continue_flag);
}



/*
==============================================================================
Name : get_parent_ptr
==============================================================================
*/

Object * Object::get_parent_ptr ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   Object * parent_ptr = _parent_order.first;
   
   return parent_ptr;
}



/*
==============================================================================
Name : get_parent_ptr
==============================================================================
*/

const Object * Object::get_parent_ptr () const
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_restored ());
#endif
   
   const Object * parent_ptr = _parent_order.first;
   
   return parent_ptr;
}



/*
==============================================================================
Name : get_parent_ptr
==============================================================================
*/

Object * Object::get_old_parent_ptr ()
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   Object * parent_ptr = _old_parent_order.first;
   
   return parent_ptr;
}



/*
==============================================================================
Name : get_parent_ptr
==============================================================================
*/

const Object * Object::get_old_parent_ptr () const
{
#if defined (OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG)
   assert (!was_inited ());
#endif
   
   const Object * parent_ptr = _old_parent_order.first;
   
   return parent_ptr;
}



/*
==============================================================================
Name : get_cdm
==============================================================================
*/

ClassDescriptionMaster &   Object::get_cdm () const
{
   assert (_cdm_ptr != 0);
   
   return *_cdm_ptr;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  Object::trace () const
{
   ohm_util_TRACE_INFO1 ("OBJ %1m16rw0X%", _ref);
   
   if (_cdm_ptr != 0)
   {
      ohm_util_TRACE_INFO1 ("  %1%", _cdm_ptr->get_name ());
   }
   else
   {
      ohm_util_TRACE_INFO ("  NO CDM");
   }
   
   ohm_util_TRACE_INFO2 (
      " INIT %1% -> %2%",
      _old_inited_flag ? "true " : "false",
      _inited_flag ? "true " : "false"
   );
}



/*
==============================================================================
Name : require_sync_operations
Description :
   Notify the document that 'this' object require cache operations
   (invalidation and synchronization)
==============================================================================
*/

void  Object::require_sync_operations ()
{
   _document.require_sync_operations (*this);
}



/*
==============================================================================
Name : pre_sync
Description :
   For objects that maitain a cache of value, this function is called for
   them to invalidate their cache.
   This function is called :
   - after the objects have been invalidated
   - before the document observers are called
   Therefore it can be used to cache value like in a 'document_changed' but
   before the observers are called.
==============================================================================
*/

void  Object::pre_sync ()
{
   // if you bind the document to receive sync operations, you then must
   // implement this function
   
   assert (false);
}



/*
==============================================================================
Name : post_sync
Description :
   For objects that maitain a cache of value, this function is called for
   them to synchronize their cache.
==============================================================================
*/

void  Object::post_sync ()
{
   // default impl : nothing
}



/*
==============================================================================
Name : who
Description :
   Returns the description of 'this' relative to its direct container :
   - as a member of its parent object
   - as an element of a container
   - as root
==============================================================================
*/

std::string Object::who () const
{
   std::string ret_val;
   
   flip::Object * parent_ptr = _parent_order.first;
   
   if (parent_ptr != 0)
   {
      ClassDescriptionMaster * parent_cdm_ptr = parent_ptr->_cdm_ptr;
      assert (parent_cdm_ptr != 0);
         
      ArrayMaster * array_ptr = dynamic_cast <ArrayMaster *> (parent_ptr);
      CollectionMaster * collection_ptr = dynamic_cast <CollectionMaster *> (parent_ptr);
      
      txt::out::Print printer;
      printer.print ("%1m16rw0X%: ").arg (_ref);
      
      if (array_ptr != 0)
      {
         printer.cat ("<array member>");
      }
      else if (collection_ptr != 0)
      {
         printer.cat ("<collection member>");
      }
      else
      {
         const ClassDescriptionMaster::VarInfoArr & via =
            parent_cdm_ptr->get_var_info_arr ();
         
         archi::Int64 offset = _ref - parent_ptr->_ref - 1;
         
         bool ok_flag
            = (offset >= 0)
            && (offset < archi::Int64 (via.size ()));
         
         if (ok_flag)
         {
            printer.cat ("%1%::%2%")
               .arg (parent_cdm_ptr->get_name ())
               .arg (via [size_t (offset)]._name_0);
         }
         else
         {
            assert (false);
            printer.cat ("<out of parent bounds>");
         }
      }
            
      ret_val = printer;
   }
   else
   {
      ret_val = "<root>";
   }
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : invalidate
Description :
   Increment the invalid count, only targetting the internal fields
   of this object (this is, not the sub objects it might contains)
   This happens when :
   - the pod value was changed from old value
   - the state was changed from old state
   - the order in containers was changed from old order
   - the parent (container) was changed from old parent
==============================================================================
*/

void  Object::invalidate ()
{
   ob_TRACE_MORE2 ("OBJ INVAL %1m16rw0X% %2%", _ref, _inner_invalid_cnt);
   
   ++_inner_invalid_cnt;

#if defined (OHM_FLIP_OBJECT_MORE_CHECKS_FLAG)  
   check_inner_invalidation_cnt (_inner_invalid_cnt);
#endif
   
   propagate_invalidate (1);
}



/*
==============================================================================
Name : propagate_invalidate
Description :
   Increment the general invalid count which enclose internal count and
   child counts. Propragate to parent.
==============================================================================
*/

void  Object::propagate_invalidate (size_t cnt)
{
   _total_invalid_cnt += cnt;
   
   Object * parent_ptr = _parent_order.first;
   
   if (parent_ptr != 0)
   {
      parent_ptr->propagate_invalidate (cnt);
   }
   else
   {
      assert (_cdm_ptr->is_root ());
   }
}



/*
==============================================================================
Name : validate
Description :
   Decrement the invalid count, only targetting the internal fields
   of this object (this is, not the sub objects it might contains)
   This happens when :
   - the pod value was changed back to old value
   - the state was changed back to old state
   - the order in containers was changed back to old order
   - the parent (container) was changed back to old parent
==============================================================================
*/

void  Object::validate ()
{
   ob_TRACE_MORE2 ("OBJ VAL %1m16rw0X% %2%", _ref, _inner_invalid_cnt);
   
   assert (_inner_invalid_cnt > 0);
   
   --_inner_invalid_cnt;

#if defined (OHM_FLIP_OBJECT_MORE_CHECKS_FLAG)  
   check_inner_invalidation_cnt (_inner_invalid_cnt);
#endif
   
   propagate_validate (1);
}



/*
==============================================================================
Name : propagate_validate
==============================================================================
*/

void  Object::propagate_validate (size_t cnt)
{
   assert (_total_invalid_cnt >= cnt);
   
   _total_invalid_cnt -= cnt;
   
   Object * parent_ptr = _parent_order.first;

   if (parent_ptr != 0)
   {
      parent_ptr->propagate_validate (cnt);
   }
   else
   {
      assert (_cdm_ptr->is_root ());
   }
}



/*
==============================================================================
Name : conflict
Description :
   Increment the conflict count, only targetting the internal fields
   of this object (this is, not the sub objects it might contains)
==============================================================================
*/

void  Object::conflict ()
{
   ob_TRACE_MORE1 ("OBJ CONFL %1m16rw0X%", _ref);
   
   ++_inner_conflict_cnt;

#if defined (OHM_FLIP_OBJECT_MORE_CHECKS_FLAG)  
   check_inner_invalidation_cnt (_inner_invalid_cnt);
#endif
   
   propagate_conflict (1);
}



/*
==============================================================================
Name : propagate_conflict
Description :
   Increment the general conflict count which enclose internal count and
   child counts. Propragate to parent.
==============================================================================
*/

void  Object::propagate_conflict (size_t cnt)
{
   _total_conflict_cnt += cnt;
   
   Object * parent_ptr = _parent_order.first;
   
   if (parent_ptr != 0)
   {
      parent_ptr->propagate_conflict (cnt);
   }
   else
   {
      assert (_cdm_ptr->is_root ());
   }
}



/*
==============================================================================
Name : propagate_resolve
==============================================================================
*/

void  Object::propagate_resolve (size_t cnt)
{
   assert (_total_conflict_cnt >= cnt);
   
   _total_conflict_cnt -= cnt;
   
   Object * parent_ptr = _parent_order.first;

   if (parent_ptr != 0)
   {
      parent_ptr->propagate_resolve (cnt);
   }
   else
   {
      assert (_cdm_ptr->is_root ());
   }
}



/*
==============================================================================
Name : use_object_manager
==============================================================================
*/

ObjectManager &   Object::use_object_manager () const
{
   return _document.use_object_manager ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : debug_check
==============================================================================
*/

void  Object::debug_check ()
{
   Object * parent_ptr = _parent_order.first;
   
   if (parent_ptr != 0)
   {
      ClassDescriptionMaster * parent_cdm_ptr = parent_ptr->_cdm_ptr;
      assert (parent_cdm_ptr != 0);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_BOOL);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_INT64);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_ENUM);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_FLOAT64);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_BLOB);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_OBJECT_REF);
      assert (parent_cdm_ptr->get_class_id () != ClassDescManager::BasicTypeId_CUE);
   }
   else
   {
      // nothing, this is Root
      assert (_cdm_ptr->is_root ());
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
