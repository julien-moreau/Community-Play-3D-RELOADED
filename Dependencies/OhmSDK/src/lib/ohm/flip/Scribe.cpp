/*****************************************************************************

        Scribe.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70516

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

#include "ohm/flip/Scribe.h"
#include "ohm/flip/DocumentClient.h"
#include "ohm/flip/DocumentServer.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/Scribable.h"
#include "ohm/flip/private/TxDesc.h"

#include <cassert>

#undef OHM_FLIP_SCRIBE_VALIDATE_COMPILATION_FLAG



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Scribe::Scribe (Object & object)
:  _object (object)
,  _document (object.use_document ())
,  _forward_flag (_document.get_thread_id () != 0)
,  _cur_annotation_sptr (new Annotation)
{
   assert (dynamic_cast <Scribable *> (&object) != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Scribe::~Scribe ()
{
   try
   {
      DocumentClient * document_ptr = dynamic_cast <DocumentClient *> (&_document);

      if (document_ptr != 0)
      {
         document_ptr->remove (*this);
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : annotate
==============================================================================
*/

void  Scribe::annotate (const std::string & key, const std::string & value)
{
   (*_cur_annotation_sptr) [key] = value;
}



/*
==============================================================================
Name : write_history
Description :
   Add a page of history.
Note :
   This can be called from any 'thread' (thread is the flip sense) but
   history is automatically propagated to main thread (thread with id 0)
==============================================================================
*/

void  Scribe::write_history (const DataMaster & tx_data)
{
   if (tx_data.empty ())
   {
      // nothing
   }
   else if (_forward_flag)
   {
      DataSPtr data_sptr (new Data);
      
      DocumentClient * document_ptr = dynamic_cast <DocumentClient *> (&_document);
      assert (document_ptr != 0);
      
      document_ptr->send_history (
         _object,
         tx_data,
         _cur_annotation_sptr
      );
   }
   else
   {
      _redo_book.clear ();
      
      HistoryPage page;
      page._normal_direction_flag = false;
      page._tx_data = tx_data;
      page._annotation_sptr = _cur_annotation_sptr;

      _undo_book.push_back (page);
      
      check_undo_stack_size ();
      
      notify_history_stack_changed ();
   }

   _cur_annotation_sptr = AnnotationSPtr (new Annotation);
}



/*
==============================================================================
Name : write_history
Description :
   Add a page of history with its annotations.
Note :
   This can be only called from "main thread" and comes from another thread
   forwarding its history.
   A session might be ongoing.
==============================================================================
*/

void  Scribe::write_history (const DataMaster & tx_data, const AnnotationSPtr & annotation_sptr)
{
   assert (!_forward_flag);
   assert (!tx_data.empty ());
   
   _redo_book.clear ();
   
   HistoryPage page;
   page._normal_direction_flag = false;
   page._tx_data = tx_data;
   page._annotation_sptr = annotation_sptr;

   _undo_book.push_back (page);
   
   check_undo_stack_size ();
   
   notify_history_stack_changed ();
}



/*
==============================================================================
Name : revert_history
==============================================================================
*/

void  Scribe::revert_history ()
{
   _cur_annotation_sptr->clear ();
}



/*
==============================================================================
Name : has_undo
==============================================================================
*/

bool  Scribe::has_undo () const
{
   assert (!_forward_flag);
   
   return !_undo_book.empty ();
}



/*
==============================================================================
Name : get_undo_annotation
==============================================================================
*/

void  Scribe::get_undo_annotation (AnnotationSPtr & annotation_sptr)
{
   assert (!_forward_flag);
   assert (has_undo ());
   
   if (has_undo ())
   {
      HistoryPage & page = _undo_book.back ();
      
      annotation_sptr = page._annotation_sptr;
   }
}



/*
==============================================================================
Name : has_redo
==============================================================================
*/

bool  Scribe::has_redo () const
{
   assert (!_forward_flag);
   
   return !_redo_book.empty ();
}



/*
==============================================================================
Name : get_redo_annotation
==============================================================================
*/

