/*****************************************************************************

        ComLayerClientBase.cpp
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

#include "ohm/flip/ComLayerClientBase.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ComLayerClientBase::ComLayerClientBase ()
:  _run_loop ()
,  _observer ("ohm.flip.ComLayerClientBase")
,  _listener_ptr (0)
,  _connected_flag (false)
{
   _observer.bind_process <
      ComLayerClientBase,
      &ComLayerClientBase::process
   > (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ComLayerClientBase::~ComLayerClientBase ()
{
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  ComLayerClientBase::run ()
{
   ThreadBase::run ();
   
   _run_sync.wait ();
}



/*
==============================================================================
Name : quit
==============================================================================
*/

void  ComLayerClientBase::stop ()
{
   _run_loop.stop ();
   
   wait_for_death ();
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ComLayerClientBase::bind (ComLayerClientListener & listener)
{
   assert (&listener != 0);
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_BIND);
   event.push (&listener);
   _observer.send (event);
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  ComLayerClientBase::unbind ()
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_UNBIND);
   _observer.send (event);
}



/*
==============================================================================
Name : connect
==============================================================================
*/

void  ComLayerClientBase::connect ()
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_CONNECT);
   _observer.send (event);
}



/*
==============================================================================
Name : disconnect
==============================================================================
*/

void  ComLayerClientBase::disconnect ()
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_DISCONNECT);
   _observer.send (event);
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  ComLayerClientBase::send (const DataMaster & data)
{
   DataMaster data_fw = data;
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_SEND);
   event.push (data_fw);
   _observer.send (event);
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  ComLayerClientBase::receive (const DataMaster & data)
{
   DataMaster data_fw = data;
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_RECEIVE);
   event.push (data_fw);
   _observer.send (event);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_run
==============================================================================
*/

void  ComLayerClientBase::do_run ()
{
   task::RunLoopManager::use_instance ().push_current (_run_loop);
   
   do_init ();
   do_notify_running ();
   
   _run_loop.add (_observer);
   
   _run_sync.signal ();
   
   _run_loop.run ();
   
   _run_loop.remove (_observer);
   
   do_notify_exiting ();
   do_restore ();
   
   task::RunLoopManager::use_instance ().pop_current (_run_loop);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process
==============================================================================
*/

void  ComLayerClientBase::process (task::Event & event)
{
   EventType type = event.pop <EventType> ();
   
   switch (type)
   {
   case EventType_BIND:
      process_bind (event);
      break;

   case EventType_UNBIND:
      process_unbind (event);
      break;

   case EventType_CONNECT:
      process_connect (event);
      break;

   case EventType_DISCONNECT:
      process_disconnect (event);
      break;

   case EventType_SEND:
      process_send (event);
      break;

   case EventType_RECEIVE:
      process_receive (event);
      break;

   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : process_bind
==============================================================================
*/

void  ComLayerClientBase::process_bind (task::Event & event)
{
   using namespace std;
      
   assert (_listener_ptr == 0);
   
   _listener_ptr = event.pop <ComLayerClientListener *> ();
}



/*
==============================================================================
Name : process_unbind
==============================================================================
*/

void  ComLayerClientBase::process_unbind (task::Event & event)
{
   using namespace std;
      
   assert (_listener_ptr != 0);
   
   _listener_ptr = 0;
}



/*
==============================================================================
Name : process_connect
==============================================================================
*/

void  ComLayerClientBase::process_connect (task::Event & event)
{
   if (!_connected_flag)
   {
      assert (_listener_ptr != 0);

      _listener_ptr->notify_connecting ();
      
      int err = do_connect ();
      
      if (err == Err_OK)
      {
         _connected_flag = true;
         
         _listener_ptr->notify_connected ();
      }
      else
      {
         _listener_ptr->notify_disconnected ();
      }
   }
   else
   {
      ohm_util_TRACE_WARNING ("Already connected");
   }
}



/*
==============================================================================
Name : process_disconnect
==============================================================================
*/

void  ComLayerClientBase::process_disconnect (task::Event & event)
{
   if (_connected_flag)
   {
      assert (_listener_ptr != 0);
      
      do_disconnect ();
      
      _connected_flag = false;
   }
   else
   {
      ohm_util_TRACE_WARNING ("Already disconnected");
   }

   if (_listener_ptr != 0)
   {
      _listener_ptr->notify_disconnected ();
   }
}



/*
==============================================================================
Name : process_send
==============================================================================
*/

void  ComLayerClientBase::process_send (task::Event & event)
{
   if (_connected_flag)
   {
      DataMaster data = event.pop <DataMaster> ();
      
      do_send (data);
   }
   else
   {
      // drop silently
   }
}



/*
==============================================================================
Name : process_receive
==============================================================================
*/

void  ComLayerClientBase::process_receive (task::Event & event)
{
   assert (_listener_ptr != 0);
      
   if (_connected_flag)
   {
      DataMaster data = event.pop <DataMaster> ();
      
      _listener_ptr->receive (data);
   }
   else
   {
      // drop silently
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
