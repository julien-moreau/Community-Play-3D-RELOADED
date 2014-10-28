/*****************************************************************************

        ClientNet.h
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



#if ! defined (client_net_ClientNet_HEADER_INCLUDED)
#define  client_net_ClientNet_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/net/ComLayerClientNet.h"

#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/SessionManagerMemory.h"

#include <list>



namespace client
{
class SkeletonGui;

namespace net
{



class ClientNet
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Slot
   {
                  Slot_GUI = 0,
                  
                  Slot_NBR_ELT
   };
   
                  ClientNet (SkeletonGui & gui, const std::string & host, int port, const std::string & session, const std::string & username, const std::string & secret);
   virtual        ~ClientNet ();
   
   void           connect ();
   void           disconnect ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           init ();
   void           restore ();
   
   SkeletonGui &  _gui;
   
   const std::string
                  _host;
   const int      _port;
   const std::string
                  _session;
   const std::string
                  _username;
   const std::string
                  _secret;

   std::auto_ptr <ohm::flip::DocumentClientMaster>
                  _document_client_master_aptr;
   ohm::flip::DocumentClientMasterSlot *
                  _document_client_master_slot_ptr_arr [Slot_NBR_ELT];
   std::auto_ptr <ComLayerClientNet>
                  _communication_layer_aptr;
   std::auto_ptr <ohm::flip::SessionManagerMemory>
                  _session_manager_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClientNet ();
                  ClientNet (const ClientNet &other);
   ClientNet &    operator = (const ClientNet &other);
   bool           operator == (const ClientNet &other);
   bool           operator != (const ClientNet &other);

}; // class ClientNet



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace net
}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/net/ClientNet.hpp"



#endif   // client_net_ClientNet_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
