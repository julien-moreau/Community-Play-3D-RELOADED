/*****************************************************************************

        DocumentClientMaster.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45386

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



#if ! defined (ohm_flip_DocumentClientMaster_HEADER_INCLUDED)
#define  ohm_flip_DocumentClientMaster_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/task/Observer.h"
#include "ohm/task/RunLoopBase.h"
#include "ohm/flip/ComLayerClient.h"
#include "ohm/flip/private/DocumentClientMasterSlot.h"
#include "ohm/flip/private/PostManClientBase.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <vector>



namespace ohm
{
namespace flip
{



class SessionManagerInterface;
class ClassDescriptionMaster;
class SynchroInterface;
class DocumentClient;
class DocumentClientMasterSlot;
class DataMaster;
class MsgRouting;
class TxDesc;

class DocumentClientMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DocumentClientMaster (SessionManagerInterface & session_manager, ComLayerClient & com_layer, const std::string & user_name);
   virtual        ~DocumentClientMaster ();
   
   const std::string &
                  get_user_name () const;
   
   void           add (DocumentClientMasterSlot & slot);
   void           remove (DocumentClientMasterSlot & slot);
   void           notify_slots_added ();
   
   void           init ();
   void           restore ();
   
   void           connect ();
   void           disconnect ();

   void           write_document ();
   void           write_document_head ();
   
   // PostManClientListener
   virtual void   read_document (const DataMaster & doc_data);
   virtual void   incoming_transaction (const DataMaster & tx_data, archi::Int64 tx_id, archi::Int64 rev_id);
   virtual void   transaction_approved (archi::Int64 tx_id, archi::Int64 rev_id);
   virtual void   transaction_refused (archi::Int64 tx_id);
   virtual void   incoming_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data);
   virtual void   notify_connecting ();
   virtual void   notify_connected ();
   virtual void   notify_disconnected ();
   
   // from slots
   void           send (const DocumentClientMasterSlot * from_slot_ptr, const DataMaster & tx_data, const TxDesc & tx_desc, const MsgRouting & msg_routing);
   void           send_ctl_msg (const DocumentClientMasterSlot * from_slot_ptr, archi::UInt32 type, const DataMaster & ctl_data, const MsgRouting & msg_routing);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum EventType
   {
                  EventType_READ_DOC = 0,
                  // DataMaster : document
                  
                  EventType_TX,
                  // archi::Int64 : tx_id
                  // archi::Int64 : rev_id
                  // DataMaster : tx_data
                  
                  EventType_TX_ACK,
                  // archi::Int64 : tx_id
                  // archi::Int64 : rev_id

                  EventType_TX_DENY,
                  // archi::Int64 : tx_id

                  EventType_CTL_MSG,
                  // archi::UInt32 : type
                  // DataMaster : data
                  
                  EventType_SEND_TX,
                  // const DocumentClientMasterSlot * : from
                  // DataMaster : tx_data
                  // TxDesc
                  // MsgRouting
                  
                  EventType_SEND_CTL_MSG,
                  // const DocumentClientMasterSlot * : from
                  // archi::UInt32 : type
                  // DataMaster : ctl_data
                  // MsgRouting
   };
   
   enum State
   {
                  State_CTORED = 0,
                  State_SLOTS_ADDED,
                  State_INITED,
                  State_CONNECTED,
   };
   
   enum SlotState
   {
                  SlotState_ADDED = 0,
                  SlotState_INITED,
                  SlotState_CONNECTED,
   };
   
   class SlotPtrState
   {
   public:
      DocumentClientMasterSlot *
                  _slot_ptr;
      SlotState   _state;
   };
   
   typedef std::list <SlotPtrState> SlotPtrStateArr;
   
   /*void            slot_state_changed (task::StateNotifier <DocumentClientMasterSlot, DocumentClientMasterSlot::State>::Event & event);*/
   
   void           process (task::Event & event);
   void           exiting_process ();

   void           process_read_doc (task::Event & event);
   void           process_tx (task::Event & event);
   void           process_tx_ack (task::Event & event);
   void           process_tx_deny (task::Event & event);
   void           process_ctl_msg (task::Event & event);
   void           process_req_write_doc (task::Event & event);
   void           process_notify_doc (task::Event & event);
   void           process_notify_doc_head (task::Event & event);
   void           process_send_tx (task::Event & event);
   void           process_send_ctl_msg (task::Event & event);

   SessionManagerInterface &
                  _session_manager;
   const std::string
                  _user_name;
   
   task::RunLoopBase &
                  _run_loop;
   task::Observer _observer;
   task::Port     _base_port;
   
   PostManClientBase
                  _post_man;
   
   State          _state;
   SlotPtrStateArr
                  _slot_ptr_state_arr;
   
   archi::Int64   _rev_id;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentClientMaster ();
                  DocumentClientMaster (const DocumentClientMaster &other);
   DocumentClientMaster &     operator = (const DocumentClientMaster &other);
   bool           operator == (const DocumentClientMaster &other);
   bool           operator != (const DocumentClientMaster &other);

}; // class DocumentClientMaster



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentClientMaster.hpp"



#endif   // ohm_flip_DocumentClientMaster_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
