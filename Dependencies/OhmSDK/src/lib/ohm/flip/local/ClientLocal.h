/*****************************************************************************

        ClientLocal.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63183

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



#if ! defined (ohm_studio_client_test_ClientLocal_HEADER_INCLUDED)
#define  ohm_studio_client_test_ClientLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/local/ComLayerClientLocal.h"
#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/SessionManagerMemory.h"



namespace ohm
{
namespace flip
{



class DocumentLocal;

class ClientLocal
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ClientLocal (DocumentLocal & central);
   virtual        ~ClientLocal ();
   
   void           connect ();
   void           disconnect ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   DocumentLocal &_central;
   
   std::auto_ptr <DocumentClientMaster>
                  _document_client_master_aptr;
   DocumentClientMasterSlot *
                  _document_client_master_slot_ptr;
   std::auto_ptr <ComLayerClientLocal>
                  _communication_layer_aptr;
   std::auto_ptr <SessionManagerMemory>
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

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/studio/client/ClientLocal.hpp"



#endif   // ohm_studio_client_test_ClientLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
