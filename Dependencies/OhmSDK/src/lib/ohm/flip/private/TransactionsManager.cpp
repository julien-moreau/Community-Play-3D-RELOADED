/*****************************************************************************

        TransactionsManager.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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

#include "ohm/flip/private/TransactionsManager.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/VirtualMachine.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/DocumentClient.h"
#include "ohm/util/trace.h"

#include <cassert>

#undef OHM_FLIP_TRANSACTIONS_MANAGER_DEBUG_FLAG

#if defined (OHM_FLIP_TRANSACTIONS_MANAGER_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_TRANSACTIONS_MANAGER_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define tm_TRACE(msg_0) ohm_util_TRACE_DEBUG(msg_0)
   #define tm_TRACE1(fmt_0, arg1)   ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define tm_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define tm_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define tm_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define tm_TRACE(msg_0) (void (0))
   #define tm_TRACE1(fmt_0, arg1)   (void (0))
   #define tm_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define tm_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define tm_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))
#endif



/*
==============================================================================
Model :
   This class handles the latent transactions. A transaction is said latent
   if it is not yet executed by the server. The transaction are ordered
   as they come in. It is not important to have a strict strong ordering
   between tx_desc. The real order in the event is the one coming from
   the server, from 'acknowledge' & 'deny'.
   What we try to do whenever we can is to try to prevent to rollback
   and reexecute again.
   
   '_rev_id' maitains the revision id of the document.
   
   'push', 'concat', 'revert', 'push_session' are all initiated from the
   slot. 'push' & 'push_session' have their transaction already correctly
   executed in the current machine state. That is they reflect the state
   of machine head.
   
   'push' : NONE end_transaction (local or public)
   'concat' & 'revert' : USER_INPUTS
   'push_session' : DOC_DIF - UNDO_REDO
   
   'acknowledge', 'deny', 'execute_external' comes from server.
   'acknowledge' & 'deny' from our internal list of transactions.
   'execute_external' is a totally new transaction to execute first and forget
   
   'acknowledge' : NETWORK_ACK
   'deny' : NETWORK_DENY
   'execute_external' : NETWORK_EXT
   
   'push_execute_local' & 'replace_execute_local' comes from another local
   slot. the transaction must be executed.
   'push_execute_local' : LOCAL
   'replace_execute_local' : LOCAL
==============================================================================
*/




namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Note :
   _rev_id is the rollbacked revision number, that is the revision number
   when the transaction manager is in the rollbacked state.
==============================================================================
*/

TransactionsManager::TransactionsManager (DocumentClient & document, VirtualMachine & virtual_machine, archi::Int32 user_id, archi::Int32 thread_id)
:  _document (document)
,  _virtual_machine (virtual_machine)
,  _user_id (user_id)
,  _thread_id (thread_id)
,  _rev_id (0)
,  _state (State_HEAD)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TransactionsManager::~TransactionsManager ()
{
   assert (_state == State_HEAD);
}



/*
==============================================================================
Name : set_rev_id
==============================================================================
*/

void  TransactionsManager::set_rev_id (archi::Int64 rev_id)
{
   assert (rev_id > 0);
   
   _rev_id = rev_id;
}



/*
==============================================================================
Name : get_rev_id
==============================================================================
*/

archi::Int64   TransactionsManager::get_rev_id () const
{
   return _rev_id;
}



/*
==============================================================================
Name : push
Description :
   Push a new transaction in the manager
   There are two cases where this is done :
   - when an USER_INPUTS transaction is finished
   - when an UNDO/REDO transaction is sent
   in all cases, the virtual machine has already executed the transaction
   which is therefore guaranted to be reverse execable in the current machine
   state.
   This call comes from this slot.
==============================================================================
*/

void  TransactionsManager::push (const TxDesc & tx_desc, const DataMaster & tx_data)
{
   tm_TRACE1 ("TXM PUSH BACK %1m16rw0X%", tx_desc.get_tx_id ());
   assert (_state == State_HEAD);
   assert (!tx_desc.is_public ());
   assert (tx_desc.is_same_user (_user_id));
   assert (tx_desc.is_same_thread (_thread_id));
   assert (!tx_data.empty ());
   
   TransactionBundle transaction_bundle;
   transaction_bundle._tx_desc = tx_desc;
   transaction_bundle._tx_data = tx_data;
   transaction_bundle._executed_flag = true;
   
   // we delay the compilation of the transaction in machine code
   // to the time where it is necessary
   
   // the transaction was the last executed
   
   _transaction_list.push_back (transaction_bundle);
}



