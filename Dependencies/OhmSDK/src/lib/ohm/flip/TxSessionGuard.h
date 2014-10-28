/*****************************************************************************

        TxSessionGuard.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63890

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



#if ! defined (ohm_flip_TxSessionGuard_HEADER_INCLUDED)
#define  ohm_flip_TxSessionGuard_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Ref.h"

#include <set>



namespace ohm
{
namespace flip
{



class DocumentClient;
class DocumentBase;
class Scribe;

class TxSessionGuard
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class AutoWield
   {
   public:
                  AutoWield (TxSessionGuard & tx_session_guard);
      virtual     ~AutoWield ();
   
   private:
      TxSessionGuard &
                  _tx_session_guard;
   };
   
                  TxSessionGuard (DocumentBase & document);
   virtual        ~TxSessionGuard ();
   
   void           prepare_record (Scribe & scribe);
   
   bool           can_start () const;
   bool           start ();
   bool           is_started () const;
   void           commit ();
   void           revert ();

   bool           can_join ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class AutoWield;
   
   void           wield ();
   void           unwield ();
   
   DocumentClient * const
                  _document_ptr;

   bool           _started_flag;
   bool           _transaction_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TxSessionGuard ();
                  TxSessionGuard (const TxSessionGuard &other);
   TxSessionGuard &
                  operator = (const TxSessionGuard &other);
   bool           operator == (const TxSessionGuard &other);
   bool           operator != (const TxSessionGuard &other);

}; // class TxSessionGuard



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/TxSessionGuard.hpp"



#endif   // ohm_flip_TxSessionGuard_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
