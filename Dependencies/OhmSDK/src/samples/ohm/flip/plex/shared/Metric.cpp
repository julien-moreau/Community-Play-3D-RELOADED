/*****************************************************************************

        Metric.cpp
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

#include "plex/shared/Metric.h"

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

Metric::Metric ()
:  _nbr_tx (0)
,  _nbr_ping_failed (0)
,  _nbr_user_max (0)
,  _nbr_session_max (0)
,  _proc_all_max_us (0)
,  _proc_fd_max_us (0)
,  _proc_toc_max_us (0)
,  _proc_tos_max_us (0)
,  _proc_recf_max_us (0)
,  _proc_rech_max_us (0)
,  _proc_res_max_us (0)
,  _proc_nch_max_us (0)
,  _proc_af_max_us (0)
,  _proc_ah_max_us (0)
,  _proc_bi_max_us (0)
,  _proc_ubi_max_us (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Metric::~Metric ()
{
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  Metric::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   bso.write (archi::UInt32 (_nbr_tx));
   bso.write (archi::UInt32 (_nbr_ping_failed));
   bso.write (archi::UInt32 (_nbr_user_max));
   bso.write (archi::UInt32 (_nbr_session_max));
   bso.write (archi::UInt32 (_proc_all_max_us));
   bso.write (archi::UInt32 (_proc_fd_max_us));
   bso.write (archi::UInt32 (_proc_toc_max_us));
   bso.write (archi::UInt32 (_proc_tos_max_us));
   bso.write (archi::UInt32 (_proc_recf_max_us));
   bso.write (archi::UInt32 (_proc_rech_max_us));
   bso.write (archi::UInt32 (_proc_res_max_us));
   bso.write (archi::UInt32 (_proc_nch_max_us));
   bso.write (archi::UInt32 (_proc_af_max_us));
   bso.write (archi::UInt32 (_proc_ah_max_us));
   bso.write (archi::UInt32 (_proc_bi_max_us));
   bso.write (archi::UInt32 (_proc_ubi_max_us));
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   Metric::read (ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (_nbr_tx, "bsi:sess:stats:nbr_tx");
   bsi.read_uint32 (_nbr_ping_failed, "bsi:sess:stats:nbr_ping_failed");
   bsi.read_uint32 (_nbr_user_max, "bsi:sess:stats:nbr_user_max");
   bsi.read_uint32 (_nbr_session_max, "bsi:sess:stats:nbr_session_max");
   bsi.read_uint32 (_proc_all_max_us, "bsi:sess:stats:proc_all_max_us");
   bsi.read_uint32 (_proc_fd_max_us, "bsi:sess:stats:proc_fd_max_us");
   bsi.read_uint32 (_proc_toc_max_us, "bsi:sess:stats:proc_toc_max_us");
   bsi.read_uint32 (_proc_tos_max_us, "bsi:sess:stats:proc_tos_max_us");
   bsi.read_uint32 (_proc_recf_max_us, "bsi:sess:stats:proc_recf_max_us");
   bsi.read_uint32 (_proc_rech_max_us, "bsi:sess:stats:proc_rech_max_us");
   bsi.read_uint32 (_proc_res_max_us, "bsi:sess:stats:proc_res_max_us");
   bsi.read_uint32 (_proc_nch_max_us, "bsi:sess:stats:proc_nch_max_us");
   bsi.read_uint32 (_proc_af_max_us, "bsi:sess:stats:proc_af_max_us");
   bsi.read_uint32 (_proc_ah_max_us, "bsi:sess:stats:proc_ah_max_us");
   bsi.read_uint32 (_proc_bi_max_us, "bsi:sess:stats:proc_bi_max_us");
   bsi.read_uint32 (_proc_ubi_max_us, "bsi:sess:stats:proc_ubi_max_us");
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

