/*****************************************************************************

        Codec.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if ! defined (ohm_flip_Codec_HEADER_INCLUDED)
#define  ohm_flip_Codec_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Err.h"

#include <vector>



namespace ohm
{
namespace flip
{



class Codec
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Encoder
   {
                  Encoder_NONE = 0,
                  Encoder_BASE64,
                  Encoder_DEFLATE,
                  Encoder_ESCAPED_ASCII,
                  Encoder_HEXADECIMAL,
   };
   
   typedef std::vector <archi::UByte>  Data;
   
   virtual        ~Codec () {}
   
   static int     encode (Data & dst, const archi::UByte * src_ptr, size_t src_size, const std::vector <Encoder> & encoder_arr);
   static int     decode (Data & dst,const archi::UByte * src_ptr, size_t src_size, const std::vector <Encoder> & encoder_arr);

   static int     encode (Data & dst, const archi::UByte * src_ptr, size_t src_size, Encoder encoder);
   static int     decode (Data & dst,const archi::UByte * src_ptr, size_t src_size, Encoder encoder);
   
   static int     encode_base64 (Data & dst, const archi::UByte * src_ptr, size_t src_size);
   static int     encode_deflate (Data & dst, const archi::UByte * src_ptr, size_t src_size);
   static int     encode_escaped_ascii (Data & dst, const archi::UByte * src_ptr, size_t src_size);
   static int     encode_hexadecimal (Data & dst, const archi::UByte * src_ptr, size_t src_size);

   static int     decode_base64 (Data & dst, const archi::UByte * src_ptr, size_t src_size);
   static int     decode_deflate (Data & dst, const archi::UByte * src_ptr, size_t src_size);
   static int     decode_escaped_ascii (Data & dst, const archi::UByte * src_ptr, size_t src_size);
   static int     decode_hexadecimal (Data & dst, const archi::UByte * src_ptr, size_t src_size);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Codec ();
                  Codec (const Codec &other);
   Codec &        operator = (const Codec &other);
   bool           operator == (const Codec &other);
   bool           operator != (const Codec &other);

}; // class Codec



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/Codec.hpp"



#endif   // ohm_flip_Codec_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
