/*****************************************************************************

        ServerLocal.h
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



#if ! defined (client_local_ServerLocal_HEADER_INCLUDED)
#define  client_local_ServerLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/DocumentServer.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/RunLoop.h"



namespace model
{
class ModelValidator;
class ModelSignal;
class ModelTweaker;
}



namespace client
{
namespace local
{



class Central;

class ServerLocal
:  public ohm::sys::ThreadBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ServerLocal ();
   virtual        ~ServerLocal ();
   
   void           run ();
   void           stop ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // ohm::sys::ThreadBase
   virtual void   do_run ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           load ();
   int            unzip (std::string & dest_path, const std::string & src_path);
   
   
   
   std::auto_ptr <ohm::flip::DocumentServer>
                  _document_server_aptr;
   std::auto_ptr <model::ModelValidator>
                  _model_validator_aptr;
   std::auto_ptr <model::ModelSignal>
                  _model_signal_aptr;
   std::auto_ptr <model::ModelTweaker>
                  _model_tweaker_aptr;
   
   ohm::task::RunLoop
                  _run_loop;
   ohm::sys::ThreadSync
                  _run_sync;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ServerLocal (const ServerLocal &other);
   ServerLocal &  operator = (const ServerLocal &other);
   bool           operator == (const ServerLocal &other);
   bool           operator != (const ServerLocal &other);

}; // class ServerLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/local/ServerLocal.hpp"



#endif   // client_local_ServerLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
