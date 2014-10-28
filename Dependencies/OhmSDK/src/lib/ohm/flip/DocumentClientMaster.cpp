/*****************************************************************************

        DocumentClientMaster.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70450

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

#include "ohm/conc/AioAdd.h"
#include "ohm/conc/AtomicIntOp.h"
#include "ohm/flip/Constants.h"
#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/DocumentClient.h"
#include "ohm/flip/private/DocumentReader.h"
#include "ohm/flip/private/DocumentClientMasterSlot.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/StdCtlMsg.h"
#include "ohm/flip/private/DocumentDifferentiator.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/SessionManagerInterface.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamOutputNa.h"
#include "ohm/util/trace.h"
#include "ohm/sys/ThreadMgr.h"

#include <cstring>
#include <cassert>



#undef OHM_FLIP_DCM_DEBUG_FLAG

#if defined (OHM_FLIP_DCM_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DCM_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define dcm_TRACE(msg_0)   ohm_util_TRACE_INFO(msg_0)
   #define dcm_TRACE1(fmt_0, arg1)  ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define dcm_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define dcm_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define dcm_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define dcm_TRACE(msg_0)   (void (0))
   #define dcm_TRACE1(fmt_0, arg1)  (void (0))
   #define dcm_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define dcm_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define dcm_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Note :
   'nbr_slot' is somehow a static property for the document managment.
==============================================================================
*/

