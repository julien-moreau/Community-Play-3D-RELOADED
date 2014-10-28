/*****************************************************************************

        DocumentClientMasterSlot.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66632

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

#include "ohm/flip/private/DocumentClientMasterSlot.h"
#include "ohm/flip/private/DocumentReader.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/DocumentClient.h"
#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/task/IpcData.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamOutputNa.h"
#include "ohm/util/trace.h"

#include <cassert>



#undef OHM_FLIP_DCMS_DEBUG_FLAG

#if defined (OHM_FLIP_DCMS_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DCMS_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define dcms_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
   #define dcms_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define dcms_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define dcms_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define dcms_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define dcms_TRACE(msg_0)  (void (0))
   #define dcms_TRACE1(fmt_0, arg1) (void (0))
   #define dcms_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define dcms_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define dcms_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif



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

DocumentClientMasterSlot::~DocumentClientMasterSlot ()
{
   try
   {
      if (_mode == Mode_GUEST)
      {
         //_bi_dir_port_ptr->stop ();
         
         ThreadBase::kill ();
      }
      else if (_mode == Mode_HOST)
      {
//       _bi_dir_port_ptr->stop ();
         
         ThreadBase::kill ();
      }
      
      assert (_document_ptr == 0);
      
      if (_bi_dir_port_ptr != 0)
      {
         delete _bi_dir_port_ptr;
         _bi_dir_port_ptr = 0;
      }
      
      if (_mode != Mode_HOST)
      {
         _run_loop.remove (_observer);
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : create
Description :
   Create the slot and attach it to the document client master.
==============================================================================
*/

DocumentClientMasterSlot * DocumentClientMasterSlot::create (DocumentClientMaster & document_master, int thread_id)
{
   DocumentClientMasterSlot * slot_ptr
      = new DocumentClientMasterSlot (document_master, thread_id);
   
   return slot_ptr;
}



/*
==============================================================================
Name : create_host
Description :
   Create the host slot and attach it to the document client master.
==============================================================================
*/

DocumentClientMasterSlot * DocumentClientMasterSlot::create_host (task::IpcPortBiDir * port_ptr, DocumentClientMaster & document_master, int thread_id)
{
   DocumentClientMasterSlot * slot_ptr
      = new DocumentClientMasterSlot (port_ptr, document_master, thread_id);
   
   return slot_ptr;
}



/*
==============================================================================
Name : create_guest
Description :
   Create the host slot and attach it to the document client master.
==============================================================================
*/

DocumentClientMasterSlot * DocumentClientMasterSlot::create_guest (const char * port_name_0, const std::string & user_name, int thread_id)
{
   DocumentClientMasterSlot * slot_ptr
      = new DocumentClientMasterSlot (port_name_0, user_name, thread_id);
   
   return slot_ptr;
}



/*
==============================================================================
Name : register_thread
==============================================================================
*/

void  DocumentClientMasterSlot::register_thread ()
{
   assert (_thread == sys::ThreadMgr::get_invalid_thread_id ());
   
   _thread = sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : get_thread_id
==============================================================================
*/

int   DocumentClientMasterSlot::get_thread_id () const
{
   return _thread_id;
}



/*
==============================================================================
Name : get_port
==============================================================================
*/

task::IpcPortBiDir * DocumentClientMasterSlot::get_port () const
{
   assert (_bi_dir_port_ptr != 0);
   
   return _bi_dir_port_ptr;
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  DocumentClientMasterSlot::init ()
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   assert (is_sync_call ());
   
   if (_mode == Mode_NORMAL)
   {
      do_init ();
   }
   else if (_mode == Mode_HOST)
   {
      assert (_bi_dir_port_ptr != 0);
      
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (EventType_INIT));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
      
      task::IpcData ipc_data;
      task::IpcPortBiDir::Reply ret = _bi_dir_port_ptr->recv (ipc_data);
      assert (ret == task::IpcPortBiDir::Reply_DATA_AVAILABLE);
      
      util::BinaryStreamInput bsi (ipc_data.data (), ipc_data.size ());
      
      archi::UByte type = 0;
      bsi.read (type);
      assert (type == ToHostEventType_STATE_INITED);
   }
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  DocumentClientMasterSlot::restore ()
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   assert (is_sync_call ());
   
   if (_mode == Mode_NORMAL)
   {
      do_restore ();
   }
   else if (_mode == Mode_HOST)
   {
      assert (_bi_dir_port_ptr != 0);
      
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (EventType_RESTORE));
      
      bool ok_flag = _bi_dir_port_ptr->send (bso.data (), bso.size ());
      
      // if send failed, then consider that the audio engine is dead,
      // and therefore we don't need to disconnect flip
      
      if (ok_flag)
      {
         task::IpcData ipc_data;
         task::IpcPortBiDir::Reply ret = _bi_dir_port_ptr->recv (ipc_data);
         assert (ret == task::IpcPortBiDir::Reply_DATA_AVAILABLE);
         
         util::BinaryStreamInput bsi (ipc_data.data (), ipc_data.size ());
         
         archi::UByte type = 0;
         bsi.read (type);
         assert (type == ToHostEventType_STATE_CTORED);
      }
   }
}



