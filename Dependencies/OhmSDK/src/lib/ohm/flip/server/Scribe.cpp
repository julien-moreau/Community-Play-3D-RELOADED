/*****************************************************************************

        Scribe.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47146

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
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/Scribable.h"
#include "ohm/flip/private/TxDesc.h"

#include <cassert>



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
,  _forward_flag (false)
,  _cur_annotation_sptr ()
{
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
      // nothing
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
   assert (false);
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
   assert (false);
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
   assert (false);
}



/*
==============================================================================
Name : revert_history
==============================================================================
*/

void  Scribe::revert_history ()
{
   assert (false);
}



/*
==============================================================================
Name : has_undo
==============================================================================
*/

bool  Scribe::has_undo () const
{
   assert (false);
   
   return false;
}



/*
==============================================================================
Name : get_undo_annotation
==============================================================================
*/

void  Scribe::get_undo_annotation (AnnotationSPtr & annotation_sptr)
{
   assert (false);
}



/*
==============================================================================
Name : has_redo
==============================================================================
*/

bool  Scribe::has_redo () const
{
   assert (false);
   
   return false;
}



/*
==============================================================================
Name : get_redo_annotation
==============================================================================
*/

void  Scribe::get_redo_annotation (AnnotationSPtr & annotation_sptr)
{
   assert (false);
}



/*
==============================================================================
Name : undo_history
==============================================================================
*/

int   Scribe::undo_history (AnnotationSPtr & annotation_sptr)
{
   assert (false);
}



/*
==============================================================================
Name : redo_history
==============================================================================
*/

int   Scribe::redo_history (AnnotationSPtr & annotation_sptr)
{
   assert (false);
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
   assert (false);
   
   return Err_UNDO_EMPTY_STACK;
}



/*
==============================================================================
Name : notify_history_stack_changed
==============================================================================
*/

void  Scribe::notify_history_stack_changed ()
{
   assert (false);
}



/*
==============================================================================
Name : process_history
==============================================================================
*/

void  Scribe::check_undo_stack_size ()
{
   assert (false);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
