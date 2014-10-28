/*****************************************************************************

        BinaryKey.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/private/RandomGenerator.h"
#include "ohm/lang/fnc.h"
#include "ohm/util/BinaryStreamOutputBase.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : create_value_one
Description :
   Create the limit value "1."
==============================================================================
*/

BinaryKey   BinaryKey::create_value_one ()
{
   BinaryKey ret_val;
   ret_val._data.push_back (0x80);
   ret_val._size = 1;
   
   return ret_val;
}
   


/*
==============================================================================
Name : ctor
==============================================================================
*/

BinaryKey::BinaryKey ()
:  _data ()
,  _size (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

BinaryKey::BinaryKey (const BinaryKey & other)
:  _data (other._data)
,  _size (other._size)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BinaryKey::~BinaryKey ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

BinaryKey & BinaryKey::operator = (const BinaryKey & other)
{
   _data = other._data;
   _size = other._size;
   
   return *this;
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  BinaryKey::operator < (const BinaryKey & other) const
{
   size_t min_octal_size = lang::min (_data.size (), other._data.size ());
   
   for (size_t i = 0 ; i < min_octal_size ; ++i)
   {
      archi::UByte b = _data [i];
      archi::UByte ob = other._data [i];
      
      if (b < ob)
      {
         return true;
      }
      else if (b > ob)
      {
         return false;
      }
   }
   
   // *this and other are equal until there
   
   return (_size < other._size);
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  BinaryKey::operator == (const BinaryKey & other) const
{
   return (_size == _size) && (_data == other._data);
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  BinaryKey::operator != (const BinaryKey & other) const
{
   return (_size != _size) || (_data != other._data);
}



/*
==============================================================================
Name : generate_random_between
Description :
   Generate a random key between l_op and r_op exclusive, that is :
   the assertion l_op < *this < r_op is true if l_op < r_op
Return :
   true if the operation suceeded
   false if l_op == r_op, in that case *this is untouched
==============================================================================
*/

bool  BinaryKey::generate_random_between (const BinaryKey & l_op, const BinaryKey & r_op)
{
   bool ok_flag = l_op < r_op;
   assert (ok_flag);
   
   if (ok_flag)
   {
      size_t max_size = lang::max (l_op._size, r_op._size);
      size_t nbr_common_bits = get_nbr_common_bits (l_op, r_op);
      
      size_t new_size = lang::max (max_size + 16, nbr_common_bits + 32);
      
      resize (new_size);
      
      copy (l_op, nbr_common_bits);
      
      size_t rem_bits = _size - nbr_common_bits;
      
      std::vector <archi::UByte> rand_buf;
      rand_buf.resize ((rem_bits + 7) / 8);
      
      RandomGenerator::use_instance ().get (&rand_buf [0], rand_buf.size ());

      size_t i2 = 0;
      bool need_gt_l_op_flag = true;
      bool all_zero_bits = true;
      
      for (size_t i = nbr_common_bits ; i < _size ; ++i, ++i2)
      {
         bool val = (rand_buf [i2 / 8] >> (i2 % 8)) & 1;
         
         if (i == nbr_common_bits)
         {
            assert (l_op.get_bit (i) == 0);
            assert (r_op.get_bit (i) == 1);
            val = 0;
         }
         
         if (need_gt_l_op_flag)
         {
            val |= l_op.get_bit (i);
         }
         
         if (val > l_op.get_bit (i))
         {
            need_gt_l_op_flag = false;
         }
         
         if (val != 0)
         {
            all_zero_bits = false;
         }
         
         if ((i == _size - 1) && (all_zero_bits))
         {
            val = 1;
         }
         
         set_bit (i, val);
      }
      
      assert (!need_gt_l_op_flag);

      // compact the key so that the is no trailing 0
      
      compact ();
      
      if (!is_compacted ())
      {
         std::string str;
         write (str);
         
         ohm_util_TRACE_PANIC ("Key not compacted");
         ohm_util_TRACE_INFO1 ("%1%", str.c_str ());
         assert (false);
      }
      
      if (!(l_op < *this))
      {
         ohm_util_TRACE_PANIC ("l_op < *this is false");

         std::string l_str;
         l_op.write (l_str);

         std::string str;
         write (str);
         
         std::string r_str;
         r_op.write (r_str);

         ohm_util_TRACE_INFO1 ("%1%", l_str.c_str ());
         ohm_util_TRACE_INFO1 ("%1%", str.c_str ());
         ohm_util_TRACE_INFO1 ("%1%", r_str.c_str ());
         
         assert (false);
      }
      
      if (!(*this < r_op))
      {
         ohm_util_TRACE_PANIC ("*this < r_op is false");

         std::string l_str;
         l_op.write (l_str);

         std::string str;
         write (str);
         
         std::string r_str;
         r_op.write (r_str);

         ohm_util_TRACE_INFO1 ("%1%", l_str.c_str ());
         ohm_util_TRACE_INFO1 ("%1%", str.c_str ());
         ohm_util_TRACE_INFO1 ("%1%", r_str.c_str ());
         
         assert (false);
      }
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : generate_deterministic
==============================================================================
*/

void  BinaryKey::generate_deterministic (size_t key_nbr)
{
   resize (key_nbr + 2);
   
   set_bit (0, 0);
   
   for (size_t i = 1 ; i < _size ; ++i)
   {
      set_bit (i, 1);
   }
}



/*
==============================================================================
Name : print
==============================================================================
*/

size_t   BinaryKey::get_nbr_common_bits (const BinaryKey & l_op, const BinaryKey & r_op)
{
   size_t max_size = lang::max (l_op._size, r_op._size);
   
   size_t i = 0;
   
   for (; i < max_size ; ++i)
   {
      bool l_val = l_op.get_bit (i);
      bool r_val = r_op.get_bit (i);
      
      if (l_val != r_val)
      {
         break;
      }
   }
   
   return i;
}



/*
==============================================================================
Name : write
Description :
   Write the binary key in hexadecimal floating format, with the first
   bit being the integral part of the float.
==============================================================================
*/

void  BinaryKey::write (std::string & dst) const
{
   dst.clear ();
   
   dst += "0.";
   
   if (_size > 1)
   {
      // binary key is < 1.
      assert (get_bit (0) == 0);
      assert (get_bit (_size - 1) == true);
      
      for (size_t i = 1 ; i < _size ; i += 4)
      {
         int   b0 = get_bit (i + 0);
         int   b1 = get_bit (i + 1);
         int   b2 = get_bit (i + 2);
         int   b3 = get_bit (i + 3);
         
         int h = b0 * 8 + b1 * 4 + b2 * 2 + b3;
         assert (h >= 0); assert (h < 16);

         char c;
         if (h <= 9)
         {
            c = '0' + h;
         }
         else
         {
            c = 'a' + (h - 10);
         }
         
         dst += c;
      }
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryKey::write (util::BinaryStreamOutputBase & bso) const
{
   assert (!_data.empty ());
   
   bso.write (archi::UInt32 (_data.size ()));
   bso.write (&_data [0], _data.size ());
}



/*
==============================================================================
Name : read
==============================================================================
*/

void  BinaryKey::read (const std::string & src)
{
   assert (src.size () >= 2);
   assert (src [0] == '0');
   assert (src [1] == '.');
   
   size_t max_size = 1 + (src.size () - 2) * 4;
   
   resize (max_size);
   set_bit (0, 0);
   
   size_t bit_ctr = 0;
   for (size_t i = 2 ; i < src.size () ; ++i, bit_ctr += 4)
   {
      char c = src [i];
      int h = 0;
      
      if ((c >= '0') && (c <= '9'))
      {
         h = c - '0';
      }
      else if ((c >= 'a') && (c <= 'f'))
      {
         h = 10 + c - 'a';
      }
      assert (h >= 0); assert (h < 16);
      
      set_bit (bit_ctr + 0, (h >> 3) & 1);
      set_bit (bit_ctr + 1, (h >> 2) & 1);
      set_bit (bit_ctr + 2, (h >> 1) & 1);
      set_bit (bit_ctr + 3, (h >> 0) & 1);
   }
   
   compact ();
}



/*
==============================================================================
Name : read
==============================================================================
*/

int BinaryKey::read (const archi::UByte * data_ptr, size_t data_size_bytes, bool tolerant_flag)
{
   if (data_size_bytes < 1)
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_KEY_TOO_SMALL;
   }
   
   if ((data_ptr [0] & 0x80) != 0)
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_KEY_INVALID;
   }
   
   resize (data_size_bytes * 8);
   
   for (size_t i = 0 ; i < _data.size () ; ++i)
   {
      _data [i] = data_ptr [i];
   }
   
   compact ();
   
   if (_size <= 1)
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_KEY_TOO_SMALL;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   BinaryKey::size () const
{
   return _size;
}



/*
==============================================================================
Name : octal_size
==============================================================================
*/

size_t   BinaryKey::octal_size () const
{
   return _data.size ();
}



/*
==============================================================================
Name : data
==============================================================================
*/

const archi::UByte * BinaryKey::data () const
{
   return &_data [0];
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_bit
==============================================================================
*/

bool  BinaryKey::get_bit (size_t i) const
{
   if (i < _size)
   {
      archi::UByte octet = _data [i / 8];
      
      bool ret_val = (octet >> (7 - (i % 8))) & 1;
      
      return ret_val;
   }
   
   return 0;
}



/*
==============================================================================
Name : set_bit
==============================================================================
*/

void  BinaryKey::set_bit (size_t i, bool val)
{
   assert (i < _size);
   
   archi::UByte & octet = _data [i / 8];
   archi::UByte mask = (val & 1) << (7 - (i % 8));
   octet |= mask;
}



/*
==============================================================================
Name : copy
==============================================================================
*/

void  BinaryKey::copy (const BinaryKey & other, size_t nbr_bits)
{
   assert (nbr_bits <= _size);
   
   size_t octet_size_part = bit_size_to_floor_octal_size (nbr_bits);
   
   for (size_t i = 0 ; i < octet_size_part ; ++i)
   {
      archi::UByte byte = 0;
      
      if (i < other._data.size ())
      {
         byte = other._data [i];
      }
      
      _data [i] = byte;
   }
   
   size_t rem_start_bit = octal_size_to_bit_size (octet_size_part);
   
   for (size_t i = rem_start_bit ; i < nbr_bits ; ++i)
   {
      bool val = other.get_bit (i);
      
      set_bit (i, val);
   }
}



/*
==============================================================================
Name : compact
Description :
   Readjust the size of the key so that it ends on the last set bit
==============================================================================
*/

void  BinaryKey::compact ()
{
   if (_size > 0)
   {
      int last_set_bit = _size - 1;
      
      for ( ; last_set_bit >= 0 ; --last_set_bit)
      {
         if (get_bit (last_set_bit) == true)
         {
            break;
         }
      }
      
      _size = last_set_bit + 1;
      
      size_t new_octal_size = bit_size_to_ceil_octal_size (_size);
      
      _data.resize (new_octal_size);
   }
}



/*
==============================================================================
Name : is_compacted
Description :
   Readjust the size of the key so that it ends on the last set bit
==============================================================================
*/

bool  BinaryKey::is_compacted ()
{
   bool last_bit = get_bit (_size - 1);
   
   return last_bit != 0;
}



/*
==============================================================================
Name : resize
Description :
   Resize the key by nbr_bits. If the key is enlarged, the new bits are set
   to 0. In all cases the last octet corresponding to the last bit is filled
   with 0 after nbr_bits
==============================================================================
*/

void  BinaryKey::resize (size_t nbr_bits)
{
   size_t old_octal_size = _data.size ();

   _size = nbr_bits;
   _data.resize (bit_size_to_ceil_octal_size (_size));
   
   for (size_t i = old_octal_size ; i < _data.size () ; ++i)
   {
      _data [i] = 0;
   }
   
   archi::UByte last_octet = 0;
   
   size_t last_octet_bit_start = octal_size_to_bit_size (_data.size () - 1);
   
   for (size_t i = last_octet_bit_start ; i < _size ; ++i)
   {
      last_octet |= get_bit (i) << (7 - (i % 8));
   }
   
   _data [_data.size () - 1] = last_octet;
}



/*
==============================================================================
Name : bit_size_to_ceil_octal_size
==============================================================================
*/

size_t   BinaryKey::bit_size_to_ceil_octal_size (size_t bit_size)
{
   return (bit_size + 7) / 8;
}



/*
==============================================================================
Name : bit_size_to_ceil_octal_size
==============================================================================
*/

size_t   BinaryKey::bit_size_to_floor_octal_size (size_t bit_size)
{
   return bit_size / 8;
}



/*
==============================================================================
Name : bit_size_to_ceil_octal_size
==============================================================================
*/

size_t   BinaryKey::octal_size_to_bit_size (size_t octal_size)
{
   return octal_size * 8;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