DocumentClientMaster::DocumentClientMaster (SessionManagerInterface & session_manager, ComLayerClient & com_layer, const std::string & user_name)
:  _session_manager (session_manager)
,  _user_name (user_name)
,  _run_loop (task::RunLoopBase::get_current ())
,  _observer ("ohm.flip.DocumentClientMaster")
,  _post_man (com_layer, *this, user_name)
,  _state (State_CTORED)
,  _slot_ptr_state_arr ()
,  _rev_id (0)
{
   _observer.bind_process <
      DocumentClientMaster,
      &DocumentClientMaster::process
   > (*this);

   _run_loop.add (_observer);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentClientMaster::~DocumentClientMaster ()
{
   try
   {
      assert (_state == State_CTORED);
      
      _run_loop.remove (_observer);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : get_user_name
==============================================================================
*/

const std::string &  DocumentClientMaster::get_user_name () const
{
   return _user_name;
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  DocumentClientMaster::add (DocumentClientMasterSlot & slot)
{
   assert (_state == State_CTORED);
   assert (&slot != 0);
   
   SlotPtrState slot_ptr_state;
   slot_ptr_state._slot_ptr = &slot;
   slot_ptr_state._state = SlotState_ADDED;
   
   _slot_ptr_state_arr.push_back (slot_ptr_state);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  DocumentClientMaster::remove (DocumentClientMasterSlot & slot)
{
   assert (_state == State_SLOTS_ADDED);
   assert (&slot != 0);
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * sub_slot_ptr = slot_ptr_state._slot_ptr;
      assert (sub_slot_ptr != 0);
      
      if (&slot == sub_slot_ptr)
      {
         assert (slot_ptr_state._state == SlotState_ADDED);
         
         _slot_ptr_state_arr.erase (it);
         break;
      }
   }
   
   if (_slot_ptr_state_arr.empty ())
   {
      _state = State_CTORED;
   }
}



/*
==============================================================================
Name : notify_slots_added
==============================================================================
*/

void  DocumentClientMaster::notify_slots_added ()
{
   assert (_state == State_CTORED);
   
   _state = State_SLOTS_ADDED;
}



/*
==============================================================================
Name : init
   Call is synchronous.
==============================================================================
*/

void  DocumentClientMaster::init ()
{
   assert (_state == State_SLOTS_ADDED);
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      assert (slot_ptr_state._state == SlotState_ADDED);
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->init ();
      
      slot_ptr_state._state = SlotState_INITED;
   }
   
   _state = State_INITED;
}



/*
==============================================================================
Name : restore
   Call is synchronous.
==============================================================================
*/

void  DocumentClientMaster::restore ()
{
   assert (_state == State_INITED);
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      assert (slot_ptr_state._state == SlotState_INITED);
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->restore ();
      
      slot_ptr_state._state = SlotState_ADDED;
   }
   
   _state = State_SLOTS_ADDED;
}



/*
==============================================================================
Name : connect
Note :
   The post man is set with info on how to connect to a session.
   The post man 'connect' call will callback 'notify_connecting' and then
   'notify_connected' when the post man is connected to the session,
   ready to deliver subsequent session messages.
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::connect ()
{
   assert (_state == State_INITED);
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->notify_connecting ();
   }
   
   _post_man.connect ();
   
   task::Event ret_event = _base_port.recv ();
   
   it = _slot_ptr_state_arr.begin ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      assert (slot_ptr_state._state == SlotState_INITED);
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->notify_connected ();
      
      slot_ptr_state._state = SlotState_CONNECTED;
   }
   
   _state = State_CONNECTED;
   
   {
      std::auto_ptr <Data> data_aptr = std::auto_ptr <Data> (new Data);
      
      util::BinaryStreamOutput bso (*data_aptr);
      bso.write (_user_name);
      
      // _rev_id might be 0
      bso.write (_rev_id);
      
      MsgRouting msg_routing;
      msg_routing._self_slot_flag = false;
      msg_routing._other_slots_flag = false;
      msg_routing._network_flag = true;
      
      DataMaster ctl_data;
      ctl_data.assign (data_aptr);
      ctl_data.lock ();

      DocumentClientMasterSlot * from_slot_ptr = 0;
      send_ctl_msg (from_slot_ptr, StdCtlMsg_HELLO, ctl_data, msg_routing);
      
      dcm_TRACE1 ("HELLO %1%", rev_id);
   }
}
   


/*
==============================================================================
Name : disconnect
Note :
   The post man is set with info on how to connect to a session.
   The post man 'disconnect' call will callback 'notify_disconnected' when
   the post man is disconnected from the session.
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::disconnect ()
{
   assert (_state == State_CONNECTED);
   
   if (_state == State_CONNECTED)
   {
      _post_man.disconnect ();
      
      task::Event ret_event = _base_port.recv ();
      
      SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
      const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         SlotPtrState & slot_ptr_state = *it;
         
         assert (slot_ptr_state._state == SlotState_CONNECTED);
         
         DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
         assert (slot_ptr != 0);
         
         slot_ptr->notify_disconnected ();
         
         slot_ptr_state._state = SlotState_INITED;
      }

      //write_document ();
      //write_document_head ();
      
      _state = State_INITED;
   }
}
   


/*
==============================================================================
Name : write_document
==============================================================================
*/

void  DocumentClientMaster::write_document ()
{
   assert (_state == State_CONNECTED);
   
   assert (!_slot_ptr_state_arr.empty ());
   
   SlotPtrState & slot_ptr_state = _slot_ptr_state_arr.front ();
   
   DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
   assert (slot_ptr != 0);
   
   DataSPtr data_sptr = slot_ptr->write_document ();

   _session_manager.write_revision (data_sptr);
}



/*
==============================================================================
Name : write_document_head
==============================================================================
*/

void  DocumentClientMaster::write_document_head ()
{
   assert (_state == State_CONNECTED);
   
   assert (!_slot_ptr_state_arr.empty ());
   
   SlotPtrState & slot_ptr_state = _slot_ptr_state_arr.front ();
   
   DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
   assert (slot_ptr != 0);
   
   DataSPtr data_sptr = slot_ptr->write_document_head ();
   
   _session_manager.write_head (data_sptr);
}



/*
==============================================================================
Name : read_document
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::read_document (const DataMaster & doc_data)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_READ_DOC);
   event.push (doc_data);
   _observer.send (event);
}



/*
==============================================================================
Name : incoming_transaction
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::incoming_transaction (const DataMaster & tx_data, archi::Int64 tx_id, archi::Int64 rev_id)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_TX);
   event.push (tx_id);
   event.push (rev_id);
   event.push (tx_data);
   _observer.send (event);
}



/*
==============================================================================
Name : transaction_approved
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::transaction_approved (archi::Int64 tx_id, archi::Int64 rev_id)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_TX_ACK);
   event.push (tx_id);
   event.push (rev_id);
   _observer.send (event);
}



/*
==============================================================================
Name : transaction_refused
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::transaction_refused (archi::Int64 tx_id)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_TX_DENY);
   event.push (tx_id);
   _observer.send (event);
}



/*
==============================================================================
Name : incoming_ctl_msg
Thread Safety :
   Can be called from any thread.
==============================================================================
*/

void  DocumentClientMaster::incoming_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_CTL_MSG);
   event.push (type);
   event.push (ctl_data);
   _observer.send (event);
}



