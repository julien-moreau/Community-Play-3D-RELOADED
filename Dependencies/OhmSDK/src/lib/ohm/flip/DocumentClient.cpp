/*****************************************************************************

        DocumentClient.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/flip/DocumentClient.h"
#include "ohm/flip/Scribable.h"
#include "ohm/flip/Scribe.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/flip/private/DocumentClientMasterSlot.h"
#include "ohm/flip/private/DocumentDifferentiator.h"
#include "ohm/flip/private/DocumentReader.h"
#include "ohm/flip/private/DocumentWriter.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/private/StdCtlMsg.h"

#include "ohm/mon/trace.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamOutputNa.h"
#include "ohm/util/trace.h"

#include "zlib.h"

#include <cassert>

#undef OHM_FLIP_DOCUMENT_DEBUG_FLAG

#if defined (OHM_FLIP_DOCUMENT_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define docc_TRACE(msg_0)  ohm_util_TRACE_DEBUG(msg_0)
   #define docc_TRACE1(fmt_0, arg1) ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define docc_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define docc_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define docc_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define docc_TRACE(msg_0)  (void (0))
   #define docc_TRACE1(fmt_0, arg1) (void (0))
   #define docc_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define docc_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define docc_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : AutoUserInputs::ctor
Description :
==============================================================================
*/

DocumentClient::AutoUserInputs::AutoUserInputs (DocumentBase & document)
:  _document (dynamic_cast <DocumentClient &> (document))
{
   assert (&document != 0);
   
   _document.notify_pre_user_inputs_tasks ();
}



/*
==============================================================================
Name : AutoUserInputs::dtor
Description :
==============================================================================
*/

DocumentClient::AutoUserInputs::~AutoUserInputs ()
{
   _document.notify_post_user_inputs_tasks ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

DocumentClient::DocumentClient (DocumentClientMasterSlot & parent, const std::string & user_name, archi::Int32 thread_id)
:  DocumentBase (user_name, thread_id)
,  _parent (parent)
,  _machine_state (MachineState_HEAD)
,  _document_state (DocumentState_VOID)
,  _in_user_inputs_task_flag (false)

,  _tx_id_generator_aptr ()
,  _tx_desc ()
,  _tx_session_state (TxSessionState_NONE)
,  _scribe_ptr_set ()
,  _need_document_changed_notification_flag (false)
,  _document_observer_ptr (0)

,  _transactions_manager_aptr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentClient::~DocumentClient ()
{
   assert (_machine_state == MachineState_HEAD);
}



/*
==============================================================================
Name : use_client_master_slot
==============================================================================
*/

DocumentClientMasterSlot & DocumentClient::use_client_master_slot ()
{
   check_caller_thread ();
   
   return _parent;
}



/*
==============================================================================
Name : restore
Description :
   Prepare for the deletion of 'this'
   We delete the root object and notify observers if the root object exists.
==============================================================================
*/

void  DocumentClient::restore ()
{
   check_caller_thread ();
   
   Object * root_ptr = get_root ();
   // 'root_ptr' might be 0 when document was not ready
   
   if (root_ptr != 0)
   {
      set_controller (Controller_DOC_RELEASE);
      
      pre_document_change_task ();
      
      use_object_manager ().release_root ();
      
      post_document_change_task ();
      
      set_controller (Controller_NONE);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  DocumentClient::bind (DocumentObserverMaster & observer)
{
   assert (&observer != 0);
   assert (_document_observer_ptr == 0);
   
   _document_observer_ptr = &observer;
}



/*
==============================================================================
Name : begin_undo_redo_replay
==============================================================================
*/

void  DocumentClient::begin_undo_redo_replay ()
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (_document_state == DocumentState_READY);
   
   assert (get_controller () == Controller_NONE);
   set_controller (Controller_UNDO_REDO);
   
   // don't call pre_document_change_task
   // as we might have objects in the recycle bin
   // this is because undo/redo is not alone in
   // a run loop cycle
   
   docc_TRACE ("CTL UNRD");
}



/*
==============================================================================
Name : end_undo_redo_replay
==============================================================================
*/

void  DocumentClient::end_undo_redo_replay ()
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (_document_state == DocumentState_READY);
   
   assert (get_controller () == Controller_UNDO_REDO);

   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : write_document_head
==============================================================================
*/

DataSPtr DocumentClient::write_document_head ()
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   
   DataSPtr data_sptr (new Data);
   
   ObjectManager & object_manager = use_object_manager ();
   
   if (object_manager.get_root () != 0)
   {
      DataStreamOutput dso;
      dso.set_mem_output (*data_sptr);
      dso.set_filter_zlib ();

      DocumentWriter writer (*this);
      writer.write (dso);
      
      assert (!data_sptr->empty ());
   }
   else
   {
      ohm_util_TRACE_WARNING ("No document available");
   }
   
   return data_sptr;
}



/*
==============================================================================
Name : trash_latent_transactions
==============================================================================
*/

void  DocumentClient::trash_latent_transactions ()
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);

   set_controller (Controller_TRASH);
   
   pre_document_change_task ();
   
   docc_TRACE ("CTL TRASH");
   
   VirtualMachine & virtual_machine = use_virtual_machine ();
   
   virtual_machine.run ();
   
   _transactions_manager_aptr->rollback ();
   // transactions manager is rollbacked but because it is trashed,
   // it is still in MachineState_HEAD mode
   
   _transactions_manager_aptr->clear ();

   _transactions_manager_aptr->reexecute ();
   
   virtual_machine.stop ();
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : push_document_differential
Description :
   Push a a transaction that represents the difference between 2 documents.
   This may happen in 2 cases :
   - client reverting to an older version of the document
   - client syncing its offline work.
   The transaction may not be empty.
