/*****************************************************************************

        SessionStats.cpp
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

#include "plex/shared/SessionStats.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

SessionStats::SessionStats ()
:  _state (State_LOAD)
,  _id ()
,  _nbr_tx (0)
,  _client_set ()
,  _mem_size (0)
,  _cpu (0)
,  _storage (Storage_IDLE)
,  _storage_cur (0)
,  _storage_total (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SessionStats::~SessionStats ()
{
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  SessionStats::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   bso.write (archi::UByte (_state));
   bso.write (_id);
   bso.write (archi::UInt32 (_nbr_tx));
   bso.write (_client_set.begin (), _client_set.end ());
   bso.write (archi::UInt32 (_mem_size));
   bso.write (archi::UInt32 (_cpu));
   bso.write (archi::UByte (_storage));
   bso.write (archi::UInt32 (_storage_cur));
   bso.write (archi::UInt32 (_storage_total));
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   SessionStats::read (ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_ubyte (_state, "bsi:sess:stats:state");
   bsi.read (_id, "bsi:sess:stats:id");
   bsi.read_uint32 (_nbr_tx, "bsi:sess:stats:nbr_tx");
   bsi.read (_client_set, "bsi:sess:stats:client_set");
   bsi.read_uint32 (_mem_size, "bsi:sess:stats:mem_size");
   bsi.read_uint32 (_cpu, "bsi:sess:stats:cpu");
   bsi.read_ubyte (_storage, "bsi:sess:stats:storage");
   bsi.read_uint32 (_storage_cur, "bsi:sess:stats:storage_cur");
   bsi.read_uint32 (_storage_total, "bsi:sess:stats:storage_total");
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

