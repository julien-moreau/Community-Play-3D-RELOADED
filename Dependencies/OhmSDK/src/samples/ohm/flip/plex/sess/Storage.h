/*****************************************************************************

        Storage.h
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



#if ! defined (plex_Storage_HEADER_INCLUDED)
#define  plex_Storage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <vector>
#include <string>
#include <set>



namespace ohm
{
namespace flip
{
class DocumentServer;
}
}



namespace plex
{



class SessCentral;

class Storage
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Storage (SessCentral & central, ohm::flip::DocumentServer & document, const std::string & session_id);
   virtual        ~Storage ();
   
   void           flush ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   bool           load ();
   int            load (const ohm::archi::UByte * data_ptr, size_t data_size);
   
   void           write (std::vector <ohm::archi::UByte> & data);
   void           flush (const std::vector <ohm::archi::UByte> & data, const std::string & resource);
   
   SessCentral &  _central;
   ohm::flip::DocumentServer &
                  _document;
   const std::string
                  _session_id;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Storage ();
                  Storage (const Storage &other);
   Storage &      operator = (const Storage &other);
   bool           operator == (const Storage &other);
   bool           operator != (const Storage &other);

}; // class Storage



}  // namespace plex



//#include  "Storage.hpp"



#endif   // plex_Storage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