Note :
   It would be better to push it also in the undo stack, so that this may
   be undone, in the revert case.
==============================================================================
*/

void  DocumentClient::push_document_differential (const DataMaster & tx_data)
{
   assert (!tx_data.empty ());
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   
   assert (get_controller () == Controller_NONE);
   
   set_controller (Controller_DOC_DIF);
   
   pre_document_change_task ();
   
   VirtualMachine & virtual_machine = use_virtual_machine ();
   
   // no transaction session should be started at this point
   TxDesc tx_desc = get_new_tx_desc ();
   
   virtual_machine.set_mode_no_fault ();
   virtual_machine.set_mode_direction (tx_desc.get_tx_id ());
   
   MachineCode code (tx_data.data (), tx_data.size ());
   code.set_mode_no_fault ();
   int err = code.compile ();
   assert (err == Err_OK);
   
   virtual_machine.run ();
   
   err = virtual_machine.execute (code);
   assert (err == Err_OK);
   
   virtual_machine.stop ();
   
   _transactions_manager_aptr->push_session (tx_desc, tx_data);

   MsgRouting msg_routing;
   msg_routing._self_slot_flag = false;
   msg_routing._other_slots_flag = true;
   msg_routing._network_flag = true;
      
   _parent.send (tx_data, tx_desc, msg_routing);
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : send_signal
==============================================================================
*/

bool  DocumentClient::send_signal (flip::Ref ref, archi::UInt32 type, const DataSPtr & data_sptr)
{
   check_caller_thread ();
   
   static const size_t signal_header_size
      = sizeof (ref) + sizeof (type) + sizeof (archi::UInt32);
   
   DataSPtr ctl_data_sptr = data_sptr;
   
   archi::UInt32 data_size = 0;
   
   if (ctl_data_sptr.get () != 0)
   {
      data_size = ctl_data_sptr->size ();
   }
   else
   {
      ctl_data_sptr = DataSPtr (new Data);
   }
   
   archi::UByte buf [signal_header_size];
   
   util::BinaryStreamOutputNa signal_bso (buf, signal_header_size);
   signal_bso.write (ref);
   signal_bso.write (type);
   signal_bso.write (data_size);
   
   ctl_data_sptr->insert (ctl_data_sptr->begin (), buf, buf + signal_header_size);
   
   MsgRouting msg_routing;
   msg_routing._self_slot_flag = true;
   msg_routing._other_slots_flag = true;
   msg_routing._network_flag = false;
   
   DataMaster ctl_data;
   ctl_data.assign (ctl_data_sptr);
   ctl_data.lock ();
   
   return _parent.send_ctl_msg (StdCtlMsg_SIGNAL, ctl_data, msg_routing);
}



/*
==============================================================================
Name : send_signal
==============================================================================
*/

bool  DocumentClient::send_signal (archi::UInt32 type, const DataSPtr & data_sptr)
{
   check_caller_thread ();
   
   return send_signal (0, type, data_sptr);
}



/*
==============================================================================
Name : send_signal_server
==============================================================================
*/

bool  DocumentClient::send_signal_server (flip::Ref ref, archi::UInt32 type, const DataSPtr & data_sptr)
{
   check_caller_thread ();
   
   static const size_t signal_header_size
      = sizeof (ref) + sizeof (type) + sizeof (archi::UInt32);
   
   DataSPtr ctl_data_sptr = data_sptr;
   
   archi::UInt32 data_size = 0;
   
   if (ctl_data_sptr.get () != 0)
   {
      data_size = ctl_data_sptr->size ();
   }
   else
   {
      ctl_data_sptr = DataSPtr (new Data);
   }
   
   archi::UByte buf [signal_header_size];
   
   util::BinaryStreamOutputNa signal_bso (buf, signal_header_size);
   signal_bso.write (ref);
   signal_bso.write (type);
   signal_bso.write (data_size);
   
   ctl_data_sptr->insert (ctl_data_sptr->begin (), buf, buf + signal_header_size);
   
   MsgRouting msg_routing;
   msg_routing._self_slot_flag = false;
   msg_routing._other_slots_flag = false;
   msg_routing._network_flag = true;
   
   DataMaster ctl_data;
   ctl_data.assign (ctl_data_sptr);
   ctl_data.lock ();
   
   return _parent.send_ctl_msg (StdCtlMsg_SIGNAL, ctl_data, msg_routing);
}



/*
==============================================================================
Name : send_history
==============================================================================
*/

void  DocumentClient::send_history (Object & object, const DataMaster & tx_data, const AnnotationSPtr & annotation_sptr)
{
   check_caller_thread ();
   
   DataSPtr ctl_data_sptr (new Data);
   
   util::BinaryStreamOutput bso (*ctl_data_sptr);
   
   bso.write (object.get_ref ());
   bso.write (tx_data.data (), tx_data.data () + tx_data.size ());
   bso.write (annotation_sptr->begin (), annotation_sptr->end ());
   
   DataMaster ctl_data;
   ctl_data.assign (ctl_data_sptr);
   ctl_data.lock ();
   
   MsgRouting msg_routing;
   msg_routing._self_slot_flag = false;
   msg_routing._other_slots_flag = true;
   msg_routing._network_flag = false;
   
   _parent.send_ctl_msg (StdCtlMsg_HISTORY, ctl_data, msg_routing);
}



/*
==============================================================================
Name : incoming_transaction
Description :
   The master sent a transaction. The transaction is trusted (it is guaranteed
   to work).
   Roll back all latent transaction, apply this one, and apply latent
   transaction not already applied by this execution.
==============================================================================
*/

void  DocumentClient::incoming_transaction (const DataMaster & tx_data, archi::Int64 tx_id, archi::Int64 rev_id)
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   
   ohm_mon_TRACE ("");
   
   if (_document_state != DocumentState_READY)
   {
      return;
   }
   
   set_controller (Controller_NETWORK_EXT);
   
   pre_document_change_task ();
   
   docc_TRACE1 ("CTL NETWORK TX %1m16rw0X%", tx_id);
   
   assert (!tx_data.empty ());
   
   _transactions_manager_aptr->execute_external (tx_id, tx_data, rev_id);
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : transaction_approved
Description :
   The master sent a transaction, which was issued by us.
   At this stage, the incoming transaction list is empty, and the next
   transaction to apply comes from us. Therefore we don't need to rollback
   the transactions to apply this one, but simply skip the already applied
   transaction (which is the first in our list of own transactions)
==============================================================================
*/

void  DocumentClient::transaction_approved (archi::Int64 tx_id, archi::Int64 rev_id)
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   assert (_document_state == DocumentState_READY);
   
   docc_TRACE1 ("CTL NETWORK ACK %1m16rw0X%", tx_id);
   
   bool has_latent_flag = _transactions_manager_aptr->has_latent_transactions ();
   assert (has_latent_flag);
   
   set_controller (Controller_NETWORK_ACK);
   
   pre_document_change_task ();
   
   _transactions_manager_aptr->acknowledge (tx_id, rev_id);
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : transaction_refused
Description :
   The master refused a transaction.
   Rollback all our latent transactions, skip the first (refused) one and
   reapply all the latent transactions.
==============================================================================
*/

void  DocumentClient::transaction_refused (archi::Int64 tx_id)
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   assert (_document_state == DocumentState_READY);
   
   ohm_mon_TRACE ("");
   
   docc_TRACE1 ("CTL NETWORK DENY %1m16rw0X%", tx_id);

   bool has_latent_flag = _transactions_manager_aptr->has_latent_transactions ();
   assert (has_latent_flag);
   
   set_controller (Controller_NETWORK_DENY);
   
   pre_document_change_task ();
   
   _transactions_manager_aptr->deny (tx_id);
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : local_transaction
Description :
   The user sent a transaction from another thread.
   Roll back all latent transaction, apply this one, and apply latent
   transaction not already applied by this execution.
==============================================================================
*/

void  DocumentClient::local_transaction (const DataMaster & tx_data, const TxDesc & tx_desc)
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   // Document might not be ready but this is valid
   
   set_controller (Controller_LOCAL);
   
   pre_document_change_task ();
   
   docc_TRACE1 ("CTL LOCAL TX %1m16rw0X%", tx_desc.get_tx_id ());
   
   if (tx_desc.is_public ())
   {
      // if 'tx_data' is empty then this might be a session revert
      
      _transactions_manager_aptr->replace_execute_local (tx_desc, tx_data);
   }
   else
   {
      assert (!tx_data.empty ());
   
      _transactions_manager_aptr->push_execute_local (tx_desc, tx_data);
   }
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");
}



