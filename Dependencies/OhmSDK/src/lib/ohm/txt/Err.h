/*****************************************************************************

        Err.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 45801

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



#if ! defined (ohm_txt_Err_HEADER_INCLUDED)
#define  ohm_txt_Err_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace txt
{



enum Err
{
   Err_OK = 0,

   Err_ZONE = -3500,

   Err_EXCEPTION = Err_ZONE,
   Err_NO_ROOM,
   Err_NOT_FOUND,
   Err_CHAR_OUT_OF_RANGE,
   Err_INVALID_UTF8_LEAD_BYTE,
   Err_INVALID_UTF8_SEQ_LEN,
   Err_INVALID_UTF8_BYTE,
   Err_TRUNCATED_UTF8_SEQ,
   Err_INVALID_UTF16_LEAD_WORD,
   Err_INVALID_UTF16_END_WORD,
   Err_CANNOT_OPEN_FILE,
   Err_CANNOT_READ_DATA,
   Err_SP_PROHIBITED_CHAR,
   Err_SP_CHECK_BIDIR,
   Err_INVALID_PC_SEQ,
   Err_TRUNCATED_PE_SEQ,
   Err_INVALID_ESC_SEQ,
   Err_TRUNCATED_ESC_SEQ

}; // enum Err






}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/Err.hpp"



#endif   // ohm_txt_Err_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