/*
==============================================================================
Name : connecting
Note :
   This function is called from the thread of ComLayerClientBase
==============================================================================
*/

void  DocumentClientMaster::notify_connecting ()
{
   /*task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_CONNECTING);
   _observer.send (event);*/
}



/*
==============================================================================
Name : notify_connected
Note :
   This function is called from the thread of ComLayerClientBase
==============================================================================
*/

void  DocumentClientMaster::notify_connected ()
{
   /*task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_CONNECTED);
   _observer.send (event);*/
   
   task::Event & event = ohm_task_EVENT_GET;
   
   _base_port.send (event);
}



/*
==============================================================================
Name : notify_disconnected
Note :
   This function is called from the thread of ComLayerClientBase
==============================================================================
*/

void  DocumentClientMaster::notify_disconnected ()
{
   /*task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_DISCONNECTED);
   _observer.send (event);*/

   task::Event & event = ohm_task_EVENT_GET;
   
   _base_port.send (event);
}



/*
==============================================================================
Name : send
Note :
   from_slot_ptr might be null if it comes from 'this'
==============================================================================
*/

void  DocumentClientMaster::send (const DocumentClientMasterSlot * from_slot_ptr, const DataMaster & tx_data, const TxDesc & tx_desc, const MsgRouting & msg_routing)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_SEND_TX);
   event.push (from_slot_ptr);
   event.push (tx_data);
   event.push (tx_desc);
   event.push (msg_routing);
   _observer.send (event);
}



/*
==============================================================================
Name : send_ctl_msg
Note :
   from_slot_ptr might be null if it comes from 'this'
==============================================================================
*/

void  DocumentClientMaster::send_ctl_msg (const DocumentClientMasterSlot * from_slot_ptr, archi::UInt32 type, const DataMaster & ctl_data, const MsgRouting & msg_routing)
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_SEND_CTL_MSG);
   event.push (from_slot_ptr);
   event.push (type);
   event.push (ctl_data);
   event.push (msg_routing);
   _observer.send (event);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process
==============================================================================
*/

