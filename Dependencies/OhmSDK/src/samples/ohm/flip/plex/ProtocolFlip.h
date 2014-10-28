/*****************************************************************************

        ProtocolFlip.h
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



#if ! defined (plex_ProtocolFlip_HEADER_INCLUDED)
#define  plex_ProtocolFlip_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <string>



namespace plex
{



class ProtocolFlip
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Fcc
   {
      // ProtocolFlip to Server Msgs
                  Fcc_CNX_REQ =     'cxrq',

      // Server to ProtocolFlip Msgs
                  Fcc_CNX_ACK =     'cxak',
                  Fcc_CNX_REFUSED = 'cxrf',
      
      // Common Msgs
                  Fcc_TX =          'tx  ',
      
      // Types
                  Fcc_SERVICE =     'serv',
                  Fcc_VERSION =     'vers',
                  Fcc_SESSION =     'sess',
                  Fcc_USERNAME =    'user',
                  Fcc_SECRET =      'secr',
                  Fcc_DATA =        'data',
      
   };
   
   virtual        ~ProtocolFlip () {}

   static int     read_fcc_string (std::string & str, ohm::archi::UInt32 fcc, const ohm::archi::UByte * & data_ptr, size_t & data_size);
   static int     read_iff_header (ohm::archi::UInt32 & fcc, size_t & iff_size, const ohm::archi::UByte * & data_ptr, size_t & data_size);
   static ohm::archi::UInt32
                  read_uint32 (const ohm::archi::UByte * data_ptr);
   
   static void    write_uint32 (ohm::archi::UByte * data_ptr, ohm::archi::UInt32 x);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ProtocolFlip ();
                  ProtocolFlip (const ProtocolFlip &other);
   ProtocolFlip & operator = (const ProtocolFlip &other);
   bool           operator == (const ProtocolFlip &other);
   bool           operator != (const ProtocolFlip &other);

}; // class ProtocolFlip



}  // namespace plex



//#include  "ProtocolFlip.hpp"



#endif   // plex_ProtocolFlip_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

