/*****************************************************************************

        TransactionsManager.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49394

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



#if ! defined (ohm_flip_TransactionsManager_HEADER_INCLUDED)
#define  ohm_flip_TransactionsManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Data.h"
#include "ohm/flip/DataMaster.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/TxDesc.h"



#include <list>

namespace ohm
{
namespace flip
{



class VirtualMachine;
class DocumentClient;

class TransactionsManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TransactionsManager (DocumentClient & document, VirtualMachine & virtual_machine, archi::Int32 user_id, archi::Int32 thread_id);
   virtual        ~TransactionsManager ();
   
   void           set_rev_id (archi::Int64 rev_id);
   archi::Int64   get_rev_id () const;
   
   void           push (const TxDesc & tx_desc, const DataMaster & tx_data);
   DataMaster     concat (TxDesc & tx_desc);
   void           revert (TxDesc & tx_desc);
   void           push_session (TxDesc & tx_desc, const DataMaster & tx_data);
   
   void           acknowledge (ohm::archi::Int64 tx_id, archi::Int64 rev_id);
   void           deny (ohm::archi::Int64 tx_id);
   void           execute_external (ohm::archi::Int64 tx_id, const DataMaster & tx_data, archi::Int64 rev_id);
   void           push_execute_local (const TxDesc & tx_desc, const DataMaster & tx_data);
   void           replace_execute_local (const TxDesc & tx_desc, const DataMaster & tx_data);
   
   bool           has_latent_transactions () const;
   void           resend_latent_transactions ();
   
   void           clear ();

   void           rollback ();
   void           reexecute ();
   
   void           trace () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum State
   {
                  State_HEAD = 0,
                  State_ROLLBACKED,
   };
   
   enum LatentQueueProp
   {
                  LatentQueueProp_VOID = 0,
                  LatentQueueProp_NO_TX,
                  LatentQueueProp_TX_BEGIN,
                  LatentQueueProp_OTHER_TX_BEGIN,
                  LatentQueueProp_TX_END,
                  LatentQueueProp_TX_NON_CONTIG,
   };

   class TransactionBundle
   {
   public:
                  TransactionBundle ();
                  TransactionBundle (const TransactionBundle & other);
      virtual     ~TransactionBundle ();
      
      void        compile ();
      
      TxDesc      _tx_desc;
      DataMaster  _tx_data;
      MachineCodeSPtr
                  _machine_code_sptr;
      bool        _executed_flag;
   };
   
   typedef std::list <TransactionBundle> TransactionBundleList;
   
   DocumentClient &
                  _document;
   VirtualMachine &
                  _virtual_machine;
   const archi::Int32
                  _user_id;
   const archi::Int32
                  _thread_id;
   
   archi::Int64   _rev_id;
   
   State          _state;
   
   TransactionBundleList
                  _transaction_list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TransactionsManager ();
                  TransactionsManager (const TransactionsManager &other);
   TransactionsManager &
                  operator = (const TransactionsManager &other);
   bool           operator == (const TransactionsManager &other);
   bool           operator != (const TransactionsManager &other);

}; // class TransactionsManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/TransactionsManager.hpp"



#endif   // ohm_flip_TransactionsManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
