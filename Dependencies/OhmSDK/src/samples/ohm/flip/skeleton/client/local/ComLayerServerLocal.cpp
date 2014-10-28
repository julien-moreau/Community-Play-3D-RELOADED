/*****************************************************************************

        ComLayerServerLocal.cpp
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

#include "client/local/ComLayerServerLocal.h"
#include "client/local/ComLayerClientLocal.h"

#include "ohm/flip/Data.h"
#include "ohm/flip/DataMaster.h"

#include <cassert>

#define OHM_STUDIO_TEST_LATENCY 1.0



namespace client
{
namespace local
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

ComLayerServerLocal::~ComLayerServerLocal ()
{
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ComLayerServerLocal::bind (ohm::task::RunLoopBase & run_loop)
{
   assert (&run_loop != 0);
   assert (_run_loop_ptr == 0);
   
   _run_loop_ptr = &run_loop;
   
   _observer.bind_process <
      ComLayerServerLocal,
      &ComLayerServerLocal::process
   > (*this);
   
   run_loop.add (_observer);
   
   _timer.bind_process <
      ComLayerServerLocal,
      &ComLayerServerLocal::timer_fired
   > (*this);
   
   run_loop.add (_timer);
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  ComLayerServerLocal::unbind ()
{
   assert (_run_loop_ptr != 0);
   
   _run_loop_ptr->remove (_timer);
   _run_loop_ptr->remove (_observer);
   _run_loop_ptr = 0;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ComLayerServerLocal::bind (const std::string & from, ComLayerClientLocal & com_layer_client_local)
{
   using namespace ohm;
   
   assert (&com_layer_client_local != 0);
   
   assert (_run_loop_ptr != 0);
   
   task::Event & event = ohm_task_EVENT_GET;
   
   event.push (EventType_BIND);
   event.push (from);
   event.push (&com_layer_client_local);
   
   _observer.send (event);
}



/*
==============================================================================
Name : local_receive
==============================================================================
*/

void  ComLayerServerLocal::local_receive (const std::string & from, const ohm::flip::DataMaster & data)
{
   using namespace ohm;
   
   assert (_run_loop_ptr != 0);
   
   task::Event & event = ohm_task_EVENT_GET;
   
   std::vector <archi::UByte> buf (data.data (), data.data () + data.size ());
   
   event.push (EventType_MSG);
   event.push (from);
   event.push (buf);
   
   _observer.send (event);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ComLayerServerLocal::ComLayerServerLocal ()
:  _run_loop_ptr (0)
,  _observer ("ohm.studio.client.test.ComLayerServerLocal")
,  _timer ("ohm.studio.client.test.ComLayerServerLocal")
,  _from_com_layer_client_local_ptr_map ()
{
   _timer.set_periodic (1.0 / 10.0);
}



/*
==============================================================================
Name : do_send
==============================================================================
*/

void  ComLayerServerLocal::do_send (const std::string & to, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   assert (!_from_com_layer_client_local_ptr_map.empty ());
   
   FromComLayerClientLocalPtrMap::iterator it
      = _from_com_layer_client_local_ptr_map.find (to);
   
   assert (it != _from_com_layer_client_local_ptr_map.end ());
   
   flip::DataSPtr data_sptr (new flip::Data (data_ptr, data_ptr + data_size));
   
   flip::DataMaster data;
   data.assign (data_sptr);
   data.lock ();
   
   ComLayerClientLocal * com_layer_client_local_ptr = it->second;
   
   com_layer_client_local_ptr->local_receive (data);
}



/*
==============================================================================
Name : do_broadcast
==============================================================================
*/

void  ComLayerServerLocal::do_broadcast (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   FromComLayerClientLocalPtrMap::iterator it
      = _from_com_layer_client_local_ptr_map.begin ();
   FromComLayerClientLocalPtrMap::iterator it_end
      = _from_com_layer_client_local_ptr_map.end ();
   
   flip::DataSPtr data_sptr (new flip::Data (data_ptr, data_ptr + data_size));
   
   flip::DataMaster data;
   data.assign (data_sptr);
   data.lock ();
   
   for (; it != it_end ; ++it)
   {
      ComLayerClientLocal * com_layer_client_local_ptr = it->second;
   
      com_layer_client_local_ptr->local_receive (data);
   }
}



/*
==============================================================================
Name : do_broadcast
==============================================================================
*/

void  ComLayerServerLocal::do_broadcast (const std::string & except, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   FromComLayerClientLocalPtrMap::iterator it
      = _from_com_layer_client_local_ptr_map.begin ();
   FromComLayerClientLocalPtrMap::iterator it_end
      = _from_com_layer_client_local_ptr_map.end ();
   
   flip::DataSPtr data_sptr (new flip::Data (data_ptr, data_ptr + data_size));
   
   flip::DataMaster data;
   data.assign (data_sptr);
   data.lock ();
   
   for (; it != it_end ; ++it)
   {
      if (it->first != except)
      {
         ComLayerClientLocal * com_layer_client_local_ptr = it->second;
      
         com_layer_client_local_ptr->local_receive (data);
      }
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process
==============================================================================
*/

void  ComLayerServerLocal::process (ohm::task::Event & event)
{
   using namespace ohm;
   
   EventType type = event.pop <EventType> ();
   
   if (type == EventType_BIND)
   {
      std::string from = event.pop <std::string> ();
      ComLayerClientLocal * com_ptr = event.pop <ComLayerClientLocal *> ();
      
      assert (
         _from_com_layer_client_local_ptr_map.find (from)
         == _from_com_layer_client_local_ptr_map.end ()
      );
      
      _from_com_layer_client_local_ptr_map [from] = com_ptr;
   }
   else if (type == EventType_MSG)
   {
      std::string from = event.pop <std::string> ();
      //flip::DataMaster data = event.pop <flip::DataMaster> ();
      std::vector <archi::UByte> data = event.pop <std::vector <archi::UByte> > ();
      
      assert (!data.empty ());
      
      //util::BinaryStreamInput bsi (data.data (), data.size ());
      //util::BinaryStreamInput bsi (&data [0], data.size ());
      
      Packet packet;
      packet._from = from;
      packet._data = data;
      packet._due_time = sys::TimerReliable::use_instance ().get () + OHM_STUDIO_TEST_LATENCY;
      
      _packet_arr.push_back (packet);
      
      //receive (from, bsi);
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : timer_fired
==============================================================================
*/

void  ComLayerServerLocal::timer_fired ()
{
   using namespace ohm;
   
   double now = sys::TimerReliable::use_instance ().get ();
   
   PacketArr::iterator it = _packet_arr.begin ();
   const PacketArr::iterator it_end = _packet_arr.end ();
   
   for (; it != it_end ;)
   {
      PacketArr::iterator it_next = it;
      ++it_next;
      
      Packet & packet = *it;
      
      if (packet._due_time <= now)
      {
         util::BinaryStreamInput bsi (&packet._data [0], packet._data.size ());
         
         receive (packet._from, bsi);
         
         _packet_arr.erase (it);
      }
      else
      {
         break;
      }
      
      it = it_next;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