/*
==============================================================================
Name : halt
Description :
   Used in HOST case when the guest has crashed
==============================================================================
*/

void  DocumentClientMasterSlot::halt ()
{
   assert (_mode == Mode_HOST);
   
   // resume from async mode
   ThreadBase::kill ();
}



/*
==============================================================================
Name : write_document
==============================================================================
*/

DataSPtr DocumentClientMasterSlot::write_document ()
{
   assert (_mode == Mode_NORMAL);
   assert (is_sync_call ());
   
   assert (_document_ptr != 0);
   
   dcms_TRACE ("WRITE");
   
   return _document_ptr->write_document ();
}



/*
==============================================================================
Name : write_document_head
==============================================================================
*/

DataSPtr DocumentClientMasterSlot::write_document_head ()
{
   assert (_mode == Mode_NORMAL);
   assert (is_sync_call ());
   
   assert (_document_ptr != 0);
   
   dcms_TRACE ("WRITE HEAD");
   
   return _document_ptr->write_document_head ();
}



/*
==============================================================================
Name : get_document_ptr
==============================================================================
*/

DocumentClient *  DocumentClientMasterSlot::get_document_ptr () const
{
   // _document_ptr might be null when restoring
   
   return _document_ptr;
}



/*
==============================================================================
Name : trash_latent_transactions
==============================================================================
*/

void  DocumentClientMasterSlot::trash_latent_transactions ()
{
   // to do
   
   assert (false);
}



/*
==============================================================================
Name : push_document_differential
==============================================================================
*/

void  DocumentClientMasterSlot::push_document_differential (const DataMaster & tx_data)
{
   // to do
   
   assert (false);
}



/*
==============================================================================
Name : send
Description :
   This is sent from the document associated from this slot.
   The message originally comes from a document in a thread, which call
   its post man which send it here.
   Then this slot send it to its master (which will in turn send it over
   internet and locally)
   Returns true iff was sent.
==============================================================================
*/

bool  DocumentClientMasterSlot::send (const DataMaster & tx_data, const TxDesc & tx_desc, const MsgRouting & msg_routing)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_GUEST));
   
   if (_state != State_CONNECTED)
   {
      dcms_TRACE ("tx sent while disconnected : dropped");
      
      return false;
   }
   
   bool ok_flag = false;
   
   if (_mode == Mode_NORMAL)
   {
      _document_master_ptr->send (this, tx_data, tx_desc, msg_routing);
      
      // always succeeds
      ok_flag = true;
   }
   else if (_mode == Mode_GUEST)
   {
      assert (_mode == Mode_GUEST);
      
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (ToHostEventType_SEND));
      bso.write (tx_data.data (), tx_data.data () + tx_data.size ());
      bso.write (tx_desc);
      bso.write (msg_routing);
      
      ok_flag = _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : send_ctl_msg
Description :
   This is sent from the document associated from this slot.
   The message originally comes from a document in a thread, which call
   its post man which send it here.
   Then this slot send it to its master (which will in turn send it over
   internet)
   Returns true iff was sent.
