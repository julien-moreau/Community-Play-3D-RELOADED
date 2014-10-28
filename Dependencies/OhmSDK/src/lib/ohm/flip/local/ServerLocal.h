/*****************************************************************************

        ServerLocal.h
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



#if ! defined (ohm_studio_client_test_ServerLocal_HEADER_INCLUDED)
#define  ohm_studio_client_test_ServerLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/DocumentServer.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/RunLoop.h"



namespace ohm
{
namespace flip
{



class DocumentLocal;

class ServerLocal
:  public sys::ThreadBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ServerLocal (DocumentLocal & central);
   virtual        ~ServerLocal ();
   
   void           run (const std::string & document_path);
   void           stop ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // sys::ThreadBase
   virtual void   do_run ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           load (const std::string & document_path);
   
   
   
   DocumentLocal &_central;
   
   std::string    _document_path;
   
   std::auto_ptr <DocumentServer>
                  _document_server_aptr;
   /*std::auto_ptr <model::ModelValidator>
                  _model_validator_aptr;*/
   
   task::RunLoop  _run_loop;
   sys::ThreadSync
                  _run_sync;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ServerLocal ();
                  ServerLocal (const ServerLocal &other);
   ServerLocal &  operator = (const ServerLocal &other);
   bool           operator == (const ServerLocal &other);
   bool           operator != (const ServerLocal &other);

}; // class ServerLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/studio/client/ServerLocal.hpp"



#endif   // ohm_studio_client_test_ServerLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
