/*****************************************************************************

        DocumentClientMasterSlot.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61924

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



#if ! defined (ohm_flip_DocumentClientMasterSlot_HEADER_INCLUDED)
#define  ohm_flip_DocumentClientMasterSlot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Data.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/task/IpcPortBiDir.h"
#include "ohm/task/RunLoopBase.h"
#include "ohm/task/Port.h"
#include "ohm/task/Observer.h"



namespace ohm
{
namespace flip
{



class DocumentClient;
class DocumentClientMaster;
class DataMaster;
class TxDesc;
class MsgRouting;

class DocumentClientMasterSlot
:  public sys::ThreadBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~DocumentClientMasterSlot ();
   
   static DocumentClientMasterSlot *
                  create (DocumentClientMaster & document_master, int thread_id);
   static DocumentClientMasterSlot *
                  create_host (task::IpcPortBiDir * port_ptr, DocumentClientMaster & document_master, int thread_id);
   static DocumentClientMasterSlot *
                  create_guest (const char * port_name_0, const std::string & user_name, int thread_id);
   
   void           register_thread ();
   
   int            get_thread_id () const;

   task::IpcPortBiDir *
                  get_port () const;
   
   void           init ();
   void           restore ();
   void           halt ();

   DocumentClient *
                  get_document_ptr () const;
   void           trash_latent_transactions ();
   void           push_document_differential (const DataMaster & tx_data);
   
   bool           send (const DataMaster & tx_data, const TxDesc & tx_desc, const MsgRouting & msg_routing);
   bool           send_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data, const MsgRouting & msg_routing);
   
   void           incoming_transaction (const DataMaster & tx_data, archi::Int64 tx_id, archi::Int64 rev_id);
   void           transaction_approved (archi::Int64 tx_id, archi::Int64 rev_id);
   void           transaction_refused (archi::Int64 tx_id);
   void           local_transaction (const DataMaster & tx_data, const TxDesc & tx_desc);
   void           incoming_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data);
   void           local_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data);

   void           read_document (const DataMaster & doc_data);
   DataSPtr       write_document ();
   DataSPtr       write_document_head ();

   void           notify_connecting ();
   void           notify_connected ();
   void           notify_disconnected ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  DocumentClientMasterSlot (DocumentClientMaster & document_master, int thread_id);
                  DocumentClientMasterSlot (task::IpcPortBiDir * port_ptr, DocumentClientMaster & document_master, int thread_id);
                  DocumentClientMasterSlot (const char * port_name_0, const std::string & user_name, int thread_id);
   
   // sys::ThreadBase
   virtual void   do_run ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum Mode
   {
                  Mode_NORMAL = 0,
                  Mode_HOST,
                  Mode_GUEST,
   };
   
   enum State
   {
                  State_CTORED = 0,
                  State_INITED,
                  State_CONNECTED,
   };
   
   enum EventType
   {
                  EventType_INIT = 0,
                  // no parameters
                  
                  EventType_RESTORE,
                  // no parameters
                  
                  EventType_CONNECTING,
                  // no parameters

                  EventType_CONNECTED,
                  // no parameters

                  EventType_DISCONNECTED,
                  // no parameters

                  EventType_LOCAL_TX,
                  // TxDesc : transaction description
                  // DataMaster : transaction

                  EventType_NETWORK_TX,
                  // archi::Int64 : tx_id
                  // archi::Int64 : rev_id
                  // DataMaster : transaction

                  EventType_NETWORK_TX_ACK,
                  // archi::Int64 : tx_id
                  // archi::Int64 : rev_id

                  EventType_NETWORK_TX_DENY,
                  // archi::Int64 : tx_id

                  EventType_NETWORK_CTL_MSG,
                  // archi::UInt32 : type
                  // DataMaster : data
                  
                  EventType_LOCAL_CTL_MSG,
                  // archi::UInt32 : type
                  // DataMaster : data
                  
                  EventType_READ_DOC,
                  // DataMaster : document
                  
                  /*EventType_REQ_WRITE_DOC,
                  EventType_REQ_WRITE_DOC_HEAD,*/

                  EventType_TRASH_TX,
                  // not supported
                  
                  EventType_PUSH_DOC_DIF,
                  // not supported
   };
   
   enum ToHostEventType
   {
                  ToHostEventType_STATE_CTORED = 0,
                  ToHostEventType_STATE_INITED,
                  ToHostEventType_STATE_CONNECTED,
                  
                  ToHostEventType_SEND,
                  ToHostEventType_SEND_CTL_MSG,
                  
                  ToHostEventType_NOTIFY_DOCUMENT,
                  ToHostEventType_NOTIFY_DOCUMENT_HEAD,
   };
   
   void           handle_ipc_data_host (task::IpcData & ipc_data);
   void           handle_ipc_data_guest (task::IpcData & ipc_data);
   
   bool           is_sync_call ();

   void           do_init ();
   void           do_restore ();
   void           do_notify_connected ();
   void           do_notify_disconnected ();

   void           process (task::Event & event);
   
   void           process_local_tx (task::Event & event);
   void           process_network_tx (task::Event & event);
   void           process_network_tx_ack (task::Event & event);
   void           process_network_tx_deny (task::Event & event);
   void           process_connecting (task::Event & event);
   void           process_network_ctl_msg (task::Event & event);
   void           process_local_ctl_msg (task::Event & event);
   void           process_read_doc (task::Event & event);
   
   sys::ThreadMgr::IdType
                  _thread;
   task::Port     _base_port;
   task::Port     _thread_port;
   
   const Mode     _mode;
   DocumentClientMaster * const
                  _document_master_ptr;
   task::IpcPortBiDir *
                  _bi_dir_port_ptr;
   
   const int      _thread_id;
   
   task::RunLoopBase &
                  _run_loop;
   task::Observer _observer;
   
   State          _state;
   DocumentClient *
                  _document_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentClientMasterSlot ();
                  DocumentClientMasterSlot (const DocumentClientMasterSlot &other);
   DocumentClientMasterSlot &    operator = (const DocumentClientMasterSlot &other);
   bool           operator == (const DocumentClientMasterSlot &other);
   bool           operator != (const DocumentClientMasterSlot &other);

}; // class DocumentClientMasterSlot



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentClientMasterSlot.hpp"



#endif   // ohm_flip_DocumentClientMasterSlot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