==============================================================================
*/

bool  DocumentClientMasterSlot::send_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data, const MsgRouting & msg_routing)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_GUEST));
   
   if (_state != State_CONNECTED)
   {
      dcms_TRACE1 ("ctl msg %1% sent while disconnected : dropped", type);
      
      // this is not an error
      return true;
   }
   
   bool ok_flag = false;
   
   if (_mode == Mode_NORMAL)
   {
      _document_master_ptr->send_ctl_msg (this, type, ctl_data, msg_routing);
      
      // always succeeds
      ok_flag = true;
   }
   else if (_mode == Mode_GUEST)
   {
      dcms_TRACE1 ("Sending CTL MSG %1%", type);
      
      assert (_bi_dir_port_ptr != 0);
      
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (ToHostEventType_SEND_CTL_MSG));
      bso.write (type);
      bso.write (ctl_data.data (), ctl_data.data () + ctl_data.size ());
      bso.write (msg_routing);
      
      ok_flag = _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : incoming_transaction
Description :
   Called from DocumentClientMaster
==============================================================================
*/

void  DocumentClientMasterSlot::incoming_transaction (const DataMaster & tx_data, archi::Int64 tx_id, archi::Int64 rev_id)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_NETWORK_TX);
      event.push (tx_id);
      event.push (rev_id);
      event.push (tx_data);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (EventType_NETWORK_TX));
      bso.write (tx_id);
      bso.write (rev_id);
      bso.write (tx_data.data (), tx_data.data () + tx_data.size ());
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : transaction_approved
Description :
   Called from DocumentClientMaster
==============================================================================
*/

void  DocumentClientMasterSlot::transaction_approved (archi::Int64 tx_id, archi::Int64 rev_id)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_NETWORK_TX_ACK);
      event.push (tx_id);
      event.push (rev_id);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      static const size_t buf_size = sizeof (archi::UByte) + sizeof (tx_id) + sizeof (rev_id);
      archi::UByte buf [buf_size];
      
      util::BinaryStreamOutputNa bso (buf, buf_size);
      
      bso.write (archi::UByte (EventType_NETWORK_TX_ACK));
      bso.write (tx_id);
      bso.write (rev_id);
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : transaction_refused
Description :
   Called from DocumentClientMaster
==============================================================================
*/

void  DocumentClientMasterSlot::transaction_refused (archi::Int64 tx_id)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_NETWORK_TX_DENY);
      event.push (tx_id);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      static const size_t buf_size = sizeof (archi::UByte) + sizeof (tx_id);
      archi::UByte buf [buf_size];
      
      util::BinaryStreamOutputNa bso (buf, buf_size);
      
      bso.write (archi::UByte (EventType_NETWORK_TX_DENY));
      bso.write (tx_id);
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : local_transaction
Description :
   Called from DocumentClientMaster
==============================================================================
*/

void  DocumentClientMasterSlot::local_transaction (const DataMaster & tx_data, const TxDesc & tx_desc)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_LOCAL_TX);
      event.push (tx_desc);
      event.push (tx_data);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (EventType_LOCAL_TX));
      bso.write (tx_desc);
      bso.write (tx_data.data (), tx_data.data () + tx_data.size ());
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : incoming_ctl_msg
Description :
   Called from DocumentClientMaster
==============================================================================
*/

void  DocumentClientMasterSlot::incoming_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_NETWORK_CTL_MSG);
      event.push (type);
      event.push (ctl_data);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (EventType_NETWORK_CTL_MSG));
      bso.write (type);
      bso.write (ctl_data.data (), ctl_data.data () + ctl_data.size ());
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : local_ctl_msg
Description :
   Called from DocumentClientMaster
==============================================================================
*/

void  DocumentClientMasterSlot::local_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data)
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_LOCAL_CTL_MSG);
      event.push (type);
      event.push (ctl_data);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (EventType_LOCAL_CTL_MSG));
      bso.write (type);
      bso.write (ctl_data.data (), ctl_data.data () + ctl_data.size ());
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : read_document
==============================================================================
*/