/*
==============================================================================
Name : concat
Description :
   Group all transactions with id 'tx_id' to make a single transaction
   (end of a transaction session)
   This call comes from this slot
Model :
   The latent transactions queue contains either latent single transactions
   or latent transactions sessions for all slots.
   Since this call marks the end of a transaction session, it should be
   reordered after the other latent transaction sessions but before the
   latent single transactions.
   The formed transaction is promoted to a transaction session.
Note :
   We drop all transactions that are not executed, and regroup them.
   The document may change if some transactions are reordered.
   Dropping transactions : chances are that the drop occurs because of an
   external transaction. In that case it should be somehow safe to drop
   them, to make the transaction more acceptable to the server point of view.
   By design, and reccurence, the latent transactions are always ordered
   with transaction sessions first, that is no single latent transaction can
   occur before a transaction session.
==============================================================================
*/

DataMaster  TransactionsManager::concat (TxDesc & tx_desc)
{
   tm_TRACE1 ("TXM CONCAT %1m16rw0X%", tx_desc.get_tx_id ());
   assert (_state == State_HEAD);
   assert (!tx_desc.is_public ());
   assert (tx_desc.is_same_user (_user_id));
   assert (tx_desc.is_same_thread (_thread_id));
   
   tx_desc.promote_public ();
   
   const archi::Int64 tx_id = tx_desc.get_tx_id ();
   
   // find first iterator matching tx_desc
   
   TransactionBundleList::iterator it = _transaction_list.begin ();
   const TransactionBundleList::iterator it_end = _transaction_list.end ();
   
   for (; it != it_end ; ++it)
   {
      const TransactionBundle & tb = *it;
      const TxDesc & sub_tx_desc = tb._tx_desc;
      
      if (sub_tx_desc.get_tx_id () == tx_id)
      {
         assert (sub_tx_desc.get_sub_id () == 1);
         
         break;
      }
   }
   
   if (it != it_end)
   {
      TransactionBundleList::iterator it_start = it;
      
      // 'it' points to the first transaction matching tx_desc
      // iterate till the end to know if we are going to modify the document
      
      bool document_changed_flag = false;
      bool other_tx_executed_flag = false;
      
      for (it = it_start; it != it_end ; ++it)
      {
         TransactionBundle & sub_tb = *it;
         
         if (sub_tb._tx_desc.get_tx_id () == tx_id)
         {
            if (sub_tb._executed_flag)
            {
               if (other_tx_executed_flag)
               {
                  document_changed_flag = true;
               }
            }
         }
         else
         {
            other_tx_executed_flag |= sub_tb._executed_flag;
         }
      }
      
      //
      
      if (document_changed_flag)
      {
         _virtual_machine.run ();
         
         rollback ();
      }
      
      // make a new element to collect before that element
      TransactionBundleList::iterator it_collect
         = _transaction_list.insert (it_start, TransactionBundle ());
      
      // now iterate till the end, collect and merge in 'transaction_bundle'
      // iff the sub transaction is executed
      // maintain a flag to know if the operation will change the document
      
      DataSPtr tx_data_sptr = DataSPtr (new Data);
      Data & tx_data = *tx_data_sptr;
      
      for (it = it_start ; it != it_end ;)
      {
         TransactionBundleList::iterator it_next = it;
         ++it_next;
         
         TransactionBundle & sub_tb = *it;
         
         if (sub_tb._tx_desc.get_tx_id () == tx_id)
         {
            if (sub_tb._executed_flag)
            {
               DataMaster sub_tx_data = sub_tb._tx_data;
               
               tx_data.insert (
                  tx_data.end (),
                  sub_tx_data.data (),
                  sub_tx_data.data () + sub_tx_data.size ()
               );
            }
            
            _transaction_list.erase (it);
         }
         
         it = it_next;
      }
      
      if (tx_data.empty ())
      {
         _transaction_list.erase (it_collect);
      }
      else
      {
         TransactionBundle & transaction_bundle = *it_collect;
         transaction_bundle._tx_desc = tx_desc;
         transaction_bundle._tx_data.assign (tx_data_sptr);
         transaction_bundle._executed_flag = true;
         transaction_bundle.compile ();
      }
      
      //
      
      if (document_changed_flag)
      {
         reexecute ();
         
         _virtual_machine.stop ();
      }
      
      //
      
      DataMaster concated_tx_data;
      concated_tx_data.assign (tx_data_sptr);
      concated_tx_data.lock ();
      
      return concated_tx_data;
   }
   else
   {
      // there is no transaction matching tx_id
      
      DataMaster empty_tx_data;
      empty_tx_data.lock ();
      
      return empty_tx_data;
   }
}

