/*****************************************************************************

        DocumentLocal.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63180

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

#include "ohm/flip/DocumentLocal.h"
#include "ohm/flip/DocumentClient.h"

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

DocumentLocal::DocumentLocal (DocumentObserverMaster & observer)
:  _observer (observer)
,  _server_aptr ()
,  _client_aptr ()
,  _slot_aptr ()
,  _document_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentLocal::~DocumentLocal ()
{
   try
   {
      restore ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : init
Description :
   Load document at 'path' (fopen path)
   If path is empty, load empty document.
==============================================================================
*/

void  DocumentLocal::init (const std::string & path)
{
   // server
   
   _server_aptr = std::auto_ptr <ServerLocal> (new ServerLocal (*this));
   _server_aptr->run (path);
   
   // client
   
   _client_aptr = std::auto_ptr <ClientLocal> (new ClientLocal (*this));
   _client_aptr->connect ();
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  DocumentLocal::restore ()
{
   // client
   
   _client_aptr->disconnect ();
   _client_aptr.reset ();
   
   // server
   
   _server_aptr->stop ();
   _server_aptr.reset ();
}



/*
==============================================================================
Name : create
==============================================================================
*/

DocumentClientMasterSlot & DocumentLocal::create (DocumentClientMaster & document_master)
{
   assert (&document_master != 0);
   
   assert (_slot_aptr.get () == 0);
   assert (_document_ptr == 0);
   
   _slot_aptr = std::auto_ptr <DocumentClientMasterSlot> (
      DocumentClientMasterSlot::create (document_master, 0)
   );
   
   _document_ptr = _slot_aptr->get_document_ptr ();
   assert (_document_ptr != 0);
   
   _document_ptr->bind (_observer);
   
   return *_slot_aptr;
}



/*
==============================================================================
Name : pre_release
==============================================================================
*/

void  DocumentLocal::pre_release ()
{
   _document_ptr = 0;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  DocumentLocal::release ()
{
   assert (_document_ptr == 0);
   
   _slot_aptr.reset ();
}



/*
==============================================================================
Name : notify_pre_user_inputs_tasks
==============================================================================
*/

void  DocumentLocal::notify_pre_user_inputs_tasks ()
{
   if (_document_ptr != 0)
   {
      _document_ptr->notify_pre_user_inputs_tasks ();
   }
}



/*
==============================================================================
Name : notify_post_user_inputs_tasks
==============================================================================
*/

void  DocumentLocal::notify_post_user_inputs_tasks ()
{
   if (_document_ptr != 0)
   {
      _document_ptr->notify_post_user_inputs_tasks ();
   }
}



/*
==============================================================================
Name : AutoUserInputs::ctor
==============================================================================
*/

DocumentLocal::AutoUserInputs::AutoUserInputs (DocumentLocal & document)
:  _document (document)
{
   assert (&document != 0);
   assert (_document._document_ptr != 0);
   
   _document._document_ptr->notify_pre_user_inputs_tasks ();
}



/*
==============================================================================
Name : AutoUserInputs::dtor
==============================================================================
*/

DocumentLocal::AutoUserInputs::~AutoUserInputs ()
{
   assert (_document._document_ptr != 0);
   
   _document._document_ptr->notify_post_user_inputs_tasks ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
