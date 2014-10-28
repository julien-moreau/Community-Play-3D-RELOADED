/*****************************************************************************

        StdCtlMsg.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_flip_StdCtlMsg_HEADER_INCLUDED)
#define  ohm_flip_StdCtlMsg_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace flip
{



enum StdCtlMsg
{
                  StdCtlMsg_HELLO = 0,
                  
                  StdCtlMsg_SET_USER_ID,
                  StdCtlMsg_SET_DOC,
                  StdCtlMsg_DOC_OK,
                  StdCtlMsg_REQ_REM_LAT,
                  
                  StdCtlMsg_SIGNAL,
                  StdCtlMsg_HISTORY,
                  
                  NBR_StdCtlMsg_TOTAL

}; // enum StdCtlMsg


/*
- StdCtlMsg_HELLO = archi::UByte
- pseudo = 4 bytes pseudo.size () + pseudo.size () * char
- revision = archi::Int64
*/

/*
- StdCtlMsg_HELLO_REPLY = archi::UByte
- document = 4 bytes document.size () + pseudo.size () * bytes
*/




/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/StdCtlMsg.hpp"



#endif   // ohm_flip_StdCtlMsg_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