void  DocumentClientMaster::process (task::Event & event)
{
   EventType type = event.pop <EventType> ();
   
   switch (type)
   {
   case EventType_READ_DOC:
      process_read_doc (event);
      break;

   case EventType_TX:
      process_tx (event);
      break;

   case EventType_TX_ACK:
      process_tx_ack (event);
      break;

   case EventType_TX_DENY:
      process_tx_deny (event);
      break;

   case EventType_CTL_MSG:
      process_ctl_msg (event);
      break;

   case EventType_SEND_TX:
      process_send_tx (event);
      break;

   case EventType_SEND_CTL_MSG:
      process_send_ctl_msg (event);
      break;

   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : exiting_process
==============================================================================
*/

void  DocumentClientMaster::exiting_process ()
{
   // nothing
}



/*
==============================================================================
Name : process_connected
==============================================================================
*/

/*void   DocumentClientMaster::process_connected (task::Event & event)
{
   assert (_state == State_INITED);
   
    // DON'T REMOVE THE FOLLOWING COMMENTED CODE
    // WE ARE KEEPING IT FOR LATER

 #if 0
   BinaryStreamOutput bso;
   bso.write (_user_name);
   
   // _rev_id might be 0
   bso.write (_rev_id);
   
   MsgRouting msg_routing;
   msg_routing._self_slot_flag = false;
   msg_routing._other_slots_flag = false;
   msg_routing._network_flag = true;
   
   DocumentClientMasterSlot * from_slot_ptr = 0;
   send_ctl_msg (from_slot_ptr, StdCtlMsg_HELLO, bso.data (), bso.size (), msg_routing);
   
   dcm_TRACE1 ("HELLO %1%", rev_id);
   
   // may have forward disconnected
   
   if (_rev_id != 0)
   {
      request_write_document_head ();
   }
   
   // we have a document, and probably offline work
   
   if (_session_manager.has_offline_work ())
   {
      assert (_session_manager.has_revision ());
            
      const std::string & document_last_rev
         = _session_manager.read_revision ();

      const std::string & document_head
         = _session_manager.read_head ();
      
      DocumentDifferentiator document_diff;
      document_diff.load_src_no_copy (document_last_rev);
      document_diff.load_dst_no_copy (document_head);
      
      Err err = document_diff.differentiate ();
      assert (err == Err_OK);
         
      SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
      const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         SlotPtrState & slot_ptr_state = *it;
         
         DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
         assert (slot_ptr != 0);
         
         slot_ptr->trash_latent_transactions ();
      }
      
      TransactionSPtr transaction_sptr = document_diff.get_transaction ();

      assert (!_slot_ptr_state_arr.empty ());
      
      if (!transaction_sptr->empty ())
      {
         DocumentClientMasterSlot * slot_ptr = _slot_ptr_state_arr.front ()._slot_ptr;
         slot_ptr->push_document_differential (transaction_sptr);
      }
   }
   else
   {
      // we will take the server document
   }
#endif
   
   // DON'T REMOVE ABOVE COMMENTED CODE
   // WE ARE KEEPING IT FOR LATER
   
   dcm_TRACE ("connect slots");
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      assert (slot_ptr_state._state == SlotState_INITED);
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->notify_connected ();
      
      slot_ptr_state._state = SlotState_CONNECTED;
   }
   
   _state = State_CONNECTED;
   
   {
      BinaryStreamOutput bso;
      bso.write (_user_name);
      
      // _rev_id might be 0
      bso.write (_rev_id);
      
      MsgRouting msg_routing;
      msg_routing._self_slot_flag = false;
      msg_routing._other_slots_flag = false;
      msg_routing._network_flag = true;
      
      DocumentClientMasterSlot * from_slot_ptr = 0;
      send_ctl_msg (from_slot_ptr, StdCtlMsg_HELLO, bso.data (), bso.size (), msg_routing);
      
      dcm_TRACE1 ("HELLO %1%", rev_id);
   }
}*/



/*
==============================================================================
Name : process_read_doc
==============================================================================
*/

void  DocumentClientMaster::process_read_doc (task::Event & event)
{
   assert (_state == State_CONNECTED);

   DataMaster doc_data = event.pop <DataMaster> ();
   
   DocumentReader reader;
   reader.load_no_copy (doc_data.data (), doc_data.size ());

   int err = reader.read ();
   assert (err == Err_OK);

   _rev_id = reader.get_rev_id ();

   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->read_document (doc_data);
   }
}



/*
==============================================================================
Name : process_tx
==============================================================================
*/

void  DocumentClientMaster::process_tx (task::Event & event)
{
   assert (_state == State_CONNECTED);

   archi::Int64 tx_id = event.pop <archi::Int64> ();
   archi::Int64 rev_id = event.pop <archi::Int64> ();
   DataMaster tx_data = event.pop <DataMaster> ();
   
   _rev_id = rev_id;
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->incoming_transaction (tx_data, tx_id, rev_id);
   }
}



/*
==============================================================================
Name : process_tx_ack
==============================================================================
*/

void  DocumentClientMaster::process_tx_ack (task::Event & event)
{
   assert (_state == State_CONNECTED);
   
   archi::Int64 tx_id = event.pop <archi::Int64> ();
   archi::Int64 rev_id = event.pop <archi::Int64> ();

   _rev_id = rev_id;

   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->transaction_approved (tx_id, rev_id);
   }
}



