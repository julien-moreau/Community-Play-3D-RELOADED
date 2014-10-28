/*****************************************************************************

        ClientLocal.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71330

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

#include "client/local/ClientLocal.h"

#include "client/SkeletonGui.h"

#include <cassert>



namespace client
{
namespace local
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ClientLocal::ClientLocal (SkeletonGui & gui)
:  _gui (gui)
,  _document_client_master_aptr ()
,  _communication_layer_aptr ()
,  _session_manager_aptr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ClientLocal::~ClientLocal ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : connect
==============================================================================
*/

void  ClientLocal::connect ()
{
   using namespace ohm;
   
   // com layer
   
   _communication_layer_aptr = std::auto_ptr <ComLayerClientLocal> (
      new ComLayerClientLocal (_main_client_0)
   );
   
   _communication_layer_aptr->run ();

   // client master
   
   _session_manager_aptr = std::auto_ptr <flip::SessionManagerMemory> (
      new flip::SessionManagerMemory
   );
   
   _document_client_master_aptr = std::auto_ptr <flip::DocumentClientMaster> (
      new flip::DocumentClientMaster (
         *_session_manager_aptr,
         *_communication_layer_aptr,
         _main_client_0
      )
   );
   
   // slots
   
   for (int i = 0 ; i < Slot_NBR_ELT ; ++i)
   {
      _document_client_master_slot_ptr_arr [i] = 0;
   }
   
   // Slot_GUI
   {
      flip::DocumentClientMasterSlot * & slot_ptr
         = _document_client_master_slot_ptr_arr [Slot_GUI];
   
      slot_ptr = &_gui.create (
         *_document_client_master_aptr, Slot_GUI
      );
      assert (slot_ptr != 0);
   
      _document_client_master_aptr->add (*slot_ptr);
   }
   
   _document_client_master_aptr->notify_slots_added ();

   // run
   
   _document_client_master_aptr->init ();
   _document_client_master_aptr->connect ();
}



/*
==============================================================================
Name : disconnect
==============================================================================
*/

void  ClientLocal::disconnect ()
{
   using namespace ohm;
   
   // run
   
   _document_client_master_aptr->disconnect ();

   // slots
   
   _gui.pre_release ();

   _document_client_master_aptr->restore ();
   
   // Slot_GUI
   {
      flip::DocumentClientMasterSlot * & slot_ptr
         = _document_client_master_slot_ptr_arr [Slot_GUI];
      assert (slot_ptr != 0);
      
      _document_client_master_aptr->remove (*slot_ptr);
      slot_ptr = 0;
      
      _gui.release ();
   }
   
   // client master

   _document_client_master_aptr.reset ();
      
   _session_manager_aptr.reset ();
   
   // com layer
   
   _communication_layer_aptr->stop ();
   
   _communication_layer_aptr.reset ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const char *   ClientLocal::_main_client_0 = "main_client";



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