/*
==============================================================================
Name : revert
Description :
   Group all transactions with id 'tx_id' to delete them.
   (end of a transaction session that is reverted)
   This call comes from this slot
Model & Note :
   See concat
   In all cases, the document is very likely to change.
==============================================================================
*/

void  TransactionsManager::revert (TxDesc & tx_desc)
{
   tm_TRACE1 ("TXM REVERT %1m16rw0X%", tx_desc.get_tx_id ());
   assert (_state == State_HEAD);
   assert (!tx_desc.is_public ());
   assert (tx_desc.is_same_user (_user_id));
   assert (tx_desc.is_same_thread (_thread_id));
   
   const archi::Int64 tx_id = tx_desc.get_tx_id ();
   
   tx_desc.promote_public ();
   
   _virtual_machine.run ();
      
   rollback ();
   
   TransactionBundleList::iterator it = _transaction_list.begin ();
   const TransactionBundleList::iterator it_end = _transaction_list.end ();
   
   for (; it != it_end ;)
   {
      TransactionBundleList::iterator it_next = it;
      ++it_next;
      
      TransactionBundle & sub_tb = *it;
      
      if (sub_tb._tx_desc.get_tx_id () == tx_id)
      {
         _transaction_list.erase (it);
      }
      
      it = it_next;
   }
   
   reexecute ();
   
   _virtual_machine.stop ();
}



/*
==============================================================================
Name : push_session
Description :
   push a complete transaction (but not yet promoted to public)
   This comes from the current slot, and is operating whether in DOC_DIF or
   UNDO_REDO controller.
   The virtual machine is already in the state where this transaction is
   executed.
==============================================================================
*/

void  TransactionsManager::push_session (TxDesc & tx_desc, const DataMaster & tx_data)
{
   tm_TRACE1 ("TXM PUSH BACK %1m16rw0X%", tx_desc.get_tx_id ());
   assert (_state == State_HEAD);
   assert (!tx_desc.is_public ());
   assert (tx_desc.is_same_user (_user_id));
   assert (tx_desc.is_same_thread (_thread_id));
   assert (!tx_data.empty ());
   
   tx_desc.promote_public ();
   
   TransactionBundle transaction_bundle;
   transaction_bundle._tx_desc = tx_desc;
   transaction_bundle._tx_data = tx_data;
   transaction_bundle._executed_flag = true;
   
   // we delay the compilation of the transaction in machine code
   // to the time where it is necessary
   
   _transaction_list.push_back (transaction_bundle);
}



/*
==============================================================================
Name : acknowledge
Description :
   Acknowledge a transaction.

Note :
   If the transaction id match the id of this thread, then the transaction
   to depile is the first one, so except some checks we don't have much
   things to to.
   If the transaction id does not match the id of this thread, then the
   transaction is probably not the first one.
   It should be noted that for a single user, transactions from different
   threads are not necesseraly orderered in the transaction queue as the
   same way the server sees them.
   So for example, we might run in the case where this transaction is
   acknowledge, the transaction comes from this user & thread, but
   it is not in front, since the local document seen another thread
   transaction as first and the server seen the opposite. In that
   case, we just reorder the approved transaction to make it first.
==============================================================================
*/