/*
==============================================================================
Name : incoming_ctl_msg
Description :
   The master sent a control message.
==============================================================================
*/

void  DocumentClient::incoming_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data)
{
   check_caller_thread ();
   
   if (type == StdCtlMsg_SET_USER_ID)
   {
      util::BinaryStreamInput bsi (ctl_data.data (), ctl_data.size ());
   
      std::string user_name;
      bsi.read (user_name);
      
      archi::UInt32 user_id = 0;
      bsi.read (user_id);
      
      docc_TRACE2 ("SET USER ID %1% %2%", user_name.c_str (), user_id);
      
      set_user_id (user_name, user_id);
   }
   else if (type == StdCtlMsg_SET_DOC)
   {
      docc_TRACE ("SET DOC");
      
      read_document (ctl_data);
      
      _document_state = DocumentState_READY;
      
      // we have to correct a flaw here
      
      /*{
         ListenerPtrArr::iterator it = _listener_ptr_arr.begin ();
         const ListenerPtrArr::iterator it_end = _listener_ptr_arr.end ();
         
         for (; it != it_end ; ++it)
         {
            ClientListener * listener_ptr = *it;
            listener_ptr->notify_doc_set ();
         }
      }*/
   }
   else if (type == StdCtlMsg_DOC_OK)
   {
      // sent in the case where client is at the right rev_id
      // (offline work, not used any more)
      
      docc_TRACE ("DOC OK");
      
      _document_state = DocumentState_READY;

      /*{
         ListenerPtrArr::iterator it = _listener_ptr_arr.begin ();
         const ListenerPtrArr::iterator it_end = _listener_ptr_arr.end ();
         
         for (; it != it_end ; ++it)
         {
            ClientListener * listener_ptr = *it;
            listener_ptr->notify_doc_set ();
         }
      }*/
   }
   else if (type == StdCtlMsg_REQ_REM_LAT)
   {
      docc_TRACE ("RESEND LAT");
      
      resend_latent_transactions ();
   }
   else if (type == StdCtlMsg_SIGNAL)
   {
      docc_TRACE ("SERVER SIGNAL");
      
      local_ctl_msg (type, ctl_data);
   }
   else
   {
      // no custom anymore
      
      assert (false);
   }

   /*{
      docc_TRACE ("CTL CUSTOM");
      
      ListenerPtrArr::iterator it = _listener_ptr_arr.begin ();
      const ListenerPtrArr::iterator it_end = _listener_ptr_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         ClientListener * listener_ptr = *it;
         listener_ptr->notify_incoming_ctl_msg (type, data_ptr, data_size);
      }
   }*/
}