void  DocumentClientMasterSlot::read_document (const DataMaster & doc_data)
{
   assert (!doc_data.empty ());
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_READ_DOC);
      event.push (doc_data);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (EventType_READ_DOC));
      bso.write (doc_data.data (), doc_data.data () + doc_data.size ());
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : connecting
==============================================================================
*/

void  DocumentClientMasterSlot::notify_connecting ()
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   
   if (_mode == Mode_NORMAL)
   {
      task::Event & event = ohm_task_EVENT_GET;
      event.push (EventType_CONNECTING);
      
      _observer.send (event);
   }
   else if (_mode == Mode_HOST)
   {
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (EventType_CONNECTING));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
}



/*
==============================================================================
Name : connected
==============================================================================
*/

void  DocumentClientMasterSlot::notify_connected ()
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   assert (is_sync_call ());
   
   if (_mode == Mode_NORMAL)
   {
      do_notify_connected ();
   }
   else if (_mode == Mode_HOST)
   {
      assert (_bi_dir_port_ptr != 0);
      
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (EventType_CONNECTED));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
      
      task::IpcData ipc_data;
      task::IpcPortBiDir::Reply ret = _bi_dir_port_ptr->recv (ipc_data);
      assert (ret == task::IpcPortBiDir::Reply_DATA_AVAILABLE);
      
      util::BinaryStreamInput bsi (ipc_data.data (), ipc_data.size ());
      
      archi::UByte type = 0;
      bsi.read (type);
      assert (type == ToHostEventType_STATE_CONNECTED);
      
      // switch to async mode
      ThreadBase::run ();
   }
}



/*
==============================================================================
Name : disconnected
==============================================================================
*/