void  TransactionsManager::acknowledge (ohm::archi::Int64 tx_id, archi::Int64 rev_id)
{
   assert (_state == State_HEAD);
   assert (SeedGenerator::to_user_id (tx_id) == _user_id);
   
   assert (!_transaction_list.empty ());

   assert (_rev_id + 1 == rev_id);
   _rev_id = rev_id;

   TransactionBundle & transaction_bundle = _transaction_list.front ();
   
   archi::Int64 front_tx_id = transaction_bundle._tx_desc.get_tx_id ();
   
   bool is_front_flag = front_tx_id == tx_id;
   
   if (is_front_flag)
   {
      transaction_bundle.compile ();
      
      assert (transaction_bundle._tx_desc.is_public ());
      
      bool executed_flag = transaction_bundle._executed_flag;
      
      // since this is the first transaction to be executed,
      // it must have been reexecuted as some point
      
      /*if (!executed_flag)
      {
         tm_TRACE1 ("TXM FRONT ERR %1m16rw0X%", tx_id);

         _document.trace ();
      }*/
      
      assert (executed_flag);
      
      tm_TRACE1 ("TXM POP FRONT %1m16rw0X%", tx_id);
      _transaction_list.pop_front ();
   }
   else
   {
      _virtual_machine.run ();
      
      rollback ();
      
      _virtual_machine.set_mode_no_fault ();
      _virtual_machine.set_mode_direction (tx_id);
      
      TransactionBundleList::iterator it = _transaction_list.begin ();
      const TransactionBundleList::iterator it_end = _transaction_list.end ();

      TransactionBundleList::iterator fit = _transaction_list.end ();
      
      for (; it != it_end ; ++it)
      {
         TransactionBundle & transaction_bundle = *it;
         
         if (transaction_bundle._tx_desc.get_tx_id () == tx_id)
         {
            fit = it;
            
            break;
         }
      }
      
      if (fit != _transaction_list.end ())
      {
         TransactionBundle & transaction_bundle = *fit;
         
         transaction_bundle.compile ();
         
         assert (transaction_bundle._tx_desc.is_public ());
         
         tm_TRACE ("TXM EXEC");
         int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
         assert (err == Err_OK);
         
         _transaction_list.erase (fit);
      }
      else
      {
         // ack not found
         tm_TRACE1 ("TXM ACK NOT FOUND %1m16rw0X%", tx_id);
         assert (false);
      }

      reexecute ();
      
      _virtual_machine.stop ();
   }
}



/*
==============================================================================
Name : deny
Description :
   Deny a transaction.

Note :
   If the transaction id match the id of this thread, then the transaction
   to depile is the first one, so except some checks we don't have much
   things to to.
   If the transaction id does not match the id of this thread, then the
   transaction is probably not the first one.
==============================================================================
*/

void  TransactionsManager::deny (ohm::archi::Int64 tx_id)
{
   assert (_state == State_HEAD);
   assert (SeedGenerator::to_user_id (tx_id) == _user_id);

   assert (!_transaction_list.empty ());
   
   tm_TRACE1 ("TXM DENY %1m16rw0X%", tx_id);
   
   // for now
   /*_document.trace ();
   assert (false);*/
   
   TransactionBundle & transaction_bundle = _transaction_list.front ();
   
   archi::Int64 front_tx_id = transaction_bundle._tx_desc.get_tx_id ();
   
   bool is_front_flag = front_tx_id == tx_id;
   
   if (is_front_flag)
   {
      assert (transaction_bundle._tx_desc.is_public ());
      
      _virtual_machine.set_mode_no_fault ();
      _virtual_machine.run ();
      
      rollback ();
      
      tm_TRACE ("TXM MARK CONFLICT");
      _virtual_machine.set_mode_conflict_marking ();

      _virtual_machine.set_mode_direction (tx_id);
      transaction_bundle.compile ();
      int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
      assert (err == Err_OK);
      
      _virtual_machine.set_mode_no_fault ();
      
      tm_TRACE1 ("TXM POP FRONT %1m16rw0X%", tx_id);
      _transaction_list.pop_front ();
      
      reexecute ();
      
      _virtual_machine.stop ();
   }
   else
   {
      _virtual_machine.run ();
      
      rollback ();
      
      _virtual_machine.set_mode_no_fault ();
      _virtual_machine.set_mode_direction (tx_id);
      
      TransactionBundleList::iterator it = _transaction_list.begin ();
      const TransactionBundleList::iterator it_end = _transaction_list.end ();

      TransactionBundleList::iterator fit = _transaction_list.end ();
      
      for (; it != it_end ; ++it)
      {
         TransactionBundle & transaction_bundle = *it;
         
         if (transaction_bundle._tx_desc.get_tx_id () == tx_id)
         {
            fit = it;
            
            break;
         }
      }
      
      if (fit != _transaction_list.end ())
      {
         TransactionBundle & transaction_bundle = *it;
         
         assert (transaction_bundle._tx_desc.is_public ());
         
         tm_TRACE ("TXM MARK CONFLICT");
         _virtual_machine.set_mode_conflict_marking ();

         _virtual_machine.set_mode_direction (tx_id);
         transaction_bundle.compile ();
         int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
         assert (err == Err_OK);
         
         _virtual_machine.set_mode_no_fault ();
         
         tm_TRACE ("TXM ERASE");
         _transaction_list.erase (fit);
      }
      else
      {
         // deny not found
         tm_TRACE1 ("TXM DENY NOT FOUND %1m16rw0X%", tx_id);
         assert (false);
      }
      
      reexecute ();
      
      _virtual_machine.stop ();
   }
}



