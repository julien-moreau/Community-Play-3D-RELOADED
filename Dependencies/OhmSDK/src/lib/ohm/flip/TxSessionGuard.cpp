/*****************************************************************************

        TxSessionGuard.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70514

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

#include "ohm/flip/TxSessionGuard.h"
#include "ohm/flip/DocumentClient.h"
#include "ohm/flip/private/DocumentBase.h"

#include "ohm/mon/Monitor.h"

#include <cassert>

#undef OHM_FLIP_TX_SESSION_GUARD_DEBUG_FLAG



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

TxSessionGuard::AutoWield::AutoWield (TxSessionGuard & tx_session_guard)
:  _tx_session_guard (tx_session_guard)
{
   assert (&tx_session_guard != 0);
   
   _tx_session_guard.wield ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TxSessionGuard::AutoWield::~AutoWield ()
{
   _tx_session_guard.unwield ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

TxSessionGuard::TxSessionGuard (DocumentBase & document)
:  _document_ptr (dynamic_cast <DocumentClient *> (&document))

,  _started_flag (false)
,  _transaction_flag (false)
{
   assert (&document != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TxSessionGuard::~TxSessionGuard ()
{
}



/*
==============================================================================
Name : prepare_record
==============================================================================
*/

void  TxSessionGuard::prepare_record (Scribe & scribe)
{
   assert (_document_ptr != 0);
   assert (&scribe != 0);
   
   // if 'this' has already been started, don't pollute
   // the document with this new scribe that is going to be
   // ignored in the following call of start
   
   if (!_document_ptr->is_in_transaction_session ())
   {
      _document_ptr->add (scribe);
   }
}



/*
==============================================================================
Name : can_start
==============================================================================
*/

bool  TxSessionGuard::can_start () const
{
   assert (_document_ptr != 0);

   if (_started_flag)
   {
      return false;
   }
      
   return !_document_ptr->is_in_transaction_session ();
}



/*
==============================================================================
Name : start
==============================================================================
*/

bool  TxSessionGuard::start ()
{
   assert (_document_ptr != 0);

   if (_started_flag)
   {
      // don't clear _ref_set : it is being used by the same
      // object, but somewhere else and it needs to keep track
      // of its _ref_set
      
      return false;
   }
      
   if (!_document_ptr->is_in_transaction_session ())
   {
      int err = _document_ptr->start_session ();
      assert (err == Err_OK);

      _started_flag = true;

#if defined (OHM_FLIP_TX_SESSION_GUARD_DEBUG_FLAG)
      void * callstack [1024];
      int frames = backtrace (callstack, 1024);
      char ** strs = backtrace_symbols (callstack, frames);
   
      mon::Monitor::use_instance ().trace_backtrace (strs [1], false, false);
      
      free (strs);
#endif
      
      return true;
   }
   else
   {
      return false;
   }
}



/*
==============================================================================
Name : is_started
==============================================================================
*/

bool  TxSessionGuard::is_started () const
{
   assert (_document_ptr != 0);
   
   return _started_flag;
}



/*
==============================================================================
Name : commit
==============================================================================
*/

void  TxSessionGuard::commit ()
{
   assert (_document_ptr != 0);
   assert (_started_flag);
   
   _document_ptr->commit_session ();
   
   _started_flag = false;
}



/*
==============================================================================
Name : revert
==============================================================================
*/

void  TxSessionGuard::revert ()
{
   assert (_document_ptr != 0);
   assert (_started_flag);
   
   _document_ptr->revert_session ();
   
   _started_flag = false;
}



/*
==============================================================================
Name : wield
==============================================================================
*/

void  TxSessionGuard::wield ()
{
   assert (_started_flag);
   assert (!_transaction_flag);
   
   _transaction_flag = true;
}



/*
==============================================================================
Name : unwield
==============================================================================
*/

void  TxSessionGuard::unwield ()
{
   assert (_started_flag);
   assert (_transaction_flag);
   
   _transaction_flag = false;

   // don't send transaction yet, it will be done in post user inputs tasks
}



/*
==============================================================================
Name : can_join
Description :
   Returns true if the state of the document permits one to join a
   transaction, false otherwise.
Note :
   In practice, a join can be made if a transaction session is ungoing.
==============================================================================
*/

bool  TxSessionGuard::can_join ()
{
   assert (_document_ptr != 0);
   
   return _document_ptr->is_in_transaction_session ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
