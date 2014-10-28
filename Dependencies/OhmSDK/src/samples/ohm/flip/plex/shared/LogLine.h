/*****************************************************************************

        LogLine.h
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



#if ! defined (plex_LogLine_HEADER_INCLUDED)
#define  plex_LogLine_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <list>



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



class LogLine
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Type
   {
                  Type_INVALID = -1,
                  
                  Type_INFO = 0,
                  Type_ERROR,
                  Type_INCIDENT,
   };
   
                  LogLine ();
                  LogLine (Type type, time_t time, const std::string & str);
   virtual        ~LogLine ();

   void           make_html (std::string & html) const;
   
   void           write (ohm::util::BinaryStreamOutputBase & bso) const;
   int            read (ohm::util::BinaryStreamInput & bsi);

   Type           _type;
   time_t         _time;
   std::string    _str;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const LogLine &other);
   bool           operator != (const LogLine &other);

}; // class LogLine

typedef std::list <LogLine>   LogLineList;



}  // namespace plex



//#include  "LogLine.hpp"



#endif   // plex_LogLine_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