/*
==============================================================================
Name : execute_external
Description :
   Execute an external (i.e. network, and not local) transaction.
   The transaction is necesseraly executed first.
==============================================================================
*/

void  TransactionsManager::execute_external (ohm::archi::Int64 tx_id, const DataMaster & tx_data, archi::Int64 rev_id)
{
   assert (_state == State_HEAD);
   assert (SeedGenerator::to_user_id (tx_id) != _user_id);
   
   if (_rev_id + 1 != rev_id)
   {
      tm_TRACE1 ("TXM _REV %1%", _rev_id);
      tm_TRACE1 ("TXM REV %1%", rev_id);
      
      assert (false);
   }
   
   _rev_id = rev_id;

   TransactionBundle transaction_bundle;
   transaction_bundle._tx_data = tx_data;
   
   transaction_bundle.compile ();

   _virtual_machine.run ();
   
   rollback ();
   
   _virtual_machine.set_mode_no_fault ();
   _virtual_machine.set_mode_direction (tx_id);
   
   tm_TRACE1 ("TXM EXEC EXT %1m16rw0X%", tx_id);
   int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
   assert (err == Err_OK);
   
   reexecute ();
   
   _virtual_machine.stop ();
}



/*
==============================================================================
Name : push_execute_local
Description :
   Execute and push a local transaction (ie not network)
   the local transaction must not be public.
==============================================================================
*/

void  TransactionsManager::push_execute_local (const TxDesc & tx_desc, const DataMaster & tx_data)
{
   assert (_state == State_HEAD);
   assert (!tx_desc.is_public ());
   assert (tx_desc.is_same_user (_user_id));
   assert (!tx_desc.is_same_thread (_thread_id));
   
   TransactionBundle transaction_bundle;
   transaction_bundle._tx_desc = tx_desc;
   transaction_bundle._tx_data = tx_data;
   
   transaction_bundle.compile ();

   _virtual_machine.run ();
   
   archi::Int64 tx_id = tx_desc.get_tx_id ();
   
   _virtual_machine.set_mode_safe ();
   _virtual_machine.set_mode_direction (tx_id);
   
   tm_TRACE1 ("TXM EXEC LOC %1m16rw0X%", tx_id);
   int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
   bool ok_flag = (err == Err_OK);
   
   _virtual_machine.stop ();
   
   if (ok_flag)
   {
      tm_TRACE ("TXM EXEC LOC OK");
   }
   else
   {
      tm_TRACE ("TXM EXEC LOC FAILED");
   }
   
   transaction_bundle._executed_flag = ok_flag;
   
   tm_TRACE1 ("TXM PUSH BACK %1m16rw0X%", tx_id);
   _transaction_list.push_back (transaction_bundle);
}



/*
==============================================================================
Name : replace_execute_local
Description :
   Replace all transactions with id 'tx_id' by 'tx_data'
   (end of a transaction session)
   This call comes from an other slot
   'tx_data' might be empty
Note :
   We need it since we want to ensure that a concat to a transaction session
   will be identical for all slots. This can be only ensured if the state
   of all executed flags are the same, which is not always possible between
   threads/slots.
   In all cases we assume that the document might have changed
==============================================================================
*/