/*
==============================================================================
Name : process_tx_deny
==============================================================================
*/

void  DocumentClientMaster::process_tx_deny (task::Event & event)
{
   assert (_state == State_CONNECTED);
   
   archi::Int64 tx_id = event.pop <archi::Int64> ();

   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->transaction_refused (tx_id);
   }
}



/*
==============================================================================
Name : process_ctl_msg
==============================================================================
*/

void  DocumentClientMaster::process_ctl_msg (task::Event & event)
{
   assert (_state == State_CONNECTED);
   
   archi::UInt32 ctl_msg_type = event.pop <archi::UInt32> ();
   DataMaster ctl_data = event.get <DataMaster> ();
   
   assert (!ctl_data.empty ());
   
   SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
   const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      SlotPtrState & slot_ptr_state = *it;
      
      DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
      assert (slot_ptr != 0);
      
      slot_ptr->incoming_ctl_msg (ctl_msg_type, ctl_data);
   }
}



/*
==============================================================================
Name : process_send_tx
==============================================================================
*/

void  DocumentClientMaster::process_send_tx (task::Event & event)
{
   // might be null
   const DocumentClientMasterSlot * from_slot_ptr
      = event.pop <const DocumentClientMasterSlot *> ();
   
   DataMaster tx_data = event.pop <DataMaster> ();
   TxDesc tx_desc = event.pop <TxDesc> ();
   MsgRouting msg_routing = event.pop <MsgRouting> ();
      
   if (_state == State_CONNECTED)
   {
      SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
      const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         SlotPtrState & slot_ptr_state = *it;
         
         DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
         assert (slot_ptr != 0);
         
         bool self_slot_flag = slot_ptr == from_slot_ptr;
         
         if (self_slot_flag)
         {
            if (msg_routing._self_slot_flag)
            {
               slot_ptr->local_transaction (tx_data, tx_desc);
            }
         }
         else
         {
            if (msg_routing._other_slots_flag)
            {
               slot_ptr->local_transaction (tx_data, tx_desc);
            }
         }
      }
      
      if (msg_routing._network_flag)
      {
         _post_man.send (tx_data, tx_desc, msg_routing);
      }
   }
   else
   {
      ohm_util_TRACE_WARNING (
         "Spurious tx after deconnection. "
         "Dropping tx."
      );
   }
}



/*
==============================================================================
Name : process_send_ctl_msg
==============================================================================
*/

void  DocumentClientMaster::process_send_ctl_msg (task::Event & event)
{
   // might be null
   const DocumentClientMasterSlot * from_slot_ptr
      = event.pop <const DocumentClientMasterSlot *> ();
   
   archi::UInt32 type = event.pop <archi::UInt32> ();
   DataMaster ctl_data = event.pop <DataMaster> ();
   MsgRouting msg_routing = event.pop <MsgRouting> ();
   
   if (_state == State_CONNECTED)
   {
      assert (!ctl_data.empty ());
   
      SlotPtrStateArr::iterator it = _slot_ptr_state_arr.begin ();
      const SlotPtrStateArr::iterator it_end = _slot_ptr_state_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         SlotPtrState & slot_ptr_state = *it;
         
         DocumentClientMasterSlot * slot_ptr = slot_ptr_state._slot_ptr;
         assert (slot_ptr != 0);
         
         bool self_slot_flag = slot_ptr == from_slot_ptr;
         
         if (self_slot_flag)
         {
            if (msg_routing._self_slot_flag)
            {
               slot_ptr->local_ctl_msg (type, ctl_data);
            }
         }
         else
         {
            if (msg_routing._other_slots_flag)
            {
               slot_ptr->local_ctl_msg (type, ctl_data);
            }
         }
      }
      
      if (msg_routing._network_flag)
      {
         _post_man.send_ctl_msg (type, ctl_data, msg_routing);
      }
   }
   else
   {
      ohm_util_TRACE_WARNING1 (
         "Spurious ctl msg after deconnection. "
         "Ctl Msg number was %1% (see ohm/flip/private/StdCtlMsg.h)",
         type
      );
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
