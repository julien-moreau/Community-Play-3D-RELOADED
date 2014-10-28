/*****************************************************************************

        SessionStats.h
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



#if ! defined (plex_SessionStats_HEADER_INCLUDED)
#define  plex_SessionStats_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <map>
#include <set>
#include <string>



namespace ohm
{
namespace util
{
class BinaryStreamOutputBase;
class BinaryStreamInput;
}
}


namespace plex
{



class SessionStats
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum State
   {
                  State_LOAD = 0,
                  State_RUN,
                  State_END,
   };

   enum Storage
   {
                  Storage_IDLE = 0,
                  Storage_READING,
                  Storage_WRITING,
                  Storage_COPYING,
   };
   
                  SessionStats ();
   virtual        ~SessionStats ();

   void           write (ohm::util::BinaryStreamOutputBase & bso) const;
   int            read (ohm::util::BinaryStreamInput & bsi);
   
   State          _state;
   std::string    _id;
   size_t         _nbr_tx;
   std::set <std::string>
                  _client_set;
   size_t         _mem_size;
   size_t         _cpu;
   Storage        _storage;
   size_t         _storage_cur;
   size_t         _storage_total;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const SessionStats &other);
   bool           operator != (const SessionStats &other);

}; // class SessionStats

typedef std::map <std::string, SessionStats> SessionStatsMap;



}  // namespace plex



//#include  "SessionStats.hpp"



#endif   // plex_SessionStats_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

