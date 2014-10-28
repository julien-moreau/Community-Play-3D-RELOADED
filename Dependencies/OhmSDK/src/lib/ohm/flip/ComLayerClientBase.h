/*****************************************************************************

        ComLayerClientBase.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if ! defined (ohm_flip_ComLayerClientBase_HEADER_INCLUDED)
#define  ohm_flip_ComLayerClientBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ComLayerClient.h"
#include "ohm/flip/DataMaster.h"
#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/RunLoop.h"
#include "ohm/task/Observer.h"



namespace ohm
{
namespace flip
{



class ComLayerClientBase
:  public ComLayerClient
,  public sys::ThreadBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ComLayerClientBase ();
   virtual        ~ComLayerClientBase ();
   
   void           run ();
   void           stop ();
   
   // ComLayerClient
   virtual void   bind (ComLayerClientListener & listener);
   virtual void   unbind ();
   
   virtual void   connect ();
   virtual void   disconnect ();
   virtual void   send (const DataMaster & data);

   void           receive (const DataMaster & data);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // sys::ThreadBase
   virtual void   do_run ();
   
   virtual void   do_init () = 0;
   virtual void   do_restore () = 0;
   virtual int    do_connect () = 0;
   virtual void   do_disconnect () = 0;
   virtual void   do_send (const DataMaster & data) = 0;
   virtual void   do_notify_exiting () = 0;
   virtual void   do_notify_running () = 0;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum EventType
   {
                  EventType_BIND = 0,
                  // ComLayerClientListener *
                  
                  EventType_UNBIND,
                  // nothing
                  
                  EventType_CONNECT,
                  // nothing
                  
                  EventType_DISCONNECT,
                  // nothing
                  
                  EventType_SEND,
                  // DataMaster
                  
                  EventType_RECEIVE,
                  // DataMaster
   };
   
   // delegates
   void           process (task::Event & event);
   
   void           process_bind (task::Event & event);
   void           process_unbind (task::Event & event);
   void           process_connect (task::Event & event);
   void           process_disconnect (task::Event & event);
   void           process_send (task::Event & event);
   void           process_receive (task::Event & event);
   
   task::RunLoop  _run_loop;
   sys::ThreadSync
                  _run_sync;
   task::Observer _observer;
   
   // thread
   ComLayerClientListener *
                  _listener_ptr;
   bool           _connected_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ComLayerClientBase (const ComLayerClientBase &other);
   ComLayerClientBase &
                  operator = (const ComLayerClientBase &other);
   bool           operator == (const ComLayerClientBase &other);
   bool           operator != (const ComLayerClientBase &other);

}; // class ComLayerClientBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/ComLayerClientBase.hpp"



#endif   // ohm_flip_ComLayerClientBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
