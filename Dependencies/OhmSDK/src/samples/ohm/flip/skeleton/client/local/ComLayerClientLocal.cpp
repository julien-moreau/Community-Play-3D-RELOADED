/*****************************************************************************

        ComLayerClientLocal.cpp
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

#include "client/local/ComLayerClientLocal.h"

#include "client/local/ComLayerServerLocal.h"
#include "client/local/ClientLocal.h"

#include "ohm/opal/Application.h"

#include <cassert>



namespace client
{
namespace local
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ComLayerClientLocal::ComLayerClientLocal (const char * user_name_0)
:  ohm::flip::ComLayerClientBase ()
,  _user_name (user_name_0)
{
   ComLayerServerLocal::use_instance ().bind (_user_name, *this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ComLayerClientLocal::~ComLayerClientLocal ()
{
}



/*
==============================================================================
Name : local_receive
==============================================================================
*/

void  ComLayerClientLocal::local_receive (const ohm::flip::DataMaster & data)
{
   receive (data);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  ComLayerClientLocal::do_init ()
{
   // nothing
}



/*
==============================================================================
Name : do_restore
==============================================================================
*/

void  ComLayerClientLocal::do_restore ()
{
   // nothing
}



/*
==============================================================================
Name : do_connect
==============================================================================
*/

int   ComLayerClientLocal::do_connect ()
{
   return ohm::flip::Err_OK;
}



/*
==============================================================================
Name : do_disconnect
==============================================================================
*/

void  ComLayerClientLocal::do_disconnect ()
{
   // nothing
}



/*
==============================================================================
Name : do_send
==============================================================================
*/

void  ComLayerClientLocal::do_send (const ohm::flip::DataMaster & data)
{
   ComLayerServerLocal::use_instance ().local_receive (_user_name, data);
}



/*
==============================================================================
Name : do_notify_running
==============================================================================
*/

void  ComLayerClientLocal::do_notify_running ()
{
}



/*
==============================================================================
Name : do_notify_exiting
==============================================================================
*/

void  ComLayerClientLocal::do_notify_exiting ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
