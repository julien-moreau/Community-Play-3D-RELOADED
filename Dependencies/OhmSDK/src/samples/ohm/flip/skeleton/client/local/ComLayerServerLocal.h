/*****************************************************************************

        ComLayerServerLocal.h
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



#if ! defined (client_local_ComLayerServerLocal_HEADER_INCLUDED)
#define  client_local_ComLayerServerLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ComLayerServerBase.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/SingletonBase.h"
#include "ohm/task/RunLoopBase.h"
#include "ohm/task/Observer.h"
#include "ohm/task/Timer.h"

#include <map>



namespace ohm
{
namespace flip
{
class DataMaster;
}
}


namespace client
{
namespace local
{



class ComLayerClientLocal;

class ComLayerServerLocal
:  public ohm::flip::ComLayerServerBase
,  public ohm::util::SingletonBase <ComLayerServerLocal>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ComLayerServerLocal ();
   
   void           bind (ohm::task::RunLoopBase & run_loop);
   void           unbind ();
   
   void           bind (const std::string & from, ComLayerClientLocal & com_layer_client_local);

   void           local_receive (const std::string & from, const ohm::flip::DataMaster & data);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ComLayerServerLocal ();

   // ComLayerServerBase
   virtual void   do_send (const std::string & to, const ohm::archi::UByte * data_ptr, size_t data_size);
   virtual void   do_broadcast (const ohm::archi::UByte * data_ptr, size_t data_size);
   virtual void   do_broadcast (const std::string & except, const ohm::archi::UByte * data_ptr, size_t data_size);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (ComLayerServerLocal)
   
   enum EventType
   {
                  EventType_BIND = 0,
                  // std::string : from
                  // ComLayerClientLocal *
                  
                  EventType_MSG,
                  // std::string : from
                  // std::vector <archi::UByte>
   };
   
   class Packet
   {
   public:
      std::string _from;
      std::vector <ohm::archi::UByte>
                  _data;
      double      _due_time;
   };
   
   typedef std::list <Packet> PacketArr;
   
   typedef std::map <std::string, ComLayerClientLocal *> FromComLayerClientLocalPtrMap;
   
   void           process (ohm::task::Event & event);
   void           timer_fired ();
   
   ohm::task::RunLoopBase *
                  _run_loop_ptr;
   ohm::task::Observer
                  _observer;
   FromComLayerClientLocalPtrMap
                  _from_com_layer_client_local_ptr_map;
   
   ohm::task::Timer
                  _timer;
   PacketArr      _packet_arr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ComLayerServerLocal (const ComLayerServerLocal &other);
   ComLayerServerLocal &      operator = (const ComLayerServerLocal &other);
   bool           operator == (const ComLayerServerLocal &other);
   bool           operator != (const ComLayerServerLocal &other);

}; // class ComLayerServerLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/local/ComLayerServerLocal.hpp"



#endif   // client_local_ComLayerServerLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
