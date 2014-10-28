/*****************************************************************************

        Transaction.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54310

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



#if ! defined (ohm_opak_Transaction_HEADER_INCLUDED)
#define  ohm_opak_Transaction_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/opak/private/TransactionDebug.h"

#include <vector>

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
#include <set>
#endif


namespace ohm
{
namespace opak
{



class Transaction
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (*Proc) (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   
                  Transaction ();
   virtual        ~Transaction ();
   Transaction &  operator = (const Transaction & other);
   
   /*void            enable_immediate_execution ();*/
   
   void           clear ();
   bool           empty ();
   
   void           push (Proc proc, void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   void           run () const;

   // debug
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   void           debug_add (void * target_ptr);
   void           debug_remove (void * target_ptr);
#endif
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           run_atom (const archi::UByte * & pos_ptr) const;
   
   /*bool            _immediate_execution_flag;*/
   std::vector <archi::UByte>
                  _data;

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   std::set <void *>
                  _target_ptr_set;
#endif



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Transaction (const Transaction &other);
   bool           operator == (const Transaction &other);
   bool           operator != (const Transaction &other);

}; // class Transaction



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/private/Transaction.hpp"



#endif   // ohm_opak_Transaction_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