void  TransactionsManager::replace_execute_local (const TxDesc & tx_desc, const DataMaster & tx_data)
{
   tm_TRACE1 ("TXM CONCAT %1m16rw0X%", tx_desc.get_tx_id ());
   assert (_state == State_HEAD);
   assert (tx_desc.is_public ());
   assert (tx_desc.is_same_user (_user_id));
   assert (!tx_desc.is_same_thread (_thread_id));
   // tx_data might be empty
   
   _virtual_machine.run ();
      
   rollback ();
   
   const archi::Int64 tx_id = tx_desc.get_tx_id ();
   
   // find first iterator matching tx_desc
   
   TransactionBundleList::iterator it = _transaction_list.begin ();
   const TransactionBundleList::iterator it_end = _transaction_list.end ();
   
   for (; it != it_end ; ++it)
   {
      const TransactionBundle & tb = *it;
      const TxDesc & sub_tx_desc = tb._tx_desc;
      
      if (sub_tx_desc.get_tx_id () == tx_id)
      {
         assert (sub_tx_desc.get_sub_id () == 1);
         
         break;
      }
   }
   
   // it might be equal to it_end
   
   if (!tx_data.empty ())
   {
      // 'it' points to the first transaction matching tx_desc
      // insert the transaction before 'it'
      
      TransactionBundle & transaction_bundle
         = *_transaction_list.insert (it, TransactionBundle ());
      
      transaction_bundle._tx_desc = tx_desc;
      transaction_bundle._tx_data = tx_data;
      transaction_bundle.compile ();
   }
   
   // now iterate till the end, erase all matching tx_desc
   
   for (; it != it_end ;)
   {
      TransactionBundleList::iterator it_next = it;
      ++it_next;
      
      TransactionBundle & sub_tb = *it;
      
      if (sub_tb._tx_desc.get_tx_id () == tx_id)
      {
         _transaction_list.erase (it);
      }
      
      it = it_next;
   }
   
   reexecute ();
   
   _virtual_machine.stop ();
}



/*
==============================================================================
Name : has_latent_transactions
Description :
   Returns true is the transaction manager contains latent transactions
   (either USER INPUTS, or local) waiting for an ack or deny.
   Returns false otherwise.
WARNING :
   This will return the transaction for this user (that is with the pending
   transactions of other threads)
Note :
   This can be used as a synchronization point between multiple documents.
==============================================================================
*/

bool  TransactionsManager::has_latent_transactions () const
{
   return (_transaction_list.size () > 0);
}



/*
==============================================================================
Name : resend_latent_transactions
Description :
   Resend latent transactions, *only for this thread* (that is other slots
   do not receive the resent transactions)
==============================================================================
*/

void  TransactionsManager::resend_latent_transactions ()
{
   // seems that the code is wrong
   assert (false);
   
   /*TransactionBundleList::iterator it = _transaction_list.begin ();
   const TransactionBundleList::iterator it_end = _transaction_list.end ();
   
   for (; it != it_end ; ++it)
   {
      TransactionBundle & transaction_bundle = *it;
      
      // machine code is already compiled
      
      DataMaster tx_data = transaction_bundle._tx_data;
      
      MsgRouting msg_routing;
      msg_routing._self_slot_flag = false;
      msg_routing._other_slots_flag = false;
      msg_routing._network_flag = true;
      document.send (tx_data, transaction_bundle._tx_desc, msg_routing);
   }*/
}



/*
==============================================================================
Name : clear
Description :
   Clear all latent transactions
==============================================================================
*/

void  TransactionsManager::clear ()
{
   tm_TRACE ("TXM CLEAR");
   
   assert (_state == State_ROLLBACKED);
   
   _transaction_list.clear ();
}



/*
==============================================================================
Name : rollback
Note :
   assumes the virtual machine is in 'run' state.
==============================================================================
*/

void  TransactionsManager::rollback ()
{
   tm_TRACE ("TXM ROLLBACK");
   assert (_state == State_HEAD);
   
   _virtual_machine.set_mode_no_fault ();

   TransactionBundleList::reverse_iterator it = _transaction_list.rbegin ();
   const TransactionBundleList::reverse_iterator it_end = _transaction_list.rend ();
   
   for (; it != it_end ; ++it)
   {
      TransactionBundle & transaction_bundle = *it;
      
      // if a transaction has failed to execute in an exec latent,
      // it is marked as non executed to ensure that the roll back
      // is exactly the inverse as the exec latent
      
      if (transaction_bundle._executed_flag)
      {
         transaction_bundle.compile ();
      
         archi::Int64 tx_id = transaction_bundle._tx_desc.get_tx_id ();
         _virtual_machine.set_mode_direction (- tx_id);

         int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
         assert (err == Err_OK);
      }
   }
   
   _state = State_ROLLBACKED;
}



/*
==============================================================================
Name : reexecute
Note :
   assumes the virtual machine is in 'run' state.
==============================================================================
*/

