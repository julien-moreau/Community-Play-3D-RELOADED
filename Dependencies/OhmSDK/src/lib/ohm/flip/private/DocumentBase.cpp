/*****************************************************************************

        DocumentBase.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67844

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

#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/DocumentWriter.h"
#include "ohm/flip/private/DocumentReader.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/StdCtlMsg.h"

#include "ohm/mon/trace.h"
#include "ohm/util/trace.h"

#include <cassert>

#undef OHM_FLIP_DOCUMENT_DEBUG_FLAG

#undef OHM_FLIP_DOCUMENT_CHECK_SYNC_FLAG

#if defined (OHM_FLIP_DOCUMENT_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define doc_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define doc_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define doc_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define doc_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define doc_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define doc_TRACE(msg_0)   (void (0))
   #define doc_TRACE1(fmt_0, arg1)  (void (0))
   #define doc_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define doc_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define doc_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
#endif

#if defined (OHM_FLIP_DOCUMENT_CHECK_SYNC_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_CHECK_SYNC_FLAG is defined in shipping level code !
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
Description :
   Server version
==============================================================================
*/

DocumentBase::DocumentBase ()
:  _thread_id (0)
,  _server_flag (true)

,  _user_manager ()

,  _object_manager_aptr ()
,  _virtual_machine_aptr ()

,  _data_aptr ()

,  _controller (Controller_NONE)
,  _in_post_task_flag (false)

