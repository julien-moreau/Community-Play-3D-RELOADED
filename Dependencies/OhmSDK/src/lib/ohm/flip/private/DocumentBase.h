/*****************************************************************************

        DocumentBase.h
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



#if ! defined (ohm_flip_DocumentBase_HEADER_INCLUDED)
#define  ohm_flip_DocumentBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Data.h"
#include "ohm/flip/DataMaster.h"
#include "ohm/flip/Object.h"
#include "ohm/flip/private/ObjectManager.h"
#include "ohm/flip/private/TxDesc.h"
#include "ohm/flip/private/TxIdGenerator.h"
#include "ohm/flip/private/UserManager.h"
#include "ohm/flip/private/VirtualMachine.h"
#include "ohm/sys/ThreadMgr.h"

#include <set>
#include <map>
#include <string>
#include <list>
#include <memory>



namespace ohm
{
namespace flip
{



class Float64;
class Object;
class MsgRouting;
class Scribe;

class DocumentBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Controller
   {
                  Controller_NONE = 0,
                  Controller_SERVER,
                  Controller_SELF,
                  Controller_NETWORK_EXT,
                  Controller_NETWORK_ACK,
                  Controller_NETWORK_DENY,
                  Controller_LOCAL,
                  Controller_UNDO_REDO,
                  Controller_DOC_READ,
                  Controller_DOC_WRITE,
                  Controller_DOC_DIF,
                  Controller_TRASH,
                  Controller_DOC_RELEASE,
   };
   
                  DocumentBase ();
                  DocumentBase (const std::string & user_name, archi::Int32 thread_id);
   virtual        ~DocumentBase ();
   
   void           set_user_id (const std::string & user_name, archi::Int32 user_id);
   archi::Int32   get_user_id () const;
   
   archi::Int32   get_thread_id () const;
   virtual archi::Int64
                  get_rev_id () = 0;
   
   Object *       get_root ();
   template <class T>
   T &            get_root ();
   Object *       get_object (Ref ref);
   template <class T>
   T *            get_object (Ref ref);
   
   UserManager &  use_user_manager ();
   ObjectManager &
                  use_object_manager ();
   
   Data &         use_transaction ();
   
   Controller     get_controller () const;
   bool           is_in_post () const;
   
   VirtualMachine &
                  use_virtual_machine ();
   const std::string &
                  get_format_version () const;
   
   void           add_dependency (CollectionMaster & collection, Object & object);
   void           remove_dependencies (CollectionMaster & collection);
   void           notify_order_changed (ArrayMaster & array);
   void           notify_order_changed (CollectionMaster & collection);
   
   void           require_sync_operations (Object & object);

   virtual void   trace () const;
   
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual void   do_check_user_inputs ();
   
   void           check_caller_thread () const;
   void           set_controller (Controller controller);
   std::auto_ptr <Data>
                  get_tx_data_aptr ();
   void           enable_post (bool post_flag);
   void           pre_sync ();
   void           post_reorder_containers ();
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <std::string, archi::Int32> UserNameIdMap;
   typedef std::set <flip::Ref> RefSet;
   typedef std::map <flip::Ref, RefSet> DependenciesSet;
   
   const archi::Int32
                  _thread_id;
   const bool     _server_flag;
   
   UserManager    _user_manager;
   
   std::auto_ptr <ObjectManager>
                  _object_manager_aptr;
   
   std::auto_ptr <VirtualMachine>
                  _virtual_machine_aptr;
   
   std::auto_ptr <Data>
                  _data_aptr;

   Controller     _controller;
   bool           _in_post_task_flag;
   
   sys::ThreadMgr::IdType
                  _sys_thread_id;
   RefSet         _cache_ref_set;
   
#if ! defined (ohm_flip_SERVER)
   DependenciesSet
                  _dependencies_set;
   RefSet         _array_ref_set;
   RefSet         _collection_ref_set;
#endif

   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentBase (const DocumentBase &other);
   DocumentBase & operator = (const DocumentBase &other);
   bool           operator == (const DocumentBase &other);
   bool           operator != (const DocumentBase &other);

}; // class DocumentBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/private/DocumentBase.hpp"



#endif   // ohm_flip_DocumentBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