void  TransactionsManager::reexecute ()
{
   tm_TRACE ("TXM REEXECUTE");
   assert (_state == State_ROLLBACKED);
   
   _virtual_machine.set_mode_safe ();

   TransactionBundleList::iterator it = _transaction_list.begin ();
   const TransactionBundleList::iterator it_end = _transaction_list.end ();
   
   for (; it != it_end ; ++it)
   {
      TransactionBundle & transaction_bundle = *it;
      
      // machine code is already compiled
      
      archi::Int64 tx_id = transaction_bundle._tx_desc.get_tx_id ();
      _virtual_machine.set_mode_direction (tx_id);

      int err = _virtual_machine.execute (*transaction_bundle._machine_code_sptr);
      
      bool ok_flag = err == Err_OK;
      
      // mark the transaction as executed or not to know
      // if it must be inverse executed on rollback
      
      transaction_bundle._executed_flag = ok_flag;
   }
   
   _state = State_HEAD;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  TransactionsManager::trace () const
{
   ohm_util_TRACE_INFO ("*** TransactionsManager : Stack ***");
   
   TransactionBundleList::const_iterator it = _transaction_list.begin ();
   const TransactionBundleList::const_iterator it_end = _transaction_list.end ();
   
   for (; it != it_end ; ++it)
   {
      const TransactionBundle & transaction_bundle = *it;
      
      const TxDesc & tx_desc = transaction_bundle._tx_desc;
      
      archi::Int64 tx_id = tx_desc.get_tx_id ();
      archi::UInt32 sub_id = tx_desc.get_sub_id ();
      
      char c_0 [2];
      
      c_0 [0] = transaction_bundle._executed_flag ? 'X' : ' ';
      c_0 [1] = '\0';
      
      if (tx_desc.is_public ())
      {
         ohm_util_TRACE_INFO2 ("%1m16rw0X%:  PUBLIC %2%", tx_id, c_0);
      }
      else
      {
         ohm_util_TRACE_INFO3 ("%1m16rw0X%:%2m8rw0X% %3%", tx_id, sub_id, c_0);
      }
   }
   
   ohm_util_TRACE_INFO ("*** TransactionsManager : Transactions ***");
   
   it = _transaction_list.begin ();
   
   for (; it != it_end ; ++it)
   {
      const TransactionBundle & transaction_bundle = *it;
      
      const TxDesc & tx_desc = transaction_bundle._tx_desc;
      
      archi::Int64 tx_id = tx_desc.get_tx_id ();
      
      ohm_util_TRACE_INFO1 ("*** %1m16rw0X%", tx_id);
      
      MachineCodeSPtr machine_code_sptr = transaction_bundle._machine_code_sptr;
      
      if (machine_code_sptr.get () == 0)
      {
         ohm_util_TRACE_INFO ("  compiling...");
         
         machine_code_sptr->compile ();
         
         ohm_util_TRACE_INFO ("  compiled");
      }
      
      machine_code_sptr->trace_hex_ascii ();
      machine_code_sptr->trace_opcodes ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : TransactionBundle::ctor
==============================================================================
*/

TransactionsManager::TransactionBundle::TransactionBundle ()
:  _tx_desc ()
,  _tx_data ()
,  _machine_code_sptr ()
,  _executed_flag (false)
{
}



/*
==============================================================================
Name : TransactionBundle::ctor
==============================================================================
*/

TransactionsManager::TransactionBundle::TransactionBundle (const TransactionBundle & other)
:  _tx_desc (other._tx_desc)
,  _tx_data (other._tx_data)
,  _machine_code_sptr (other._machine_code_sptr)
,  _executed_flag (other._executed_flag)
{
}



/*
==============================================================================
Name : TransactionBundle::dtor
==============================================================================
*/

TransactionsManager::TransactionBundle::~TransactionBundle ()
{
}



/*
==============================================================================
Name : TransactionBundle::compile
==============================================================================
*/

void  TransactionsManager::TransactionBundle::compile ()
{
   if (_machine_code_sptr.get () == 0)
   {
      MachineCode * machine_code_ptr = new MachineCode (
         _tx_data.data (), _tx_data.size ()
      );
      
      _machine_code_sptr = MachineCodeSPtr (machine_code_ptr);
      
      _machine_code_sptr->set_mode_no_fault ();
      
      int err = _machine_code_sptr->compile ();
      assert (err == Err_OK);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