/*
==============================================================================
Name : local_ctl_msg
==============================================================================
*/

void  DocumentClient::local_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data)
{
   if (type == StdCtlMsg_SIGNAL)
   {
      docc_TRACE ("SIGNAL");
      
      if (_document_observer_ptr != 0)
      {
         util::BinaryStreamInput bsi (ctl_data.data (), ctl_data.size ());
         
         flip::Ref ref = 0;
         bsi.read (ref);
         
         Object * object_ptr = 0;
         bool send_flag = true;
         
         if (ref != 0)
         {
            object_ptr = get_object (ref);
            
            // don't send msg if the object does not exists anymore
            send_flag = object_ptr != 0;
         }
         
         if (send_flag)
         {
            archi::UInt32 sig_type = 0;
            bsi.read (sig_type);
            
            archi::UInt32 sig_size = 0;
            bsi.read (sig_size);
            
            size_t sig_pos = bsi.tell ();
            
            DataMaster signal_data = ctl_data.subdata (sig_pos, sig_size);
            
            bsi.advance (sig_size);
            
            _document_observer_ptr->signal (object_ptr, sig_type, signal_data);
         }
      }
   }
   else if (type == StdCtlMsg_HISTORY)
   {
      docc_TRACE ("HISTORY");
      
      util::BinaryStreamInput bsi (ctl_data.data (), ctl_data.size ());
      
      // only "main thread"

      if (get_thread_id () == 0)
      {
         flip::Ref ref = 0;
         bsi.read (ref);
         
         Object * object_ptr = get_object (ref);
         
         if (object_ptr != 0)
         {
            archi::UInt32 tx_size = 0;
            bsi.read (tx_size);
            
            size_t tx_pos = bsi.tell ();
            
            DataMaster tx_data = ctl_data.subdata (tx_pos, tx_size);
            
            bsi.advance (tx_size);
            
            AnnotationSPtr annotation_sptr (new Annotation);
            
            bsi.read (*annotation_sptr);

            Scribable * scribable_ptr
               = dynamic_cast <Scribable *> (object_ptr);
            assert (scribable_ptr != 0);
            
            scribable_ptr->push_history (tx_data, annotation_sptr);
         }
      }
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : read_document
==============================================================================
*/

/*void   DocumentClient::read_document (const std::string & document)
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   
   assert (!document.empty ());
   
   DocumentReader reader;
   reader.load_no_copy (document);
   
   Err err = reader.read ();
   assert (err == Err_OK);
   
   archi::Int64 rev_id = reader.get_rev_id ();
   _transactions_manager_aptr->set_rev_id (rev_id);
   
   docc_TRACE1 ("READ DOC REV %1%", rev_id);
      
   ObjectManager & object_manager = use_object_manager ();

   Object * root_ptr = object_manager.get_root ();
   
   if (root_ptr == 0)
   {
      set_controller (Controller_DOC_READ);
      
      pre_document_change_task ();
      
      object_manager.create_root ();
      
      err = reader.exec (*this);
      assert (err == Err_OK);
      
      post_document_change_task ();
      
      set_controller (Controller_NONE);
   }
   else
   {
      std::string cur_document;
      write_document (cur_document);
      
      DocumentDifferentiator document_diff;
      document_diff.load_src_no_copy (cur_document);
      document_diff.load_dst_no_copy (document);
      
      Err err = document_diff.differentiate ();
      assert (err == Err_OK);
      
      set_controller (Controller_DOC_READ);
      
      pre_document_change_task ();
      
      VirtualMachine & virtual_machine = use_virtual_machine ();
      virtual_machine.run ();
      
      _transactions_manager_aptr->rollback ();
      _machine_state = MachineState_ROLLBACKED;

      virtual_machine.stop ();
   
      err = document_diff.exec (*this);
      assert (err == Err_OK);
      
      assert (_machine_state == MachineState_ROLLBACKED);

      virtual_machine.run ();

      _transactions_manager_aptr->reexecute ();
      _machine_state = MachineState_HEAD;
      
      virtual_machine.stop ();
      
      post_document_change_task ();
      
      set_controller (Controller_NONE);
   }
}*/



/*
==============================================================================
Name : read_document
==============================================================================
*/

void  DocumentClient::read_document (const DataMaster & doc_data)
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   assert (doc_data.size () > 8);
   
   const archi::UByte * data_ptr = doc_data.data ();
   size_t data_size = doc_data.size ();
   
   bool osf_flag
      = (data_ptr [0] == '%')
      && (data_ptr [1] == 'O')
      && (data_ptr [2] == 'S')
      && (data_ptr [3] == 'F')
   ;
   
   bool zlib_flag
      = (data_ptr [0] == 'z')
      && (data_ptr [1] == 'l')
      && (data_ptr [2] == 'i')
      && (data_ptr [3] == 'b')
   ;
   
   assert (osf_flag || zlib_flag);
   
   flip::Data alt_data;
   
   if (zlib_flag)
   {
      archi::UInt32 alt_data_size = 0;
      alt_data_size += (data_ptr [4] << 24);
      alt_data_size += (data_ptr [5] << 16);
      alt_data_size += (data_ptr [6] << 8);
      alt_data_size += (data_ptr [7]);
      
      alt_data.resize (alt_data_size);
      
      uLongf dest_len = alt_data_size;
   
      int err = uncompress (
         &alt_data [0], &dest_len,
         &data_ptr [8], data_size - 8
      );
      
      assert (err == Z_OK);
      assert (dest_len == alt_data_size);
      
      data_ptr = &alt_data [0];
      data_size = alt_data_size;
      
      assert (data_ptr [0] == '%');
      assert (data_ptr [1] == 'O');
      assert (data_ptr [2] == 'S');
      assert (data_ptr [3] == 'F');
   }
   
   DocumentReader reader;
   reader.load_no_copy (data_ptr, data_size);
   
   int err = reader.read ();
   assert (err == Err_OK);
   
   archi::Int64 rev_id = reader.get_rev_id ();
   _transactions_manager_aptr->set_rev_id (rev_id);
      
   docc_TRACE1 ("READ DOC REV %1%", rev_id);
      
   ObjectManager & object_manager = use_object_manager ();

   Object * root_ptr = object_manager.get_root ();
   
   if (root_ptr == 0)
   {
      set_controller (Controller_DOC_READ);
      
      pre_document_change_task ();
      
      object_manager.create_root ();
      
      err = reader.exec (*this);
      assert (err == Err_OK);
      
      post_document_change_task ();
      
      set_controller (Controller_NONE);
   }
   else
   {
      DataSPtr src_sptr = write_document ();
      
      Data & src_data = *src_sptr;
      assert (!src_data.empty ());
      
      DocumentDifferentiator document_diff;
      document_diff.load_src_no_copy (&src_data [0], src_data.size ());
      document_diff.load_dst_no_copy (data_ptr, data_size);
      
      int err = document_diff.differentiate ();
      assert (err == Err_OK);
      
      set_controller (Controller_DOC_READ);
      
      pre_document_change_task ();
      
      VirtualMachine & virtual_machine = use_virtual_machine ();
      virtual_machine.run ();
      
      _transactions_manager_aptr->rollback ();
      _machine_state = MachineState_ROLLBACKED;

      virtual_machine.stop ();
   
      err = document_diff.exec (*this);
      assert (err == Err_OK);
      
      assert (_machine_state == MachineState_ROLLBACKED);

      virtual_machine.run ();

      _transactions_manager_aptr->reexecute ();
      _machine_state = MachineState_HEAD;
      
      virtual_machine.stop ();
      
      post_document_change_task ();
      
      set_controller (Controller_NONE);
   }
}



/*
==============================================================================
Name : write_document
==============================================================================
*/

DataSPtr DocumentClient::write_document ()
{
   check_caller_thread ();
   assert (_machine_state == MachineState_HEAD);
   assert (get_controller () == Controller_NONE);
   
   DataSPtr data_sptr (new Data);
   
   ObjectManager & object_manager = use_object_manager ();
   
   set_controller (Controller_DOC_WRITE);
   
   pre_document_change_task ();
   
   if (object_manager.get_root () != 0)
   {
      VirtualMachine & virtual_machine = use_virtual_machine ();
      virtual_machine.run ();
      
      _transactions_manager_aptr->rollback ();
      _machine_state = MachineState_ROLLBACKED;
      
      DataStreamOutput dso;
      dso.set_mem_output (*data_sptr);
      dso.set_filter_zlib ();
      
      DocumentWriter writer (*this);
      writer.write (dso);
      
      assert (!data_sptr->empty ());

      assert (_machine_state == MachineState_ROLLBACKED);

      _transactions_manager_aptr->reexecute ();
      _machine_state = MachineState_HEAD;
      
      virtual_machine.stop ();
   }
   else
   {
      ohm_util_TRACE_WARNING ("No document available");
   }
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   return data_sptr;
}



/*
==============================================================================
Name : resend_latent_transactions
==============================================================================
*/

void  DocumentClient::resend_latent_transactions ()
{
   assert (_document_state == DocumentState_READY);

   _transactions_manager_aptr->resend_latent_transactions ();
}



/*
==============================================================================
Name : get_rev_id
==============================================================================
*/

archi::Int64   DocumentClient::get_rev_id ()
{
   if (_transactions_manager_aptr.get () == 0)
   {
      return 0;
   }
   else
   {
      return _transactions_manager_aptr->get_rev_id ();
   }
}



/*
==============================================================================
Name : notify_connecting
==============================================================================
*/

void  DocumentClient::notify_connecting ()
{
   /*ListenerPtrArr::iterator it = _listener_ptr_arr.begin ();
   const ListenerPtrArr::iterator it_end = _listener_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientListener * listener_ptr = *it;
      listener_ptr->notify_connecting ();
   }*/
}



/*
==============================================================================
Name : notify_connected
==============================================================================
*/

void  DocumentClient::notify_connected ()
{
   /*ListenerPtrArr::iterator it = _listener_ptr_arr.begin ();
   const ListenerPtrArr::iterator it_end = _listener_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientListener * listener_ptr = *it;
      listener_ptr->notify_connected ();
   }*/
}



/*
==============================================================================
Name : notify_disconnected
==============================================================================
*/

void  DocumentClient::notify_disconnected ()
{
   /*ListenerPtrArr::iterator it = _listener_ptr_arr.begin ();
   const ListenerPtrArr::iterator it_end = _listener_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientListener * listener_ptr = *it;
      listener_ptr->notify_disconnected ();
   }*/
   
   _document_state = DocumentState_VOID;
}



/*
==============================================================================
Name : add
Description :
   Mark the current object as in the object that want to receive transaction
   session history.
   This must be called before a transaction session starts.
   The object (which must be an 'HistoryMaster' will receive the transaction
   at the end of the session, if it is commited)
==============================================================================
*/

void  DocumentClient::add (Scribe & scribe)
{
   assert (_tx_session_state == TxSessionState_NONE);
   assert (&scribe != 0);
   
   assert (_scribe_ptr_set.find (&scribe) == _scribe_ptr_set.end ());
   
   _scribe_ptr_set.insert (&scribe);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  DocumentClient::remove (Scribe & scribe)
{
   // no _tx_session_state check
   assert (&scribe != 0);
   
   // no scribe _scribe_ptr_set existence check
   
   _scribe_ptr_set.erase (&scribe);
}



/*
==============================================================================
Name : start_session
Description :
   start a transaction session
   Returns 'Err_VM_TX_SESSION_COLLISION' if a transaction session was already
   started. In that case the client must internally revert its transaction
   session request (the consequence being not to fill transaction nor calling
   commit or revert)
==============================================================================
*/

int   DocumentClient::start_session ()
{
   if (_tx_session_state != TxSessionState_NONE)
   {
      return Err_VM_TX_SESSION_COLLISION;
   }
   else
   {
      _tx_session_state = TxSessionState_STARTED;
      _tx_desc.begin_public (_tx_id_generator_aptr->get ());
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : is_in_transaction_session
==============================================================================
*/

bool  DocumentClient::is_in_transaction_session ()
{
   return (_tx_session_state != TxSessionState_NONE);
}



/*
==============================================================================
Name : commit_session
Description :
   commit the transaction session
==============================================================================
*/

void  DocumentClient::commit_session ()
{
   assert (_tx_session_state == TxSessionState_STARTED);
   
   Controller controller = get_controller ();
   
   assert (
      (controller == Controller_NONE)
      || (controller == Controller_SELF)
   );
   
   if (get_controller () == Controller_SELF)
   {
      end_transaction ();
   }
   
   DataMaster tx_data = end_transaction_session ();
   
   // fill our scribes
   
   {
      ScribePtrSet::iterator it = _scribe_ptr_set.begin ();
      const ScribePtrSet::iterator it_end = _scribe_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Scribe * scribe_ptr = *it;
         
         scribe_ptr->write_history (tx_data);
      }
      
      _scribe_ptr_set.clear ();
   }

   _tx_session_state = TxSessionState_NONE;
   _tx_desc.end_public ();
   
   // don't call 'notify_post_user_inputs_tasks' as we want the
   // document_changed to be called outside of events
}



/*
==============================================================================
Name : revert_session
Description :
   revert the transaction session.
   This will search for the current tx_desc in the transaction managers and
   remove each of them.
==============================================================================
*/

void  DocumentClient::revert_session ()
{
   assert (_tx_session_state == TxSessionState_STARTED);
   
   Controller controller = get_controller ();
   
   assert (
      (controller == Controller_NONE)
      || (controller == Controller_SELF)
      || (controller == Controller_DOC_RELEASE)
   );
   
   if (get_controller () == Controller_SELF)
   {
      end_transaction ();
   }
   
   if (get_controller () != Controller_DOC_RELEASE)
   {
      revert_transaction_session ();
   }
   
   {
      ScribePtrSet::iterator it = _scribe_ptr_set.begin ();
      const ScribePtrSet::iterator it_end = _scribe_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Scribe * scribe_ptr = *it;
         
         scribe_ptr->revert_history ();
      }
      
      _scribe_ptr_set.clear ();
   }
   
   _tx_session_state = TxSessionState_NONE;
   _tx_desc.end_public ();
}



/*
==============================================================================
Name : notify_pre_user_inputs_tasks
Description :
   Check that everything is in the right state before we work on the document.
==============================================================================
*/

void  DocumentClient::notify_pre_user_inputs_tasks ()
{
   assert (get_controller () == Controller_NONE);
   assert (!_need_document_changed_notification_flag);
   assert (!_in_user_inputs_task_flag);
   
   _in_user_inputs_task_flag = true;
}



/*
==============================================================================
Name : notify_post_user_inputs_tasks
Description :
   send a transaction by ending it if a transaction is available.
   This is called repeatedly after each end of each function in a run loop
   dealing with a document.
==============================================================================
*/

void  DocumentClient::notify_post_user_inputs_tasks ()
{
   assert (_in_user_inputs_task_flag);
   
   {
      // _controller might be Controller_NONE
      // in that case there is nothing to send
      
      if (get_controller () == Controller_SELF)
      {
         end_transaction ();
      }
   }
   
   if (_need_document_changed_notification_flag)
   {
      set_controller (Controller_SELF);
      
      post_document_change_task ();

      set_controller (Controller_NONE);
   }
   
   if (_document_state == DocumentState_READY)
   {
      assert (use_object_manager ().is_recycle_bin_empty ());
   }
   
   _in_user_inputs_task_flag = false;
}



/*
==============================================================================
Name : push_send_transaction
Description :
   Push and send a transaction.
   This is called from the undo/redo manager.
==============================================================================
*/

void  DocumentClient::push_send_transaction (const DataMaster & tx_data, bool normal_direction_flag)
{
   TxDesc tx_desc = get_new_tx_desc ();
      
   if (!normal_direction_flag)
   {
      tx_desc.inverse_direction ();
   }
   
   _transactions_manager_aptr->push_session (
      tx_desc, tx_data
   );
   
   MsgRouting msg_routing;
   msg_routing._self_slot_flag = false;
   msg_routing._other_slots_flag = true;
   msg_routing._network_flag = true;

   _parent.send (tx_data, tx_desc, msg_routing);
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  DocumentClient::trace () const
{
   DocumentBase::trace ();
   
   _transactions_manager_aptr->trace ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_check_user_inputs
==============================================================================
*/

void  DocumentClient::do_check_user_inputs ()
{
   if (get_controller () != Controller_DOC_RELEASE)
   {
      // If this fails, this mean that the original caller
      // (process, etc.) did not enclose its call in a
      // notify_pre_user_inputs_tasks / notify_post_user_inputs_tasks
      // In most case, you can use a flip::DocumentClient::AutoUserInputs
      // as a guard that will handle this for you.

      assert (_in_user_inputs_task_flag);

      // Did you take the return of 'start_session' into account ?
      assert (is_in_transaction_session ());
   }
}



/*
==============================================================================
Name : push_transaction
Note :
   Push a transaction in the transaction manager.
   This is done when a transaction end is reached, in the local and public
   case. In the public case (that is a transaction session is finished),
   'concat_transaction' will be called just after.
==============================================================================
*/

void  DocumentClient::push_transaction (const DataMaster & tx_data, const TxDesc & tx_desc)
{
   assert (_machine_state == MachineState_HEAD);

   _transactions_manager_aptr->push (tx_desc, tx_data);
}



/*
==============================================================================
Name : concat_transaction
Description :
   Collect all transaction matching 'tx_desc'. This marks the end of a
   transaction session, and the resulting session is going to be sent
   to the other slots as well as the network.
==============================================================================
*/

DataMaster  DocumentClient::concat_transaction (TxDesc & tx_desc)
{
   assert (_machine_state == MachineState_HEAD);

   return _transactions_manager_aptr->concat (tx_desc);
}



/*
==============================================================================
Name : revert_transaction
Description :
   Collect all transaction matching 'tx_desc' and delete them. This marks the
   end of a transaction session.
==============================================================================
*/

void  DocumentClient::revert_transaction (TxDesc & tx_desc)
{
   assert (_machine_state == MachineState_HEAD);

   return _transactions_manager_aptr->revert (tx_desc);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_user_id
==============================================================================
*/

void  DocumentClient::set_user_id (const std::string & user_name, archi::Int32 user_id)
{
   DocumentBase::set_user_id (user_name, user_id);
   
   if (use_user_manager ().get_self_user_name () == user_name)
   {
      if (_transactions_manager_aptr.get () == 0)
      {
         _transactions_manager_aptr = std::auto_ptr <TransactionsManager> (
            new TransactionsManager (*this, use_virtual_machine (), user_id, get_thread_id ())
         );
         
         _tx_id_generator_aptr = std::auto_ptr <TxIdGenerator> (
            new TxIdGenerator (user_id, get_thread_id ())
         );
      }
      
      docc_TRACE1 ("set user_id=%1%", user_id);
   }
}



/*
==============================================================================
Name : get_new_tx_desc
==============================================================================
*/

TxDesc   DocumentClient::get_new_tx_desc ()
{
   check_caller_thread ();
   
   // we are in a transaction session
   assert (_tx_session_state == TxSessionState_NONE);
   
   TxDesc tx_desc;
   tx_desc.begin_public (_tx_id_generator_aptr->get ());
   
   return tx_desc;
}



/*
==============================================================================
Name : end_transaction
Description :
   Mark the end of a transaction in a transaction session.
   The transaction is only sent to the other slots (that is not on the
   network)
==============================================================================
*/

void  DocumentClient::end_transaction ()
{
   set_controller (Controller_NONE);
   
   docc_TRACE ("CTL NONE");

   std::auto_ptr <Data> tx_data_aptr = get_tx_data_aptr ();
   
   if (tx_data_aptr.get () != 0)
   {
      if (tx_data_aptr->size () > 0)
      {
         _tx_desc.increment_local ();
         
         DataMaster tx_data;
         tx_data.assign (tx_data_aptr);
         
         push_transaction (tx_data, _tx_desc);
         
         MsgRouting msg_routing;
         msg_routing._self_slot_flag = false;
         msg_routing._other_slots_flag = true;
         msg_routing._network_flag = false;
         
         _parent.send (tx_data, _tx_desc, msg_routing);
         
         _need_document_changed_notification_flag = true;
      }
   }
}



/*
==============================================================================
Name : end_transaction_session
Description :
   Mark the end of a transaction session.
   All transactions matching the current tx_desc are concatened into a single
   transaction, which is sent to the other slots and to the network.
==============================================================================
*/

DataMaster  DocumentClient::end_transaction_session ()
{
   set_controller (Controller_SELF);
   
   DataMaster tx_data = concat_transaction (_tx_desc);
   assert (_tx_desc.is_public ());
   
   bool session_empty_flag = tx_data.empty ();
   
   if (!session_empty_flag)
   {
      MsgRouting msg_routing;
      msg_routing._self_slot_flag = false;
      msg_routing._other_slots_flag = true;
      msg_routing._network_flag = false;
      
      _parent.send (tx_data, _tx_desc, msg_routing);
   }
   
   if (!session_empty_flag)
   {
      MsgRouting msg_routing;
      msg_routing._self_slot_flag = false;
      msg_routing._other_slots_flag = false;
      msg_routing._network_flag = true;
      
      _parent.send (tx_data, _tx_desc, msg_routing);
   }
   
   if (session_empty_flag)
   {
      _tx_id_generator_aptr->give_back ();
   }
   else
   {
      _need_document_changed_notification_flag = true;
   }
   
   set_controller (Controller_NONE);
   
   return tx_data;
}



/*
==============================================================================
Name : revert_transaction_session
==============================================================================
*/

void  DocumentClient::revert_transaction_session ()
{
   set_controller (Controller_SELF);
   
   revert_transaction (_tx_desc);
   assert (_tx_desc.is_public ());
   
   {
      MsgRouting msg_routing;
      msg_routing._self_slot_flag = false;
      msg_routing._other_slots_flag = true;
      msg_routing._network_flag = false;
      
      DataMaster empty_tx_data;
      
      _parent.send (empty_tx_data, _tx_desc, msg_routing);
   }
   
   _tx_id_generator_aptr->give_back ();
   _need_document_changed_notification_flag = true;
   
   set_controller (Controller_NONE);
}



/*
==============================================================================
Name : pre_document_change_task
==============================================================================
*/

void  DocumentClient::pre_document_change_task ()
{
   docc_TRACE ("PRE");
   
   assert (get_controller () != Controller_NONE);
   
   if (!use_object_manager ().is_recycle_bin_empty ())
   {
      use_object_manager ().trace_recycle_bin ();
      
      assert (false);
   }
}



/*
==============================================================================
Name : post_document_change_task
==============================================================================
*/

void  DocumentClient::post_document_change_task ()
{
   enable_post (true);
   
   docc_TRACE ("POST");
   
   assert (get_controller () != Controller_NONE);
   
   post_reorder_containers ();
   pre_sync ();
   
   Object * root_ptr = use_object_manager ().get_root ();
   
   if (_document_observer_ptr != 0)
   {
      assert (root_ptr != 0);
      _document_observer_ptr->notify_document_changed (*root_ptr);
   }
   
   use_object_manager ().empty_recycle_bin ();
   
   if (get_controller () == Controller_DOC_RELEASE)
   {
      // don't synchronize root : it does not exist anymore
   }
   else
   {
      assert (root_ptr != 0);
         
      root_ptr->synchronize ();

#if defined (OHM_FLIP_DOCUMENT_CHECK_SYNC_FLAG)
      root_ptr->check_synchronized ();
#endif
   }

   enable_post (false);

   _need_document_changed_notification_flag = false;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
