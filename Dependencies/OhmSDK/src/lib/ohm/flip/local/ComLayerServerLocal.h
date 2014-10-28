/*****************************************************************************

        ComLayerServerLocal.h
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



#if ! defined (ohm_studio_client_test_ComLayerServerLocal_HEADER_INCLUDED)
#define  ohm_studio_client_test_ComLayerServerLocal_HEADER_INCLUDED

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
class ComLayerClientLocal;

class ComLayerServerLocal
:  public ComLayerServerBase
,  public util::SingletonBase <ComLayerServerLocal>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ComLayerServerLocal ();
   
   void           bind (task::RunLoopBase & run_loop);
   void           unbind ();
   
   void           bind (const std::string & from, ComLayerClientLocal & com_layer_client_local);

   void           local_receive (const std::string & from, const DataMaster & data);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ComLayerServerLocal ();

   // ComLayerServerBase
   virtual void   do_send (const std::string & to, const archi::UByte * data_ptr, size_t data_size);
   virtual void   do_broadcast (const archi::UByte * data_ptr, size_t data_size);
   virtual void   do_broadcast (const std::string & except, const archi::UByte * data_ptr, size_t data_size);
   


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
      std::vector <archi::UByte>
                  _data;
      double      _due_time;
   };
   
   typedef std::list <Packet> PacketArr;
   
   typedef std::map <std::string, ComLayerClientLocal *> FromComLayerClientLocalPtrMap;
   
   void           process (task::Event & event);
   void           timer_fired ();
   
   task::RunLoopBase *
                  _run_loop_ptr;
   task::Observer _observer;
   FromComLayerClientLocalPtrMap
                  _from_com_layer_client_local_ptr_map;
   
   task::Timer    _timer;
   PacketArr      _packet_arr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ComLayerServerLocal (const ComLayerServerLocal &other);
   ComLayerServerLocal &      operator = (const ComLayerServerLocal &other);
   bool           operator == (const ComLayerServerLocal &other);
   bool           operator != (const ComLayerServerLocal &other);

}; // class ComLayerServerLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/studio/client/ComLayerServerLocal.hpp"



#endif   // ohm_studio_client_test_ComLayerServerLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
