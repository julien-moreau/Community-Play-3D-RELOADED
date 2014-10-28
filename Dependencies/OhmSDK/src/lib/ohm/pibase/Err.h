/*****************************************************************************

        Err.h
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 25854

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



#if ! defined (ohm_pibase_Err_HEADER_INCLUDED)
#define  ohm_pibase_Err_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{

namespace lang
{
   class CodeTxt;
}

namespace pibase
{



enum Err
{
   Err_OK = 0,

   Err_ZONE = -2000,

   Err_EXCEPTION = Err_ZONE,

   Err_NOT_IMPLEMENTED = -1,
   Err_GENERIC_FAILURE = -2

}; // enum Err



extern const lang::CodeTxt Err_txt_list [];



}  // namespace pibase
}  // namespace ohm



//#include  "ohm/pibase/Err.hpp"



#endif   // ohm_pibase_Err_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
