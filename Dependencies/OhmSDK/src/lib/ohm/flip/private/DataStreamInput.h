/*****************************************************************************

        DataStreamInput.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if ! defined (ohm_flip_DataStreamInput_HEADER_INCLUDED)
#define  ohm_flip_DataStreamInput_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Err.h"

#include <string>



namespace ohm
{
namespace flip
{



class BinaryKey;

class DataStreamInput
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DataStreamInput ();
   virtual        ~DataStreamInput ();
   
   int            open (const char * path_0);
   void           assign_no_copy (const archi::UByte * data_ptr, size_t data_size);
   
   void           print ();
   
   size_t         size ();
   
   int            seek_beg (size_t relative_pos);
   int            seek_end (size_t neg_relative_pos);
   int            seek (const std::string & pattern);
   int            rseek (const std::string & pattern);
   int            advance (const std::string & str);
   
   size_t         tell ();
   
   int            find (char c);
   int            find_first_not_of_ws ();
   int            find_first_not_of_crlf ();

   int            is_equal (bool & equal_flag, const std::string & pattern);
   int            require_keyword (const char * pattern_0);
   int            require_keyword (const std::string & pattern);
   int            require_char (const char * c_0);
   int            read (archi::Int64 & val);
   int            read (size_t & val);
   int            read (archi::Float64 & val);
   int            read (std::string & str);
   int            read (const char * & str_ptr, size_t & len);
   int            read (BinaryKey & key);
   int            read (archi::UByte * & data_ptr, size_t data_size);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   int            find_first_of (const char * char_arr_0);
   int            find_first_not_of (const char * char_arr_0);

   bool           is_of (const char * char_arr_0, size_t char_arr_len);
   bool           is_not_of (const char * char_arr_0, size_t char_arr_len);
   void           release ();
   
   const char *   _data_ptr;
   size_t         _data_size;
   bool           _allocated_flag;
   
   size_t         _pos;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataStreamInput (const DataStreamInput &other);
   DataStreamInput & operator = (const DataStreamInput &other);
   bool           operator == (const DataStreamInput &other);
   bool           operator != (const DataStreamInput &other);

}; // class DataStreamInput



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DataStreamInput.hpp"



#endif   // ohm_flip_DataStreamInput_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