,  _sys_thread_id (sys::ThreadMgr::get_invalid_thread_id ())
{
   archi::Int32 self_user_id = _user_manager.get_self_user_id ();

   _object_manager_aptr = std::auto_ptr <ObjectManager> (
      new ObjectManager (*this, self_user_id, _thread_id)
   );
   
   // has a dependency on Object Manager
   _virtual_machine_aptr = std::auto_ptr <VirtualMachine> (
      new VirtualMachine (*this, *_object_manager_aptr, true)
   );

   _sys_thread_id = sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : ctor
Description :
   Client version
==============================================================================
*/

DocumentBase::DocumentBase (const std::string & user_name, archi::Int32 thread_id)
:  _thread_id (thread_id)
,  _server_flag (false)

,  _user_manager (user_name)

,  _object_manager_aptr ()
,  _virtual_machine_aptr ()

,  _data_aptr ()

,  _controller (Controller_NONE)
,  _in_post_task_flag (false)

,  _sys_thread_id (sys::ThreadMgr::get_invalid_thread_id ())
{
   assert (thread_id >= 0);
   assert (thread_id <= SeedGenerator::NBR_THREAD_MAX);
   
   _sys_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   doc_TRACE1 ("DocumentBase ctor client, user=\'%1%\'", user_name.c_str ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentBase::~DocumentBase ()
{
}



/*
==============================================================================
Name : set_user_id
==============================================================================
*/

void  DocumentBase::set_user_id (const std::string & user_name, archi::Int32 user_id)
{
   check_caller_thread ();
   
   if (!_user_manager.has_user (user_name))
   {
      _user_manager.set_user_id (user_name, user_id);
      
      if (user_name == _user_manager.get_self_user_name ())
      {
         archi::Int32 self_user_id = _user_manager.get_self_user_id ();
         
         _object_manager_aptr = std::auto_ptr <ObjectManager> (
            new ObjectManager (*this, self_user_id, _thread_id)
         );
         
         // has a dependency on Object Manager
         _virtual_machine_aptr = std::auto_ptr <VirtualMachine> (
            new VirtualMachine (*this, *_object_manager_aptr, false)
         );
      }
   }
   else
   {
      archi::Int32 sub_user_id = _user_manager.get_user_id (user_name);
      assert (sub_user_id == user_id);
   }
}



/*
==============================================================================
Name : get_user_id
==============================================================================
*/

archi::Int32   DocumentBase::get_user_id () const
{
   check_caller_thread ();
   
   return _user_manager.get_self_user_id ();
}



/*
==============================================================================
Name : get_thread_id
==============================================================================
*/

archi::Int32   DocumentBase::get_thread_id () const
{
   check_caller_thread ();
   
   return _thread_id;
}



/*
==============================================================================
Name : get_root
==============================================================================
*/

Object * DocumentBase::get_root ()
{
   check_caller_thread ();
   
   if (_object_manager_aptr.get () != 0)
   {
      // '_object_manager_aptr' is not yet ready, which may happen if the
      // client did not receive a 'set_user_id' from the server
      
      return _object_manager_aptr->get_root ();
   }
   else
   {
      return 0;
   }
}



/*
==============================================================================
Name : get_object
==============================================================================
*/

Object * DocumentBase::get_object (Ref ref)
{
   check_caller_thread ();
   
   assert (_object_manager_aptr.get () != 0);
   
   return _object_manager_aptr->get_object (ref);
}



/*
==============================================================================
Name : use_user_manager
==============================================================================
*/

UserManager &  DocumentBase::use_user_manager ()
{
   check_caller_thread ();
   
   return _user_manager;
}



/*
==============================================================================
Name : use_object_manager
==============================================================================
*/

ObjectManager &   DocumentBase::use_object_manager ()
{
   check_caller_thread ();
   
   if (_object_manager_aptr.get () == 0)
   {
      ohm_util_TRACE_PANIC ("DocumentBase::use_object_manager: no object manager");
      ohm_mon_BACKTRACE;
      ohm_mon_DUMP;
      
      throw std::bad_cast ();
   }
   
   return *_object_manager_aptr;
}



/*
==============================================================================
Name : use_transaction
Description :
   Returns the current transaction if one is already available, or a new
   transaction. This will be one transaction in a transaction session.
   A transaction session must have been started at this point.
==============================================================================
*/

Data &   DocumentBase::use_transaction ()
{
   check_caller_thread ();
   do_check_user_inputs ();

   assert (!_in_post_task_flag);
   
   if (_controller == Controller_NONE)
   {
      assert (_data_aptr.get () == 0);
      
      _data_aptr = std::auto_ptr <Data> (new Data);
      
      set_controller (Controller_SELF);
   }
   else if (_controller == Controller_DOC_RELEASE)
   {
      if (_data_aptr.get () == 0)
      {
         assert (use_object_manager ().is_recycle_bin_empty ());
      
         _data_aptr = std::auto_ptr <Data> (new Data);
      }
   }
   else
   {
      assert (_data_aptr.get () != 0);
      
      // we deny any transaction while receiving data
      assert (_controller == Controller_SELF);
   }
   
   return *_data_aptr;
}



/*
==============================================================================
Name : get_controller
==============================================================================
*/

DocumentBase::Controller   DocumentBase::get_controller () const
{
   check_caller_thread ();
   
   return _controller;
}



/*
==============================================================================
Name : is_in_post
==============================================================================
*/

bool  DocumentBase::is_in_post () const
{
   check_caller_thread ();
   
   return _in_post_task_flag;
}



/*
==============================================================================
Name : use_virtual_machine
==============================================================================
*/

VirtualMachine &  DocumentBase::use_virtual_machine ()
{
   check_caller_thread ();
   
   return *_virtual_machine_aptr;
}



/*
==============================================================================
Name : get_format_version
==============================================================================
*/

const std::string &  DocumentBase::get_format_version () const
{
   check_caller_thread ();
   
   return ClassDescManager::use_instance ().get_format_version ();
}



/*
==============================================================================
Name : add_dependency
==============================================================================
*/

void  DocumentBase::add_dependency (CollectionMaster & collection, Object & object)
{
   check_caller_thread ();
   
#if ! defined (ohm_flip_SERVER)
   RefSet & collection_ptr_set = _dependencies_set [object.get_ref ()];
   
   collection_ptr_set.insert (collection.get_ref ());
#endif
}



/*
==============================================================================
Name : remove_dependencies
==============================================================================
*/

void  DocumentBase::remove_dependencies (CollectionMaster & collection)
{
   check_caller_thread ();
   
#if ! defined (ohm_flip_SERVER)
   DependenciesSet::iterator it = _dependencies_set.begin ();
   const DependenciesSet::iterator it_end = _dependencies_set.end ();
   
   for (; it != it_end ;)
   {
      DependenciesSet::iterator it_next = it;
      ++it_next;
      
      bool erase_set_flag = false;

      {
         RefSet & collection_ptr_set = it->second;
      
         RefSet::iterator fit = collection_ptr_set.find (collection.get_ref ());
         
         if (fit != collection_ptr_set.end ())
         {
            collection_ptr_set.erase (fit);
            
            if (collection_ptr_set.empty ())
            {
               erase_set_flag = true;
            }
         }
      }
      
      if (erase_set_flag)
      {
         _dependencies_set.erase (it);
      }

      it = it_next;
   }
#endif
}



/*
==============================================================================
Name : notify_order_changed
==============================================================================
*/

void  DocumentBase::notify_order_changed (ArrayMaster & array)
{
   check_caller_thread ();
   
#if ! defined (ohm_flip_SERVER)
   _array_ref_set.insert (array.get_ref ());
#endif
}



/*
==============================================================================
Name : notify_order_changed
==============================================================================
*/

void  DocumentBase::notify_order_changed (CollectionMaster & collection)
{
   check_caller_thread ();
   
#if ! defined (ohm_flip_SERVER)
   _collection_ref_set.insert (collection.get_ref ());
#endif
}



/*
==============================================================================
Name : require_sync_operations
==============================================================================
*/

void  DocumentBase::require_sync_operations (Object & object)
{
   check_caller_thread ();
   
   _cache_ref_set.insert (object.get_ref ());
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  DocumentBase::trace () const
{
   check_caller_thread ();
   
   assert (_object_manager_aptr.get () != 0);
   
   _object_manager_aptr->trace ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_check_user_inputs
==============================================================================
*/

void  DocumentBase::do_check_user_inputs ()
{
   // default implementation : nothing
}



/*
==============================================================================
Name : check_caller_thread
==============================================================================
*/

void  DocumentBase::check_caller_thread () const
{
#if ! defined (NDEBUG)

   sys::ThreadMgr::IdType cur_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   assert (cur_thread_id == _sys_thread_id);
   
#endif
}



/*
==============================================================================
Name : set_controller
==============================================================================
*/

void  DocumentBase::set_controller (Controller controller)
{
   check_caller_thread ();
   
   if (controller == Controller_NONE)
   {
      assert (_controller != Controller_NONE);
      
      _controller = controller;
   }
   else
   {
      assert (_controller == Controller_NONE);
      
      _controller = controller;
   }
}



/*
==============================================================================
Name : get_tx_data_aptr
Note :
   Lose tx data ownership.
==============================================================================
*/

std::auto_ptr <Data> DocumentBase::get_tx_data_aptr ()
{
   check_caller_thread ();
   
   return _data_aptr;
}



/*
==============================================================================
Name : enable_post
==============================================================================
*/

void  DocumentBase::enable_post (bool post_flag)
{
   check_caller_thread ();
   
   assert (_in_post_task_flag != post_flag);
   
   _in_post_task_flag = post_flag;
}



/*
==============================================================================
Name : pre_sync
==============================================================================
*/

void  DocumentBase::pre_sync ()
{
   check_caller_thread ();
   
   RefSet::iterator it = _cache_ref_set.begin ();
   const RefSet::iterator it_end = _cache_ref_set.end ();
   
   for (; it != it_end ;)
   {
      RefSet::iterator it_next = it;
      ++it_next;
      
      Ref ref = *it;
      
      Object * object_ptr = get_object (ref);
      
      if (object_ptr == 0)
      {
         _cache_ref_set.erase (it);
      }
      else
      {
         if (object_ptr->is_invalid ())
         {
            object_ptr->pre_sync ();
         }
      }
      
      it = it_next;
   }
}



/*
==============================================================================
Name : post_reorder_containers
Description :
   Called in post, and just before the document observer are notified.
   All marked containers are reordered. They were marked through
   'notify_order_changed' when a potential reordering might have took place.
==============================================================================
*/

void  DocumentBase::post_reorder_containers ()
{
   check_caller_thread ();
   
#if ! defined (ohm_flip_SERVER)
   // reorder arrays
   
   {
      RefSet::iterator it = _array_ref_set.begin ();
      const RefSet::iterator it_end = _array_ref_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Ref ref = *it;
         
         ArrayMaster * array_ptr = get_object <ArrayMaster> (ref);
         
         if (array_ptr != 0)
         {
            array_ptr->broadcast_order_change ();
         }
         else
         {
            // the container might have been erased after he notified
         }
      }
   }
   
   // check external collection dependencies
   
   {
      DependenciesSet::iterator it = _dependencies_set.begin ();
      const DependenciesSet::iterator it_end = _dependencies_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Ref ref = it->first;
         
         Object * object_ptr = get_object (ref);
         
         if (object_ptr != 0)
         {
            if (object_ptr->is_invalid ())
            {
               RefSet & collection_ref_set = it->second;
               
               RefSet::iterator sit = collection_ref_set.begin ();
               const RefSet::iterator sit_end = collection_ref_set.end ();
               
               for (; sit != sit_end ; ++sit)
               {
                  _collection_ref_set.insert (*sit);
               }
            }
         }
         else
         {
            // the dependencies might have been erased
         }
      }
   }
   
   // reorder collections
   
   {
      RefSet::iterator it = _collection_ref_set.begin ();
      const RefSet::iterator it_end = _collection_ref_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Ref ref = *it;
         
         CollectionMaster * collection_ptr = get_object <CollectionMaster> (ref);
         
         if (collection_ptr != 0)
         {
            collection_ptr->broadcast_order_change ();
         }
         else
         {
            // the container might have been erased after he notified
         }
      }
   }
   
   // clean up
   
   _array_ref_set.clear ();
   _collection_ref_set.clear ();
#endif
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
