/*****************************************************************************

        ProtocolFlip.cpp
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

#include "plex/ProtocolFlip.h"

#include "ohm/util/trace.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : read_fcc_string
==============================================================================
*/

int   ProtocolFlip::read_fcc_string (std::string & str, ohm::archi::UInt32 fcc, const ohm::archi::UByte * & data_ptr, size_t & data_size)
{
   using namespace ohm;
   
   int err = 0;
   
   size_t fcc_size;

   if (err == 0)
   {
      archi::UInt32 sub_fcc;
      
      err = read_iff_header (sub_fcc, fcc_size, data_ptr, data_size);
      
      if (sub_fcc != fcc)
      {
         ohm_util_TRACE_ERROR2 ("read_fcc_string: fcc mismatch (%1% expected %2%)", fcc, sub_fcc);
         err = -1;
      }
   }
   
   if (err == 0)
   {
      size_t str_size = fcc_size - 8;
      
      str.assign (data_ptr, data_ptr + str_size);
      
      data_ptr += str_size;
      data_size -= str_size;
   }
   
   return err;
}



/*
==============================================================================
Name : read_iff_header
Description :
   Read the iff header : fcc followed by the whole size.
   Check that there is enough bytes to read the header, and that the total
   iff fits in the remaining data size.
   On return, advance data_ptr and reduce data_size by the header size
   (8 bytes)
==============================================================================
*/

int   ProtocolFlip::read_iff_header (ohm::archi::UInt32 & fcc, size_t & fcc_size, const ohm::archi::UByte * & data_ptr, size_t & data_size)
{
   int err = 0;
   
   if (err == 0)
   {
      if (data_size < 8)
      {
         ohm_util_TRACE_ERROR1 ("read_iff_header: not enough room to read (%1%)", data_size);
         err = -1;
      }
   }

   if (err == 0)
   {
      fcc = read_uint32 (data_ptr);
      data_ptr += 4;

      fcc_size = read_uint32 (data_ptr);
      data_ptr += 4;
      
      if (fcc_size < 8)
      {
         ohm_util_TRACE_ERROR1 ("read_iff_header: iff packet too small (%1%)", fcc_size);
         err = -1;
      }
      
      if (fcc_size > data_size)
      {
         ohm_util_TRACE_ERROR2 ("read_iff_header: iff packet too big (%1% > %2%)", fcc_size, data_size);
         err = -1;
      }
      
      data_size -= 8;
   }
   
   return err;
}



/*
==============================================================================
Name : read_uint32
==============================================================================
*/

ohm::archi::UInt32   ProtocolFlip::read_uint32 (const ohm::archi::UByte * data_ptr)
{
   using namespace ohm;
   
   archi::UInt32 ret_val = data_ptr [0] << 24;
   ret_val |= data_ptr [1] << 16;
   ret_val |= data_ptr [2] << 8;
   ret_val |= data_ptr [3];
   
   return ret_val;
}



/*
==============================================================================
Name : write_uint32
==============================================================================
*/

void  ProtocolFlip::write_uint32 (ohm::archi::UByte * data_ptr, ohm::archi::UInt32 x)
{
   data_ptr [0] = (x >> 24) & 255;
   data_ptr [1] = (x >> 16) & 255;
   data_ptr [2] = (x >>  8) & 255;
   data_ptr [3] = (x      ) & 255;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

