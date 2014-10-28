/*****************************************************************************

        SessSession.cpp
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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/sess/SessSession.h"

#include "plex/sess/SessCentral.h"
#include "plex/shared/Configuration.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Create the flip document server and read the document if any.
   Throws if the session could not be created, for example if a non
   recoverable while reading on S3 occurs.
==============================================================================
*/

SessSession::SessSession (SessCentral & central)
:  _central (central)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SessSession::~SessSession ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  SessSession::receive (const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   if (data_size > 0)
   {
      util::BinaryStreamInput bsi (data_ptr, data_size);
      
      // send/broadcast will be called back in receive

      flip::ComLayerServerBase::receive (username, bsi);
   }
}



/*
==============================================================================
Name : notify_document_need_flush
==============================================================================
*/

void  SessSession::notify_document_need_flush ()
{
   _central.document_need_flush ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_send
==============================================================================
*/

void  SessSession::do_send (const std::string & to, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   _central.send (to, data_ptr, data_size);
}



/*
==============================================================================
Name : do_broadcast
==============================================================================
*/

void  SessSession::do_broadcast (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   _central.broadcast (data_ptr, data_size);
}



/*
==============================================================================
Name : do_broadcast
==============================================================================
*/

void  SessSession::do_broadcast (const std::string & except, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   _central.broadcast (except, data_ptr, data_size);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

