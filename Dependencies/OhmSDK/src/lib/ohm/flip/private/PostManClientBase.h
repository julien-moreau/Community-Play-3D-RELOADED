/*****************************************************************************

        PostManClientBase.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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



#if ! defined (ohm_flip_PostManClientBase_HEADER_INCLUDED)
#define  ohm_flip_PostManClientBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ComLayerClient.h"



namespace ohm
{
namespace util
{
class BinaryStreamInput;
}



namespace flip
{



class DocumentClientMaster;
class DataMaster;
class MsgRouting;
class TxDesc;

class PostManClientBase
:  public ComLayerClientListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PostManClientBase (ComLayerClient & com_layer, DocumentClientMaster & listener, const std::string & user_name);
   virtual        ~PostManClientBase ();
   
   void           connect ();
   void           disconnect ();
   
   // PostManClient
   void           send (const DataMaster & tx_data, const TxDesc & tx_desc, const MsgRouting & msg_routing);
   void           send_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data, const MsgRouting & msg_routing);
   
   // ComLayerClientListener
   virtual void   receive (const DataMaster & data);
   virtual void   notify_connecting ();
   virtual void   notify_connected ();
   virtual void   notify_disconnected ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           process_sc_tx_ext (const DataMaster & data, util::BinaryStreamInput & bsi);
   void           process_sc_tx_ack (const DataMaster & data, util::BinaryStreamInput & bsi);
   void           process_sc_tx_deny (const DataMaster & data, util::BinaryStreamInput & bsi);
   void           process_sc_ctl (const DataMaster & data, util::BinaryStreamInput & bsi);
   
   ComLayerClient &
                  _com_layer;
   DocumentClientMaster &
                  _listener;
   const std::string
                  _user_name;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PostManClientBase ();
                  PostManClientBase (const PostManClientBase &other);
   PostManClientBase &
                  operator = (const PostManClientBase &other);
   bool           operator == (const PostManClientBase &other);
   bool           operator != (const PostManClientBase &other);

}; // class PostManClientBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/PostManClientBase.hpp"



#endif   // ohm_flip_PostManClientBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
