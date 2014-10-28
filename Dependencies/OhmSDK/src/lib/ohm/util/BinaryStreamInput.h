/*****************************************************************************

        BinaryStreamInput.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70845

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



#if ! defined (ohm_util_BinaryStreamInput_HEADER_INCLUDED)
#define  ohm_util_BinaryStreamInput_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>



namespace ohm
{
namespace util
{



class BinaryStreamInput
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BinaryStreamInput ();
                  BinaryStreamInput (const BinaryStreamInput & other);
                  BinaryStreamInput (const archi::UByte * data_ptr, size_t data_size);
                  BinaryStreamInput (const std::vector <archi::UByte> & data);
   virtual        ~BinaryStreamInput ();

   BinaryStreamInput &
                  operator = (const BinaryStreamInput &other);

   const archi::UByte *
                  data () const;
   size_t         size () const;
   bool           empty () const;
   
   int            seek_beg (size_t relative_pos, const char * error_0 = 0);
   int            seek_end (size_t neg_relative_pos, const char * error_0 = 0);
   int            advance (long relative_pos, const char * error_0 = 0);
   
   size_t         tell () const;
   
   int            read (bool & val, const char * error_0 = 0);
   int            read (archi::UByte & val, const char * error_0 = 0);
   int            read (archi::SByte & val, const char * error_0 = 0);
   int            read (archi::UInt16 & val, const char * error_0 = 0);
   int            read (archi::Int16 & val, const char * error_0 = 0);
   int            read (archi::UInt32 & val, const char * error_0 = 0);
   int            read (archi::Int32 & val, const char * error_0 = 0);
   int            read (archi::UInt64 & val, const char * error_0 = 0);
   int            read (archi::Int64 & val, const char * error_0 = 0);
   int            read (archi::Float32 & val, const char * error_0 = 0);
   int            read (archi::Float64 & val, const char * error_0 = 0);
   int            read (void * & val, const char * error_0 = 0);
   int            read (const void * & val, const char * error_0 = 0);
   int            read (std::string & str, const char * error_0 = 0);
   int            read (const archi::UByte * & data_ptr, size_t data_size, const char * error_0 = 0);
   template <class T>
   int            read_ubyte (T & val, const char * error_0 = 0);
   template <class T>
   int            read_uint32 (T & val, const char * error_0 = 0);
   template <class T>
   int            read_int32 (T & val, const char * error_0 = 0);
   template <class T>
   int            read_float32 (T & val, const char * error_0 = 0);
   template <class T>
   int            read_float64 (T & val, const char * error_0 = 0);
   template <class T>
   int            read (std::list <T> & elem_arr, const char * error_0 = 0);
   template <class T>
   int            read (std::vector <T> & elem_arr, const char * error_0 = 0);
   template <class T>
   int            read (std::set <T> & elem_set, const char * error_0 = 0);
   template <class T, class U>
   int            read (std::map <T, U> & elem_map, const char * error_0 = 0);
   template <class T, class U>
   int            read (std::pair <T, U> & val, const char * error_0 = 0);
   template <class T>
   int            read (T & val, const char * error_0 = 0);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   const archi::UByte *
                  _data_ptr;
   size_t         _data_size;
   
   size_t         _pos;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const BinaryStreamInput &other);
   bool           operator != (const BinaryStreamInput &other);

}; // class BinaryStreamInput



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/util/BinaryStreamInput.hpp"



#endif   // ohm_util_BinaryStreamInput_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
