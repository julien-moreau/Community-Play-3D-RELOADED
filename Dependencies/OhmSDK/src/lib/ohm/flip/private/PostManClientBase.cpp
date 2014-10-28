/*****************************************************************************

        PostManClientBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49417

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

#include "ohm/flip/ComLayerMsgType.h"
#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/flip/private/TxDesc.h"
#include "ohm/flip/private/PostManClientBase.h"
#include "ohm/flip/private/StdCtlMsg.h"
#include "ohm/flip/ValidationReportReader.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*
==============================================================================
Description :
   This class handle the basis of connection. It also handles the spurious
   deconnection case.
==============================================================================
*/


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

PostManClientBase::PostManClientBase (ComLayerClient & com_layer, DocumentClientMaster & listener, const std::string & user_name)
:  _com_layer (com_layer)
,  _user_name (user_name)
,  _listener (listener)
{
   _com_layer.bind (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PostManClientBase::~PostManClientBase ()
{
   _com_layer.unbind ();
}



/*
==============================================================================
Name : connect
Description :
   two cases on the client side :
   - reconnection after a spurious disconnection the client might have latent
      transactions. If the server has enough history, those latent transactions
      are going to be consummed, and the remaining latent transactions will be
      resent.
   - reconnection after some time
      the client has been be loaded with the revised copy of the document, and
      has been pushed a latent transaction that will be difference between its
      offline copy and online copy. If the client did not work offline, then
      no transaction will be pushed. This latent transaction will be
      considered as remaining.
==============================================================================
*/

void  PostManClientBase::connect ()
{
   _com_layer.connect ();
}



/*
==============================================================================
Name : disconnect
==============================================================================
*/

void  PostManClientBase::disconnect ()
{
   _com_layer.disconnect ();
}



/*
==============================================================================
Name : send
Description :
   process a client to server transaction
Note :
   tx_id    8
   tx_size  4
   tx_data  var
==============================================================================
*/

void  PostManClientBase::send (const DataMaster & tx_data, const TxDesc & tx_desc, const MsgRouting & msg_routing)
{
   assert (msg_routing._network_flag);
   assert (tx_desc.is_public ());
   assert (!tx_data.empty ());
   
   std::auto_ptr <Data> data_aptr = std::auto_ptr <Data> (new Data);
   
   util::BinaryStreamOutput bso (*data_aptr);
   bso.write (archi::UByte (ComLayerMsgType_CS_TX));
   bso.write (tx_desc.get_tx_id ());
   
   bso.write (tx_data.data (), tx_data.data () + tx_data.size ());
   
   DataMaster data;
   data.assign (data_aptr);
   data.lock ();
   
   _com_layer.send (data);
}



/*
==============================================================================
Name : send_ctl_msg
Description :
   process a client to server control message
Note :
   ctl_type    4
   ctl_size    4
   ctl_data    var
==============================================================================
*/

void  PostManClientBase::send_ctl_msg (archi::UInt32 type, const DataMaster & ctl_data, const MsgRouting & msg_routing)
{
   assert (msg_routing._network_flag);
   
   std::auto_ptr <Data> data_aptr = std::auto_ptr <Data> (new Data);
   
   util::BinaryStreamOutput bso (*data_aptr);
   bso.write (archi::UByte (ComLayerMsgType_CS_CTL));
   bso.write (type);
   
   bso.write (ctl_data.data (), ctl_data.data () + ctl_data.size ());
   
   DataMaster data;
   data.assign (data_aptr);
   data.lock ();
   
   _com_layer.send (data);
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  PostManClientBase::receive (const DataMaster & data)
{
   assert (data.size () > 0);
   
   util::BinaryStreamInput bsi (data.data (), data.size ());
   
   archi::UByte msg_type = 0;
   bsi.read (msg_type);
   
   if (msg_type == ComLayerMsgType_SC_TX_EXT)
   {
      process_sc_tx_ext (data, bsi);
   }
   else if (msg_type == ComLayerMsgType_SC_TX_ACK)
   {
      process_sc_tx_ack (data, bsi);
   }
   else if (msg_type == ComLayerMsgType_SC_TX_DENY)
   {
      process_sc_tx_deny (data, bsi);
   }
   else if (msg_type == ComLayerMsgType_SC_CTL)
   {
      process_sc_ctl (data, bsi);
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : notify_connecting
==============================================================================
*/

void  PostManClientBase::notify_connecting ()
{
   _listener.notify_connecting ();
}



/*
==============================================================================
Name : notify_connected
==============================================================================
*/

void  PostManClientBase::notify_connected ()
{
   _listener.notify_connected ();
}



/*
==============================================================================
Name : notify_disconnected
==============================================================================
*/

void  PostManClientBase::notify_disconnected ()
{
   _listener.notify_disconnected ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_sc_tx_ext
Description :
   process a server to client external transaction
Note :
   tx_id    8
   rev_id   8
   tx_size  4
   tx_data  var
==============================================================================
*/

void  PostManClientBase::process_sc_tx_ext (const DataMaster & data, util::BinaryStreamInput & bsi)
{
   archi::Int64 tx_id = 0;
   bsi.read (tx_id);
   
   archi::Int64 rev_id = 0;
   bsi.read (rev_id);
   
   archi::UInt32 tx_size;
   bsi.read (tx_size);
   
   size_t tx_pos = bsi.tell ();
   
   DataMaster tx_data = data.subdata (tx_pos, tx_size);
   
   _listener.incoming_transaction (tx_data, tx_id, rev_id);
}



/*
==============================================================================
Name : process_sc_tx_ack
Description :
   process a server to client transaction approval
Note :
   tx_id    8
   rev_id   8
==============================================================================
*/

void  PostManClientBase::process_sc_tx_ack (const DataMaster & data, util::BinaryStreamInput & bsi)
{
   archi::Int64 tx_id = 0;
   bsi.read (tx_id);
   
   archi::Int64 rev_id = 0;
   bsi.read (rev_id);
   
   _listener.transaction_approved (tx_id, rev_id);
}



/*
==============================================================================
Name : process_sc_tx_deny
==============================================================================
*/

void  PostManClientBase::process_sc_tx_deny (const DataMaster & data, util::BinaryStreamInput & bsi)
{
   archi::Int64 tx_id = 0;
   bsi.read (tx_id);
   
   archi::UInt32 report_size;
   bsi.read (report_size);

   size_t report_pos = bsi.tell ();
   
   DataMaster ctl_data = data.subdata (report_pos, report_size);
   
   ValidationReportReader validation_report_reader (ctl_data);
   validation_report_reader.trace ();
   
   _listener.transaction_refused (tx_id);
}



/*
==============================================================================
Name : process_sc_ctl
==============================================================================
*/

void  PostManClientBase::process_sc_ctl (const DataMaster & data, util::BinaryStreamInput & bsi)
{
   archi::UInt32 type = 0;
   bsi.read (type);
   
   archi::UInt32 ctl_size;
   bsi.read (ctl_size);
   
   size_t ctl_pos = bsi.tell ();
   
   DataMaster ctl_data = data.subdata (ctl_pos, ctl_size);
   
   _listener.incoming_ctl_msg (type, ctl_data);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
