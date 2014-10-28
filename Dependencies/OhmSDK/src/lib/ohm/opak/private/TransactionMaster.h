/*****************************************************************************

        TransactionMaster.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if ! defined (ohm_opak_TransactionMaster_HEADER_INCLUDED)
#define  ohm_opak_TransactionMaster_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/ClockListener.h"

#include "ohm/util/SingletonBase.h"
#include "ohm/conc/LockFreeQueue.h"
#include "ohm/conc/CellPool.h"
#include "ohm/sys/ThreadMgr.h"

#include "Transaction.h"

#include <set>



namespace ohm
{
namespace opak
{



class TransactionMaster
:  public util::SingletonBase <TransactionMaster>
,  public opal::ClockListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~TransactionMaster ();

   static Transaction &
                  use ();
   
   // main Thread
   void           send ();
   void           send_wait_vbr ();
   
   // ClockListener
   virtual void   init (double timestamp);
   virtual void   tick (double timestamp);
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  TransactionMaster ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (TransactionMaster)
   
   Transaction &  do_use ();
   
   // Video Thread
   void           receive ();

   Transaction    _main_thread_transaction;
   
   sys::ThreadMgr::IdType
                  _main_thread_id;
   
   class Token
   {
   public:
      Transaction _transaction;
   };
   
   typedef conc::LockFreeQueue <Token>::CellType Cell;
   
   conc::LockFreeQueue <Token>
                  _queue;
   conc::CellPool <Token>
                  _cell_pool;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TransactionMaster (const TransactionMaster &other);
   TransactionMaster &     operator = (const TransactionMaster &other);
   bool           operator == (const TransactionMaster &other);
   bool           operator != (const TransactionMaster &other);

}; // class TransactionMaster



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/private/TransactionMaster.hpp"



#endif   // ohm_opak_TransactionMaster_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