void  Scribe::get_redo_annotation (AnnotationSPtr & annotation_sptr)
{
   assert (!_forward_flag);
   assert (has_redo ());
   
   if (has_redo ())
   {
      HistoryPage & page = _redo_book.back ();
      
      annotation_sptr = page._annotation_sptr;
   }
}



/*
==============================================================================
Name : undo_history
==============================================================================
*/

int   Scribe::undo_history (AnnotationSPtr & annotation_sptr)
{
   assert (!_forward_flag);
   
   return process_history (annotation_sptr, _undo_book, _redo_book);
}



/*
==============================================================================
Name : redo_history
==============================================================================
*/

int   Scribe::redo_history (AnnotationSPtr & annotation_sptr)
{
   assert (!_forward_flag);
   
   return process_history (annotation_sptr, _redo_book, _undo_book);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_history
==============================================================================
*/

int   Scribe::process_history (AnnotationSPtr & annotation_sptr, HistoryBook & src_book, HistoryBook & dst_book)
{
   DocumentClient * document_ptr = dynamic_cast <DocumentClient *> (&_document);
   assert (document_ptr != 0);
   
   int err = Err_OK;
   
   if (err == Err_OK)
   {
      if (document_ptr->is_in_transaction_session ())
      {
         err = Err_UNDO_TX_SESSION_COLLISION;
      }
   }
   
   if (err == Err_OK)
   {
      if (src_book.empty ())
      {
         err = Err_UNDO_EMPTY_STACK;
      }
   }
   
   HistoryPage page;
   
   DataMaster tx_data;
   
   if (err == Err_OK)
   {
      page = src_book.back ();
      
      tx_data = page._tx_data;
      assert (!tx_data.empty ());
      
      HistoryPage inv_page = page;
      inv_page._normal_direction_flag
         = ! inv_page._normal_direction_flag;

      src_book.pop_back ();
      dst_book.push_back (inv_page);
      
      VirtualMachine & virtual_machine = document_ptr->use_virtual_machine ();
      
      virtual_machine.set_mode_auto_correct ();
      virtual_machine.set_mode_direction (page._normal_direction_flag ? 1LL : -1LL);
      
      // we don't operate on a copy anymore... problematic ?
      /*// operate on a copy of the transaction*/
      
      MachineCode code (tx_data.data (), tx_data.size ());

      code.set_mode_no_fault ();
   
      int code_err = code.compile ();
      
      assert (code_err == Err_OK);
      
      document_ptr->begin_undo_redo_replay ();
      
      virtual_machine.run ();
      
      err = virtual_machine.execute (code);
      
      virtual_machine.stop ();
      
      document_ptr->end_undo_redo_replay ();
      
      if (code.has_corrected_tx ())
      {
         DataSPtr data_sptr = code.get_corrected_tx_data ();
         
         if (tx_data.is_locked ())
         {
            tx_data.unlock ();
         }
         
         tx_data.assign (data_sptr);

#if defined (OHM_FLIP_SCRIBE_VALIDATE_COMPILATION_FLAG)
         MachineCode code (tx_data.data (), tx_data.size ());
         code.set_mode_no_fault ();
         int code_err = code.compile ();
         assert (code_err == Err_OK);
#endif
      }
   }
   
   if (err == Err_OK)
   {
      if (tx_data.empty ())
      {
         err = Err_UNDO_TX_EMPTY;
      }
   }
   
   if (err == Err_OK)
   {
      annotation_sptr = page._annotation_sptr;
      
      document_ptr->push_send_transaction (tx_data, page._normal_direction_flag);
      
      notify_history_stack_changed ();
   }
   
   return err;
}



/*
==============================================================================
Name : notify_history_stack_changed
==============================================================================
*/

void  Scribe::notify_history_stack_changed ()
{
   // to do
}



/*
==============================================================================
Name : process_history
==============================================================================
*/

void  Scribe::check_undo_stack_size ()
{
   while (_undo_book.size () > 100)
   {
      _undo_book.pop_front ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