void  DocumentClientMasterSlot::notify_disconnected ()
{
   assert ((_mode == Mode_NORMAL) || (_mode == Mode_HOST));
   assert (is_sync_call ());
   
   if (_mode == Mode_NORMAL)
   {
      do_notify_disconnected ();
   }
   else if (_mode == Mode_HOST)
   {
      // resume from async mode
      ThreadBase::kill ();

      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (EventType_DISCONNECTED));
      
      bool ok_flag = _bi_dir_port_ptr->send (bso.data (), bso.size ());
      
      // if send failed, then consider that the audio engine is dead,
      // and therefore we don't need to disconnect flip
      
      bool loop_flag = ok_flag;
      
      while (loop_flag)
      {
         task::IpcData ipc_data;
         task::IpcPortBiDir::Reply ret = _bi_dir_port_ptr->recv (ipc_data);
         assert (ret == task::IpcPortBiDir::Reply_DATA_AVAILABLE);
         
         util::BinaryStreamInput bsi (ipc_data.data (), ipc_data.size ());
         
         archi::UByte type = 0;
         bsi.read (type);

         loop_flag = type != ToHostEventType_STATE_INITED;
         
         if (loop_flag)
         {
            ohm_util_TRACE_WARNING1 ("msg type %1% dropped while disconnecting", type);
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Same process version (for debug)
==============================================================================
*/

DocumentClientMasterSlot::DocumentClientMasterSlot (DocumentClientMaster & document_master, int thread_id)
:  _thread (sys::ThreadMgr::get_current_thread_id ())
,  _base_port ()
,  _thread_port ()
,  _mode (Mode_NORMAL)
,  _document_master_ptr (&document_master)
,  _bi_dir_port_ptr (0)

,  _thread_id (thread_id)

,  _run_loop (task::RunLoopBase::get_current ())
,  _observer ("ohm.flip.DocumentClientMasterSlot")

,  _state (State_CTORED)
,  _document_ptr (0)
{
   _observer.bind_process <
      DocumentClientMasterSlot,
      &DocumentClientMasterSlot::process
   > (*this);

   _run_loop.add (_observer);
   
   const std::string & user_name = document_master.get_user_name ();
   
   _document_ptr = new DocumentClient (*this, user_name, _thread_id);
}



/*
==============================================================================
Name : ctor
Description :
   Host version
==============================================================================
*/

DocumentClientMasterSlot::DocumentClientMasterSlot (task::IpcPortBiDir * port_ptr, DocumentClientMaster & document_master, int thread_id)
:  _thread (sys::ThreadMgr::get_current_thread_id ())
,  _base_port ()
,  _thread_port ()
,  _mode (Mode_HOST)
,  _document_master_ptr (&document_master)
,  _bi_dir_port_ptr (port_ptr)

,  _thread_id (thread_id)

,  _run_loop (task::RunLoopBase::get_current ())
,  _observer ("ohm.flip.DocumentClientMasterSlot")

,  _state (State_CTORED)
,  _document_ptr (0)
{
   assert (port_ptr != 0);
}



/*
==============================================================================
Name : ctor
Description :
   Guest version
==============================================================================
*/

DocumentClientMasterSlot::DocumentClientMasterSlot (const char * port_name_0, const std::string & user_name, int thread_id)
:  _thread (sys::ThreadMgr::get_current_thread_id ())
,  _base_port ()
,  _thread_port ()
,  _mode (Mode_GUEST)
,  _document_master_ptr (0)
,  _bi_dir_port_ptr (0)

,  _thread_id (thread_id)

,  _run_loop (task::RunLoopBase::get_current ())
,  _observer ("ohm.flip.DocumentClientMasterSlot")

,  _state (State_CTORED)
,  _document_ptr (0)
{
   dcms_TRACE1 ("creating guest at %1%", port_name_0);
   
   _bi_dir_port_ptr = new task::IpcPortBiDir (port_name_0, false);
   
   _observer.bind_process <
      DocumentClientMasterSlot,
      &DocumentClientMasterSlot::process
   > (*this);

   _run_loop.add (_observer);
   
   _document_ptr = new DocumentClient (*this, user_name, thread_id);

   ThreadBase::run ();
}



/*
==============================================================================
Name : do_run
==============================================================================
*/

void  DocumentClientMasterSlot::do_run ()
{
   assert (_bi_dir_port_ptr != 0);
   
   assert ((_mode == Mode_HOST) || (_mode == Mode_GUEST));
   
   // will be killed
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      task::IpcData ipc_data;
      task::IpcPortBiDir::Reply reply = _bi_dir_port_ptr->recv (ipc_data);
      
      if (reply == task::IpcPortBiDir::Reply_DATA_AVAILABLE)
      {
         if (_mode == Mode_HOST)
         {
            handle_ipc_data_host (ipc_data);
         }
         else if (_mode == Mode_GUEST)
         {
            handle_ipc_data_guest (ipc_data);
         }
      }
      else if (reply == task::IpcPortBiDir::Reply_CONNECTION_CLOSED)
      {
         dcms_TRACE ("DocumentClientMasterSlot::do_run cnx closed");
         
         loop_flag = false;
      }
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : handle_ipc_data_host
==============================================================================
*/

void  DocumentClientMasterSlot::handle_ipc_data_host (task::IpcData & ipc_data)
{
   assert (_document_master_ptr != 0);
   
   util::BinaryStreamInput bsi (ipc_data.data (), ipc_data.size ());
   
   int err = 0;
   
   archi::UByte event_type;
   err = bsi.read (event_type);
   assert (err == 0);
   
   switch (event_type)
   {
   case ToHostEventType_STATE_CTORED:
      //_state_notifier_ptr->notify (State_CTORED);
      break;

   case ToHostEventType_STATE_INITED:
      //_state_notifier_ptr->notify (State_INITED);
      break;

   case ToHostEventType_STATE_CONNECTED:
      //_state_notifier_ptr->notify (State_CONNECTED);
      break;

   case ToHostEventType_SEND:
      {
         // transaction
         // tx_desc
         // msg_routing
         
         archi::UInt32 tx_size;
         err = bsi.read (tx_size);
         assert (err == 0);
         
         size_t tx_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster tx_data = full_data.subdata (tx_pos, tx_size);
         
         bsi.advance (tx_size);
         
         TxDesc tx_desc;
         tx_desc.read (bsi);
         
         MsgRouting msg_routing;
         msg_routing.read (bsi);
         
         _document_master_ptr->send (this, tx_data, tx_desc, msg_routing);
      }
      break;

   case ToHostEventType_SEND_CTL_MSG:
      {
         // type
         // data
         // msg_routing
         
         archi::UInt32 type;
         err = bsi.read (type);
         assert (err == 0);
         
         archi::UInt32 ctl_size;
         err = bsi.read (ctl_size);
         assert (err == 0);

         size_t ctl_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster ctl_data = full_data.subdata (ctl_pos, ctl_size);
         
         bsi.advance (ctl_size);
         
         MsgRouting msg_routing;
         msg_routing.read (bsi);
         
         _document_master_ptr->send_ctl_msg (this, type, ctl_data, msg_routing);
      }
      break;

   case ToHostEventType_NOTIFY_DOCUMENT:
      {
         std::string document;
         err = bsi.read (document);
         assert (err == 0);
         
         task::Event & event = ohm_task_EVENT_GET;
         
         event.push (document);
         
         _base_port.send (event);
      }
      break;

   case ToHostEventType_NOTIFY_DOCUMENT_HEAD:
      {
         std::string document;
         err = bsi.read (document);
         assert (err == 0);
         
         task::Event & event = ohm_task_EVENT_GET;
         
         event.push (document);
         
         _base_port.send (event);
      }
      break;
   
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : handle_ipc_data_guest
==============================================================================
*/

void  DocumentClientMasterSlot::handle_ipc_data_guest (task::IpcData & ipc_data)
{
   util::BinaryStreamInput bsi (ipc_data.data (), ipc_data.size ());
   
   int err = 0;
   
   archi::UByte event_type_nbr = 0;
   err = bsi.read (event_type_nbr);
   assert (err == 0);
   
   EventType event_type = static_cast <EventType> (event_type_nbr);
   
   task::Event & fw_event = ohm_task_EVENT_GET;
   fw_event.push (event_type);

   dcms_TRACE1 ("receving msg %1%", event_type);
   
   switch (event_type)
   {
   case EventType_INIT:
   case EventType_RESTORE:
   case EventType_CONNECTING:
   case EventType_CONNECTED:
   case EventType_DISCONNECTED:
      // no additionnal parameter
      break;
   
   case EventType_LOCAL_TX:
      {
         TxDesc tx_desc;
         tx_desc.read (bsi);
         
         archi::UInt32 tx_size = 0;
         err = bsi.read (tx_size);
         assert (err == 0);
         
         size_t tx_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster tx_data = full_data.subdata (tx_pos, tx_size);
         
         bsi.advance (tx_size);
         
         fw_event.push (tx_desc);
         fw_event.push (tx_data);
      }
      break;

   case EventType_NETWORK_TX:
      {
         archi::Int64 tx_id = 0;
         bsi.read (tx_id);

         archi::Int64 rev_id = 0;
         bsi.read (rev_id);

         archi::UInt32 tx_size = 0;
         err = bsi.read (tx_size);
         assert (err == 0);
         
         size_t tx_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster tx_data = full_data.subdata (tx_pos, tx_size);
         
         bsi.advance (tx_size);

         fw_event.push (tx_id);
         fw_event.push (rev_id);
         fw_event.push (tx_data);
      }
      break;

   case EventType_NETWORK_TX_ACK:
      {
         archi::Int64 tx_id = 0;
         bsi.read (tx_id);

         archi::Int64 rev_id = 0;
         bsi.read (rev_id);

         fw_event.push (tx_id);
         fw_event.push (rev_id);
      }
      break;

   case EventType_NETWORK_TX_DENY:
      {
         archi::Int64 tx_id = 0;
         bsi.read (tx_id);

         fw_event.push (tx_id);
      }
      break;
   
   case EventType_NETWORK_CTL_MSG:
      {
         archi::UInt32 type = 0;
         bsi.read (type);
         
         archi::UInt32 ctl_size = 0;
         err = bsi.read (ctl_size);
         assert (err == 0);
         
         size_t ctl_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster ctl_data = full_data.subdata (ctl_pos, ctl_size);
         
         bsi.advance (ctl_size);
         
         fw_event.push (type);
         fw_event.push (ctl_data);
      }
      break;
   
   case EventType_LOCAL_CTL_MSG:
      {
         archi::UInt32 type = 0;
         bsi.read (type);
         
         archi::UInt32 ctl_size = 0;
         err = bsi.read (ctl_size);
         assert (err == 0);
         
         size_t ctl_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster ctl_data = full_data.subdata (ctl_pos, ctl_size);
         
         bsi.advance (ctl_size);
         
         fw_event.push (type);
         fw_event.push (ctl_data);
      }
      break;
   
   case EventType_READ_DOC:
      {
         archi::UInt32 doc_size = 0;
         err = bsi.read (doc_size);
         assert (err == 0);
         
         size_t doc_pos = bsi.tell ();
         
         DataMaster full_data;
         full_data.assign (ipc_data);
         
         DataMaster doc_data = full_data.subdata (doc_pos, doc_size);
         
         bsi.advance (doc_size);
         
         fw_event.push (doc_data);
         
         /*std::string document;
         bsi.read (document);
         
         fw_event.push (document);*/
      }
      break;
   
   default:
      assert (false);
      break;
   }
   
   _observer.send (fw_event);
}



/*
==============================================================================
Name : is_sync_call
==============================================================================
*/

bool  DocumentClientMasterSlot::is_sync_call ()
{
   assert (_thread != sys::ThreadMgr::get_invalid_thread_id ());

   return _thread == sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  DocumentClientMasterSlot::process (task::Event & event)
{
   EventType type = event.pop <EventType> ();
   
   switch (type)
   {
   case EventType_INIT:
      do_init ();
      break;

   case EventType_RESTORE:
      do_restore ();
      break;

   case EventType_CONNECTING:
      process_connecting (event);
      break;

   case EventType_CONNECTED:
      do_notify_connected ();
      break;

   case EventType_DISCONNECTED:
      do_notify_disconnected ();
      break;

   case EventType_LOCAL_TX:
      process_local_tx (event);
      break;

   case EventType_NETWORK_TX:
      process_network_tx (event);
      break;

   case EventType_NETWORK_TX_ACK:
      process_network_tx_ack (event);
      break;

   case EventType_NETWORK_TX_DENY:
      process_network_tx_deny (event);
      break;

   case EventType_NETWORK_CTL_MSG:
      process_network_ctl_msg (event);
      break;
   
   case EventType_LOCAL_CTL_MSG:
      process_local_ctl_msg (event);
      break;
   
   case EventType_READ_DOC:
      process_read_doc (event);
      break;
   
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : process_local_tx
==============================================================================
*/

void  DocumentClientMasterSlot::process_local_tx (task::Event & event)
{
   // we can arise in the situation where another thread
   // is ready and sent a local transaction
   // while this thread did not yet created its document
   // we need to sync those
   assert (_document_ptr != 0);

   TxDesc tx_desc = event.pop <TxDesc> ();
   DataMaster tx_data = event.pop <DataMaster> ();
   
   // this can arise even when the slot is not connected
   
   _document_ptr->local_transaction (
      tx_data, tx_desc
   );
}



/*
==============================================================================
Name : process_network_tx
==============================================================================
*/

void  DocumentClientMasterSlot::process_network_tx (task::Event & event)
{
   assert (_document_ptr != 0);

   archi::Int64 tx_id = event.pop <archi::Int64> ();
   archi::Int64 rev_id = event.pop <archi::Int64> ();
   DataMaster tx_data = event.pop <DataMaster> ();
   
   _document_ptr->incoming_transaction (
      tx_data, tx_id, rev_id
   );
}



/*
==============================================================================
Name : process_network_tx_ack
==============================================================================
*/

void  DocumentClientMasterSlot::process_network_tx_ack (task::Event & event)
{
   assert (_document_ptr != 0);

   archi::Int64 tx_id = event.pop <archi::Int64> ();
   archi::Int64 rev_id = event.pop <archi::Int64> ();

   _document_ptr->transaction_approved (tx_id, rev_id);
}



/*
==============================================================================
Name : process_network_tx_deny
==============================================================================
*/

void  DocumentClientMasterSlot::process_network_tx_deny (task::Event & event)
{
   assert (_document_ptr != 0);

   archi::Int64 tx_id = event.pop <archi::Int64> ();

   _document_ptr->transaction_refused (tx_id);
}



/*
==============================================================================
Name : do_init
==============================================================================
*/

void  DocumentClientMasterSlot::do_init ()
{
   assert (_document_ptr != 0);
   assert (_state == State_CTORED);
   
   dcms_TRACE ("init");
   
   if (_mode == Mode_GUEST)
   {
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (ToHostEventType_STATE_INITED));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
   
   _state = State_INITED;
}



/*
==============================================================================
Name : do_restore
==============================================================================
*/

void  DocumentClientMasterSlot::do_restore ()
{
   assert (_document_ptr != 0);
   assert (_state == State_INITED);
   
   dcms_TRACE ("restore");
   
   _document_ptr->restore ();
   
   delete _document_ptr;
   _document_ptr = 0;
   
   if (_mode == Mode_GUEST)
   {
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (ToHostEventType_STATE_CTORED));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }

   _state = State_CTORED;
}



/*
==============================================================================
Name : process_connecting
==============================================================================
*/

void  DocumentClientMasterSlot::process_connecting (task::Event & event)
{
   assert (_document_ptr != 0);
   //assert (_state == State_INITED);
   
   dcms_TRACE ("connecting");
   
   _document_ptr->notify_connecting ();
}



/*
==============================================================================
Name : do_notify_connected
==============================================================================
*/

void  DocumentClientMasterSlot::do_notify_connected ()
{
   assert (_document_ptr != 0);
   assert (_state == State_INITED);
   
   dcms_TRACE ("connected");
   
   _document_ptr->notify_connected ();
   
   if (_mode == Mode_GUEST)
   {
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (ToHostEventType_STATE_CONNECTED));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());    
   }
   
   _state = State_CONNECTED;
}



/*
==============================================================================
Name : do_notify_disconnected
==============================================================================
*/

void  DocumentClientMasterSlot::do_notify_disconnected ()
{
   assert (_document_ptr != 0);
   assert (_state == State_CONNECTED);
   
   dcms_TRACE ("disconnected");
   
   _document_ptr->notify_disconnected ();
   
   if (_mode == Mode_GUEST)
   {
      archi::UByte val;
      
      util::BinaryStreamOutputNa bso (&val, 1);
      bso.write (archi::UByte (ToHostEventType_STATE_INITED));
      
      _bi_dir_port_ptr->send (bso.data (), bso.size ());
   }
   
   _state = State_INITED;
}



/*
==============================================================================
Name : process_network_ctl_msg
==============================================================================
*/

void  DocumentClientMasterSlot::process_network_ctl_msg (task::Event & event)
{
   assert (_document_ptr != 0);
   
   dcms_TRACE ("CTL MSG");
   
   archi::UInt32 type = event.pop <archi::UInt32> ();
   DataMaster ctl_data = event.pop <DataMaster> ();
   assert (!ctl_data.empty ());
   
   _document_ptr->incoming_ctl_msg (
      type, ctl_data
   );
}



/*
==============================================================================
Name : process_local_ctl_msg
==============================================================================
*/

void  DocumentClientMasterSlot::process_local_ctl_msg (task::Event & event)
{
   assert (_document_ptr != 0);
   
   dcms_TRACE ("LOCAL CTL MSG");
   
   archi::UInt32 type = event.pop <archi::UInt32> ();
   DataMaster ctl_data = event.pop <DataMaster> ();
   assert (!ctl_data.empty ());
   
   _document_ptr->local_ctl_msg (
      type, ctl_data
   );
}



/*
==============================================================================
Name : process_read_doc
==============================================================================
*/

void  DocumentClientMasterSlot::process_read_doc (task::Event & event)
{
   assert (_document_ptr != 0);
   
   dcms_TRACE ("READ");
   
   DataMaster doc_data = event.pop <DataMaster> ();
   
   /*DocumentReader reader;
   reader.load_no_copy (document);*/

   _document_ptr->read_document (doc_data);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
