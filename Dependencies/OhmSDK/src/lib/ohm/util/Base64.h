/*****************************************************************************

        Base64.h
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 29282

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



#if ! defined (Base64_HEADER_INCLUDED)
#define  Base64_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <string>
#include <vector>



namespace ohm
{
namespace util
{



class Base64
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~Base64 ();

   static void    encode (std::string &dst_base64, const ohm::archi::UByte *src_ptr, size_t src_size, size_t line_wrap = 0, const char *line_end_0 = 0);
   static void    encode (std::string &dst_base64, const std::vector <ohm::archi::UByte> &src, size_t line_wrap = 0, const char *line_end_0 = 0);
   static void    decode (std::vector <ohm::archi::UByte> &dst, const std::string &src_base64);
   static char    encode_digit (int digit_6);
   static int     decode_char (char c);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
      
                  Base64 ();

   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   static void    insert_cr_if_needed (std::string & dst_base64, size_t & line_char_cnt, size_t line_wrap, const char * line_end_0);
   



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Base64 (const Base64 &other);
   Base64 &       operator = (const Base64 &other);
   bool           operator == (const Base64 &other);
   bool           operator != (const Base64 &other);

}; // class Base64



}  // namespace util
}  // namespace ohm



//#include  "Base64.hpp"



#endif   // Base64_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
