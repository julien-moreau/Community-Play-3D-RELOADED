/*****************************************************************************

        BinaryKey.h
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



#if ! defined (ohm_flip_BinaryKey_HEADER_INCLUDED)
#define  ohm_flip_BinaryKey_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Err.h"

#include <vector>
#include <string>



namespace ohm
{
namespace util
{
class BinaryStreamOutputBase;
}



namespace flip
{



class BinaryKey
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static BinaryKey
                  create_value_one ();
   
                  BinaryKey ();
                  BinaryKey (const BinaryKey & other);
   virtual        ~BinaryKey ();

   BinaryKey &    operator = (const BinaryKey & other);
   bool           operator < (const BinaryKey & other) const;
   bool           operator == (const BinaryKey & other) const;
   bool           operator != (const BinaryKey & other) const;
   
   bool           generate_random_between (const BinaryKey & l_op, const BinaryKey & r_op);
   void           generate_deterministic (size_t key_nbr);
   size_t         get_nbr_common_bits (const BinaryKey & l_op, const BinaryKey & r_op);
   
   void           write (std::string & dst) const;
   void           write (util::BinaryStreamOutputBase & bso) const;
   void           read (const std::string & src);
   int            read (const archi::UByte * data_ptr, size_t data_size_bytes, bool tolerant_flag);
   
   size_t         size () const;
   size_t         octal_size () const;
   const archi::UByte *
                  data () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   bool           get_bit (size_t i) const;
   void           set_bit (size_t i, bool val);
   void           copy (const BinaryKey & other, size_t nbr_bits);
   void           compact ();
   bool           is_compacted ();
   void           resize (size_t nbr_bits);
   
   size_t         bit_size_to_ceil_octal_size (size_t bit_size);
   size_t         bit_size_to_floor_octal_size (size_t bit_size);
   size_t         octal_size_to_bit_size (size_t octal_size);
   
   std::vector <archi::UByte>
                  _data;
   size_t         _size;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class BinaryKey



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/private/BinaryKey.hpp"



#endif   // ohm_flip_BinaryKey_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
