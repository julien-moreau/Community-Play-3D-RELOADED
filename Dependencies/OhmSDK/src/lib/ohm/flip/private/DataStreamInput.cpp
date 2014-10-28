/*****************************************************************************

        DataStreamInput.cpp
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

#include "ohm/flip/private/DataStreamInput.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/txt/ascii/ascii_fnc.h"

#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>

#include <climits>
#include <cassert>

#define OHM_FLIP_DATA_STREAM_DEBUG_FLAG

#if defined (OHM_FLIP_DATA_STREAM_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DATA_STREAM_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define ds_break static const char * data_stream_ptr = &_data_ptr [_pos]; assert (false); data_stream_ptr
#else
   #define ds_break ((void) 0)
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

DataStreamInput::DataStreamInput ()
:  _data_ptr (0)
,  _data_size (0)
,  _allocated_flag (false)
,  _pos (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DataStreamInput::~DataStreamInput ()
{
   release ();
}



/*
==============================================================================
Name : open
==============================================================================
*/

int   DataStreamInput::open (const char * path_0)
{
   release ();
   
   std::ifstream  ifs (path_0, std::ios::binary);
   
   if (!ifs.good ())
   {
      return Err_DS_STREAM_OPEN_ERROR;
   }
   
   ifs.seekg (0, std::ios::end);

   if (!ifs.good ())
   {
      return Err_DS_STREAM_SEEK_ERROR;
   }
   
   _data_size = ifs.tellg ();
   
   ifs.seekg (0, std::ios::beg);
   
   if (!ifs.good ())
   {
      return Err_DS_STREAM_SEEK_ERROR;
   }
   
   if (_data_size > 0)
   {
      char * data_ptr = new char [_data_size];
   
      ifs.read (data_ptr, _data_size);
      
      if (!ifs.good ())
      {
         return Err_DS_STREAM_READ_ERROR;
      }
      
      _data_ptr = data_ptr;
      _allocated_flag = true;
      _pos = 0;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : assign_no_copy
==============================================================================
*/

void  DataStreamInput::assign_no_copy (const archi::UByte * data_ptr, size_t data_size)
{
   release ();
   
   _data_ptr = reinterpret_cast <const char *> (data_ptr);
   _data_size = data_size;
   _allocated_flag = false;
   _pos = 0;
}



/*
==============================================================================
Name : print
Description :
   Print the data stream to std out
==============================================================================
*/

void  DataStreamInput::print ()
{
#if ! defined (ohm_flip_SERVER)
   // use printf, as the trace is size limited
   
   for (size_t i = 0 ; i < _data_size ; ++i)
   {
      char c = _data_ptr [i];
      
      printf ("%c", c);
   }
   printf ("\n");
#endif
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   DataStreamInput::size ()
{
   return _data_size;
}


   
/*
==============================================================================
Name : seek_beg
Description :
   seek to 'relative_pos' relative to begin of the stream
==============================================================================
*/

int   DataStreamInput::seek_beg (size_t relative_pos)
{
   _pos = relative_pos;

   if (_pos >= _data_size)
   {
      _pos = _data_size - 1;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : seek_end
Description :
   seek to - 'neg_relative_pos' relative to end of the stream
==============================================================================
*/

int   DataStreamInput::seek_end (size_t neg_relative_pos)
{
   if (neg_relative_pos > _data_size)
   {
      _pos = 0;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   _pos = _data_size - neg_relative_pos;

   return Err_OK;
}



/*
==============================================================================
Name : seek
==============================================================================
*/

int   DataStreamInput::seek (const std::string & pattern)
{
   const char * pos_ptr = std::search (
      &_data_ptr [_pos], &_data_ptr [_data_size],
      pattern.begin (), pattern.end ()
   );
   
   _pos = pos_ptr - &_data_ptr [_pos];
   
   if (_pos >= _data_size)
   {
      _pos = _data_size;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : rseek
==============================================================================
*/

int   DataStreamInput::rseek (const std::string & pattern)
{
   const char * pos_ptr = std::find_end (
      &_data_ptr [0], &_data_ptr [_pos],
      pattern.begin (), pattern.end ()
   );
   
   _pos = pos_ptr - &_data_ptr [0];
   
   if (_pos >= _data_size)
   {
      _pos = 0;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : advance
==============================================================================
*/

int   DataStreamInput::advance (const std::string & str)
{
   _pos += str.size ();
   
   if (_pos >= _data_size)
   {
      _pos = _data_size;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : tell
==============================================================================
*/

size_t   DataStreamInput::tell ()
{
   return _pos;
}

   

/*
==============================================================================
Name : find
==============================================================================
*/

int   DataStreamInput::find (char c)
{
   const char * pos_ptr = std::find (
      &_data_ptr [_pos], &_data_ptr [_data_size],
      c
   );
   
   _pos = pos_ptr - &_data_ptr [0];
   
   if (_pos >= _data_size)
   {
      _pos = _data_size;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}
   


/*
==============================================================================
Name : find_first_not_of_ws
==============================================================================
*/

int   DataStreamInput::find_first_not_of_ws ()
{
#if 0
   return find_first_not_of (" \t\n\r");

#elif 1
   const char * pos_ptr = &_data_ptr [_pos];
   const char * end_pos_ptr = &_data_ptr [_data_size];
   
   for (; pos_ptr < end_pos_ptr ; ++pos_ptr)
   {
      char c = *pos_ptr;
      
      if (
         (c == ' ')
         || (c == '\t')
         || (c == '\n')
         || (c == '\r')
         )
      {
         // nothing
      }
      else
      {
         break;
      }
   }
   
   _pos = pos_ptr - &_data_ptr [0];
   
   if (_pos >= _data_size)
   {
      _pos = _data_size;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
   
#endif
}
   


/*
==============================================================================
Name : find_first_not_of_crlf
==============================================================================
*/

int   DataStreamInput::find_first_not_of_crlf ()
{
   return find_first_not_of ("\n\r");
}
   


/*
==============================================================================
Name : is_equal
==============================================================================
*/

int   DataStreamInput::is_equal (bool & equal_flag, const std::string & pattern)
{
   size_t old_pos = _pos;
   size_t end_pos = _pos + pattern.size ();
   
   if (end_pos > _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   equal_flag = true;
   size_t   str_pos = 0;
   
   for (; _pos < end_pos ; ++_pos, ++str_pos)
   {
      equal_flag = _data_ptr [_pos] == pattern [str_pos];
      
      if (!equal_flag)
      {
         break;
      }
   }
   
   if (equal_flag)
   {
      _pos = end_pos;
   }
   else
   {
      _pos = old_pos;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : require_keyword
==============================================================================
*/

int   DataStreamInput::require_keyword (const char * pattern_0)
{
   if (_pos >= _data_size)
   {
      return Err_DS_EOF;
   }
   
   const char * pos_ptr = &_data_ptr [_pos];
   const char * end_pos_ptr = &_data_ptr [_data_size];
   
   while (true)
   {
      char pat_c = *pattern_0;
      
      if (pat_c == '\0')
      {
         break;
      }

      if (pos_ptr >= end_pos_ptr)
      {
         ds_break;
         return Err_DS_EOF;
      }
      
      char c = *pos_ptr;
      
      if (c != pat_c)
      {
         ds_break;
         return Err_DS_UNEXPECTED_KEYWORD;
      }
      
      ++pattern_0;
      ++pos_ptr;
   }
   
   if (pos_ptr >= end_pos_ptr)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   char c = *pos_ptr;
   
   bool ok_flag = (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
   
   if (!ok_flag)
   {
      ds_break;
      return Err_DS_UNEXPECTED_KEYWORD;
   }
   
   _pos = pos_ptr - &_data_ptr [0];
   
   return Err_OK;
}


   
/*
==============================================================================
Name : require_keyword
==============================================================================
*/

int   DataStreamInput::require_keyword (const std::string & pattern)
{
   std::string str;
   
   int err = read (str);
   if (err != Err_OK)   return err;
   
   if (str != pattern)
   {
      ds_break;
      return Err_DS_UNEXPECTED_KEYWORD;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : require_char
==============================================================================
*/

int   DataStreamInput::require_char (const char * c_0)
{
   if (_pos + 1 >= _data_size)
   {
      return Err_DS_EOF;
   }
   
   char sub_c = _data_ptr [_pos];
   
   if (sub_c != c_0 [0])
   {
      ds_break;
      return Err_DS_UNEXPECTED_TOKEN;
   }
   
   _pos += 1;
   
   return Err_OK;
}


   
/*
==============================================================================
Name : read
==============================================================================
*/

int   DataStreamInput::read (archi::Int64 & val)
{
   if (_pos >= _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   bool ok_flag = is_of ("0123456789-", 11);
   if (!ok_flag)
   {
      ds_break;
      return Err_DS_UNEXPECTED_TOKEN;
   }
   
   int s = 1;
   if (_data_ptr [_pos] == '-')
   {
      s = -1;
      ++ _pos;
   }
   
   size_t start_pos = _pos;

   int err = find_first_not_of ("0123456789");  if (err != Err_OK)   return err;
   size_t end_pos = _pos;
   
   val = 0;
   
   for (_pos = start_pos ; _pos < end_pos ; ++_pos)
   {
      const archi::Int64   old_val = val;
      
      char c = _data_ptr [_pos];
      int digit = c - '0';
      
      val *= 10;
      val += digit * s;
      
      if ((val / 10) != old_val)
      {
         ds_break;
         val = s == 1 ? archi::INT64_MAX_VAL : archi::INT64_MIN_VAL;
         return Err_DS_INTEGER_OVERFLOW;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   DataStreamInput::read (size_t & val)
{
   val = 0;
   
   archi::Int64 val_64 = 0;
   int err = read (val_64);   if (err != Err_OK)   return err;
   
   // 6.5.3.4 size_t is unsigned int
   
   if (val_64 > archi::Int64 (UINT_MAX))
   {
      ds_break;
      return Err_DS_INTEGER_OVERFLOW;
   }
   
   if (val_64 < 0)
   {
      ds_break;
      return Err_DS_INTEGER_UNDERFLOW;
   }
   
   val = static_cast <size_t> (val_64);
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   DataStreamInput::read (archi::Float64 & val)
{
   if (_pos >= _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   bool ok_flag = is_of ("0123456789-", 11);
   if (!ok_flag)
   {
      ds_break;
      return Err_DS_UNEXPECTED_TOKEN;
   }
   
   bool hex_flag = false;
   
   if (_data_ptr [_pos] == '0')
   {
      if (_pos + 1 < _data_size)
      {
         hex_flag = _data_ptr [_pos + 1] == 'x';
      }
   }
   
   if (hex_flag)
   {
      // hex version
      _pos += 2;
      
      size_t start_pos = _pos;
      
      int err = find_first_not_of ("0123456789abcdef");  if (err != Err_OK)   return err;
      size_t end_pos = _pos;
      size_t hex_size = end_pos - start_pos;
      
      if (hex_size != 16)
      {
         ds_break;
         return Err_DS_FLOAT_HEX_WRONG_SIZE;
      }
      
      archi::UInt64 tval = 0;

#if 0    
      _pos = start_pos;

      for (; _pos < end_pos ; ++_pos)
      {
         char c = _data_ptr [_pos];
         
         archi::UByte h = 0;

         if ((c >= '0') && (c <= '9'))
         {
            h = c - '0';
         }
         else if ((c >= 'a') && (c <= 'f'))
         {
            h = 10 + c - 'a';
         }
         
         tval <<= 4LL;
         tval += h;
      }

#elif 1
      char buf [16];
      memcpy (buf, &_data_ptr [start_pos], 16);

      for (int i = 0 ; i < 16 ; ++i)
      {
         char c = buf [i];
         
         int h = 0;

         if ((c >= '0') && (c <= '9'))
         {
            h = c - '0';
         }
         else if ((c >= 'a') && (c <= 'f'))
         {
            h = 10 + c - 'a';
         }
         
         tval <<= 4LL;
         tval |= h;
      }

#endif
      
      reinterpret_cast <archi::UInt64 &> (val) = tval;
   }
   else
   {
      int s = 1;
      if (_data_ptr [_pos] == '-')
      {
         s = -1;
         ++ _pos;
      }
      
      size_t start_pos = _pos;

      int err = find_first_not_of ("0123456789.e+-"); if (err != Err_OK)   return err;
      size_t end_pos = _pos;
      
      _pos = start_pos;
      
      val = 0.0;
      
      std::string str;
      str.assign (_data_ptr + start_pos, _data_ptr + end_pos);
      
      ohm::txt::ascii::conv_str_to_double (val, str.c_str ());
      
      assert (val >= 0.0);
      
      val *= double (s);
      
      _pos = end_pos;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   DataStreamInput::read (std::string & str)
{
   str.clear ();
   
   if (_pos >= _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   bool ok_flag = is_not_of (" \t\n\r", 4);

   if (!ok_flag)
   {
      ds_break;
      return Err_DS_UNEXPECTED_TOKEN;
   }
   
   size_t start_pos = _pos;

   int err = find_first_of (" \t\n\r");   if (err != Err_OK)   return err;
   size_t end_pos = _pos;
   
   str.assign (&_data_ptr [start_pos], &_data_ptr [end_pos]);
   
   return Err_OK;
}



/*
==============================================================================
Name : read
Note :
   No allocation/deallocation version of 'read (std::string &)'
   If successful, on output :
   - 'str_ptr' points to the start of the string
   - 'len' is the length of the string.
==============================================================================
*/

int   DataStreamInput::read (const char * & str_ptr, size_t & len)
{
   if (_pos >= _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   bool ok_flag = is_not_of (" \t\n\r", 4);

   if (!ok_flag)
   {
      ds_break;
      return Err_DS_UNEXPECTED_TOKEN;
   }
   
   size_t start_pos = _pos;

   int err = find_first_of (" \t\n\r");   if (err != Err_OK)   return err;
   
   len = _pos - start_pos;
   
   str_ptr = &_data_ptr [start_pos];
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   DataStreamInput::read (BinaryKey & key)
{
   if (_pos + 2 >= _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   bool ok_flag = _data_ptr [_pos] == '0';
   ++_pos;
   ok_flag &= _data_ptr [_pos] == '.';
   ++_pos;
   
   if (!ok_flag)
   {
      ds_break;
      return Err_DOC_ARRAY_WRONG_KEY;
   }
   
   size_t start_pos = _pos;

   int err = find_first_of (" \t\n\r");   if (err != Err_OK)   return err;
   size_t end_pos = _pos;
   
   size_t length = end_pos - start_pos;
   
   std::vector <archi::UByte> data ((length) / 2 + 1, 0);
   
   for (size_t i = 0 ; i < length ; ++i)
   {
      size_t bin_pos = i / 2;
      size_t bin_part = i % 2;
      
      char c = _data_ptr [start_pos + i];
      
      archi::UByte bits = 0;
      
      if ((c >= '0') && (c <= '9'))
      {
         bits = c - '0';
      }
      else if ((c >= 'a') && (c <= 'f'))
      {
         bits = c - 'a' + 10;
      }
      
      if (bin_part == 0)
      {
         data [bin_pos] |= (bits << 3);
      }
      else
      {
         data [bin_pos] |= (bits >> 1);
         data [bin_pos + 1] |= ((bits & 1) << 7);
      }
   }
   
   key.read (&data [0], data.size (), false);
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   DataStreamInput::read (archi::UByte * & data_ptr, size_t data_size)
{
   if (_pos + data_size > _data_size)
   {
      ds_break;
      return Err_DS_EOF;
   }
   
   data_ptr = (archi::UByte *) &_data_ptr [_pos];
   
   _pos += data_size;
   
   return Err_OK;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : find_first_of
==============================================================================
*/

int   DataStreamInput::find_first_of (const char * char_arr_0)
{
   using namespace std;
   
   const char * pos_ptr = std::find_first_of (
      &_data_ptr [_pos], &_data_ptr [_data_size],
      char_arr_0, char_arr_0 + strlen (char_arr_0)
   );
   
   _pos = pos_ptr - &_data_ptr [0];
   
   if (_pos >= _data_size)
   {
      _pos = _data_size;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}
   


class unary_predicate_not_all
{
public:
   unary_predicate_not_all (const char * char_arr_0) : _char_arr_0 (char_arr_0)
   {
      _end_char_arr_0 = char_arr_0 + strlen (char_arr_0);
   }
   
   bool  operator () (char c)
   {
      return std::find (_char_arr_0, _end_char_arr_0, c) == _end_char_arr_0;
   }

private:
   const char * _char_arr_0;
   const char * _end_char_arr_0;
};
   


/*
==============================================================================
Name : find_first_not_of
==============================================================================
*/

int   DataStreamInput::find_first_not_of (const char * char_arr_0)
{
   const char * pos_ptr = std::find_if (
      &_data_ptr [_pos], &_data_ptr [_data_size],
      unary_predicate_not_all (char_arr_0)
   );
   
   _pos = pos_ptr - &_data_ptr [0];
   
   if (_pos >= _data_size)
   {
      _pos = _data_size;
      
      ds_break;
      return Err_DS_EOF;
   }
   
   return Err_OK;
}
   


/*
==============================================================================
Name : is_of
==============================================================================
*/

bool  DataStreamInput::is_of (const char * char_arr_0, size_t char_arr_len)
{
   if (_pos >= _data_size)
   {
      return false;
   }
   
   char c = _data_ptr [_pos];
   
   return std::find (char_arr_0, char_arr_0 + char_arr_len, c) != char_arr_0 + char_arr_len;
}



/*
==============================================================================
Name : is_not_of
==============================================================================
*/

bool  DataStreamInput::is_not_of (const char * char_arr_0, size_t char_arr_len)
{
   if (_pos >= _data_size)
   {
      return false;
   }
   
   char c = _data_ptr [_pos];

   return std::find (char_arr_0, char_arr_0 + char_arr_len, c) == char_arr_0 + char_arr_len;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  DataStreamInput::release ()
{
   if (_allocated_flag)
   {
      delete [] _data_ptr;
      _data_ptr = 0;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
