/*****************************************************************************

        DocumentClient.h
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



#if ! defined (ohm_flip_DocumentClient_HEADER_INCLUDED)
#define  ohm_flip_DocumentClient_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Annotation.h"
#include "ohm/flip/Data.h"
#include "ohm/flip/DataMaster.h"
#include "ohm/flip/Object.h"
#include "ohm/flip/DocumentObserver.h"
#include "ohm/flip/private/TransactionsManager.h"
#include "ohm/flip/private/DocumentBase.h"

#include <set>
#include <map>
#include <string>



namespace ohm
{
namespace flip
{



class DocumentClientMasterSlot;
class DocumentReader;
class Scribe;

class DocumentClient
:  public DocumentBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class AutoUserInputs
   {
   public:
                  AutoUserInputs (DocumentBase & document);
      virtual     ~AutoUserInputs ();
   
   private:
                  AutoUserInputs ();
      
      DocumentClient &
                  _document;
   };
   
                  DocumentClient (DocumentClientMasterSlot & parent, const std::string & user_name, archi::Int32 thread_id);
   virtual        ~DocumentClient ();
   
   DocumentClientMasterSlot &
                  use_client_master_slot ();
   
   void           restore ();
   
   void           bind (DocumentObserverMaster & observer);

   void           begin_undo_redo_replay ();
   void           end_undo_redo_replay ();

   DataSPtr       write_document_head ();
   void           trash_latent_transactions ();
   void           push_document_differential (const DataMaster & tx_data);
   
   bool           send_signal (flip::Ref ref, archi::UInt32 type, const DataSPtr & data_sptr);
   bool           send_signal (archi::UInt32 type, const DataSPtr & data_sptr = DataSPtr ());
   bool           send_signal_server (flip::Ref ref, archi::UInt32 type, const DataSPtr & data_sptr);
   void           send_history (Object & object, const DataMaster & tx_data, const AnnotationSPtr & annotation_sptr);
   
   // PostManClientListener
   void           incoming_transaction (const DataMaster & tx_data, archi::Int64 tx_id, archi::Int64 rev_id);
   void           transaction_approved (archi::Int64 tx_id, archi::Int64 rev_id);
   void           transaction_refused (archi::Int64 tx_id);
   void           local_transaction (const DataMaster & tx_data, const TxDesc & tx_desc);
   void           incoming_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data);
   void           local_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data);

   void           read_document (const DataMaster & doc_data);
   DataSPtr       write_document ();
   void           resend_latent_transactions ();
   archi::Int64   get_rev_id ();

   void           notify_connecting ();
   void           notify_connected ();
   void           notify_disconnected ();

   void           add (Scribe & scribe);
   void           remove (Scribe & scribe);
   
   int            start_session ();
   bool           is_in_transaction_session ();
   void           commit_session ();
   void           revert_session ();
   void           notify_pre_user_inputs_tasks ();
   void           notify_post_user_inputs_tasks ();
   
   void           push_send_transaction (const DataMaster & tx_data, bool normal_direction_flag);
   
   virtual void   trace () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // DocumentBase
   virtual void   do_check_user_inputs ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::set <Scribe *> ScribePtrSet;

   enum MachineState
   {
                  MachineState_HEAD = 0,
                  MachineState_ROLLBACKED,
   };
   
   enum DocumentState
   {
                  DocumentState_VOID = 0,
                  DocumentState_READY,
   };
   
   enum TxSessionState
   {
                  TxSessionState_NONE = 0,
                  TxSessionState_STARTED,
   };
   
   void           push_transaction (const DataMaster & tx_data, const TxDesc & tx_desc);
   DataMaster     concat_transaction (TxDesc & tx_desc);
   void           revert_transaction (TxDesc & tx_desc);
   
   void           set_user_id (const std::string & user_name, archi::Int32 user_id);

   TxDesc         get_new_tx_desc ();

   void           end_transaction ();
   DataMaster     end_transaction_session ();
   void           revert_transaction_session ();

   void           pre_document_change_task ();
   void           post_document_change_task ();
   
   DocumentClientMasterSlot &
                  _parent;
   MachineState   _machine_state;
   DocumentState  _document_state;
   bool           _in_user_inputs_task_flag;
   
   std::auto_ptr <TxIdGenerator>
                  _tx_id_generator_aptr;
   TxDesc         _tx_desc;
   TxSessionState _tx_session_state;
   ScribePtrSet   _scribe_ptr_set;

   bool           _need_document_changed_notification_flag;
   DocumentObserverMaster *
                  _document_observer_ptr;
   
   std::auto_ptr <TransactionsManager>
                  _transactions_manager_aptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentClient ();
                  DocumentClient (const DocumentClient &other);
   DocumentClient &     operator = (const DocumentClient &other);
   bool           operator == (const DocumentClient &other);
   bool           operator != (const DocumentClient &other);

}; // class DocumentClient



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentClient.hpp"



#endif   // ohm_flip_DocumentClient_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
