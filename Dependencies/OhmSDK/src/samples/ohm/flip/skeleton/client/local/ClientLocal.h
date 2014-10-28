/*****************************************************************************

        ClientLocal.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70462

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



#if ! defined (client_local_ClientLocal_HEADER_INCLUDED)
#define  client_local_ClientLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/local/ComLayerClientLocal.h"

#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/SessionManagerMemory.h"

#include <list>



namespace client
{
class SkeletonGui;

namespace local
{



class ClientLocal
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Slot
   {
                  Slot_GUI = 0,
                  
                  Slot_NBR_ELT
   };
   
                  ClientLocal (SkeletonGui & gui);
   virtual        ~ClientLocal ();
   
   void           connect ();
   void           disconnect ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           init ();
   void           restore ();
   
   SkeletonGui &  _gui;
   
   std::auto_ptr <ohm::flip::DocumentClientMaster>
                  _document_client_master_aptr;
   ohm::flip::DocumentClientMasterSlot *
                  _document_client_master_slot_ptr_arr [Slot_NBR_ELT];
   std::auto_ptr <ComLayerClientLocal>
                  _communication_layer_aptr;
   std::auto_ptr <ohm::flip::SessionManagerMemory>
                  _session_manager_aptr;
   
   static const char *
                  _main_client_0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClientLocal ();
                  ClientLocal (const ClientLocal &other);
   ClientLocal &  operator = (const ClientLocal &other);
   bool           operator == (const ClientLocal &other);
   bool           operator != (const ClientLocal &other);

}; // class ClientLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/local/ClientLocal.hpp"



#endif   // client_local_ClientLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
