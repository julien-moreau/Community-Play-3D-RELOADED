/*****************************************************************************

        Err.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 61015

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



#if ! defined (ohm_util_Err_HEADER_INCLUDED)
#define  ohm_util_Err_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace util
{



enum Err
{
   Err_OK = 0,

   Err_ZONE = -4000,

   Err_EXCEPTION = Err_ZONE,
   Err_ODD_LENGTH,
   Err_NOT_HEXADECIMAL_CHARACTER,
   Err_CANNOT_BUILD_FILEREF_FROM_OPAQUE_DATA,
   Err_INVALID_FILEREF,
   Err_INVALID_DATE_STRING,
   Err_EOS,
   Err_OUT_OF_VALID_AREA

}; // enum Err



}  // namespace util
}  // namespace ohm



#endif   // ohm_util_Err_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
