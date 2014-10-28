/*****************************************************************************

        DataStreamOutput.cpp
        Copyright (c) 2010 Ohm Force

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

#include "ohm/flip/private/DataStreamOutput.h"

#include <cstring>
#include <cassert>



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

DataStreamOutput::DataStreamOutput ()
:  _ofs_ptr (0)
,  _data_ptr (0)
,  _str_ptr (0)

,  _buf_len (0)
,  _length (0)

,  _z_ptr (0)

{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DataStreamOutput::~DataStreamOutput ()
{
   try
   {
      close ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_file_output
==============================================================================
*/

int   DataStreamOutput::set_file_output (const char * path_0)
{
   assert (_ofs_ptr == 0);
   assert (_data_ptr == 0);
   assert (_str_ptr == 0);
   
   _ofs_ptr = new std::ofstream (path_0, std::ios::binary);
   
   if (!_ofs_ptr->good ())
   {
      return Err_DS_STREAM_OPEN_ERROR;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : set_mem_output
==============================================================================
*/

void  DataStreamOutput::set_mem_output (std::vector <archi::UByte> & data)
{
   assert (_ofs_ptr == 0);
   assert (_data_ptr == 0);
   assert (_str_ptr == 0);
   assert (&data != 0);
   
   _data_ptr = &data;
}



/*
==============================================================================
Name : set_mem_output
==============================================================================
*/

void  DataStreamOutput::set_mem_output (std::string & data)
{
   assert (_ofs_ptr == 0);
   assert (_data_ptr == 0);
   assert (_str_ptr == 0);
   assert (&data != 0);
   
   _str_ptr = &data;
}



/*
==============================================================================
Name : set_filter_zlib
==============================================================================
*/

void  DataStreamOutput::set_filter_zlib ()
{
   assert (_z_ptr == 0);
   
   _z_ptr = new z_stream;

   _z_ptr->zalloc = (alloc_func) 0;
   _z_ptr->zfree = (free_func) 0;
   _z_ptr->opaque = (voidpf) 0;

   int err = deflateInit (_z_ptr, Z_DEFAULT_COMPRESSION);
   //err = deflateInit (_z_ptr, 9);
   assert (err == Z_OK);
}



/*
==============================================================================
Name : print_begin
==============================================================================
*/

void  DataStreamOutput::print_begin ()
{
   // reset is not supported
   assert (_length == 0);
   
   _length = 0;
   
   if (_z_ptr != 0)
   {
      write_dst ("zlibxxxx", 8);
   }
}



/*
==============================================================================
Name : tell
==============================================================================
*/

size_t   DataStreamOutput::tell ()
{
   return _length;
}
   


/*
==============================================================================
Name : tell
==============================================================================
*/

void  DataStreamOutput::print_raw (const std::vector <archi::UByte> & data)
{
   if (_buf_len > 0)
   {
      flush ();
   }
   
   write_filter ((const char *) &data [0], data.size ());
   
   _length += data.size ();
}



/*
==============================================================================
Name : print_char
==============================================================================
*/

void  DataStreamOutput::print_char (char c)
{
   _buf [_buf_len] = c;
   ++_buf_len;
   
   if (_buf_len == BUF_LEN)
   {
      flush ();
   }
   
   ++_length;
}



/*
==============================================================================
Name : print_end
==============================================================================
*/

void  DataStreamOutput::print_end ()
{
   if (_buf_len > 0)
   {
      flush ();
   }
}



/*
==============================================================================
Name : close
==============================================================================
*/

void  DataStreamOutput::close ()
{
   if (_buf_len > 0)
   {
      flush ();
   }
   
   if (_z_ptr != 0)
   {
      int err = Z_OK;
      
      while (err != Z_STREAM_END)
      {
         char z_buf [Z_BUF_LEN];
         
         _z_ptr->avail_out = Z_BUF_LEN;
         _z_ptr->next_out = (Bytef *) z_buf;
         
         err = deflate (_z_ptr, Z_FINISH);
         assert ((err == Z_OK) || (err == Z_STREAM_END));
         
         if (_z_ptr->avail_out < Z_BUF_LEN)
         {
            write_dst (z_buf, Z_BUF_LEN - _z_ptr->avail_out);
         }
      }
      
      err = deflateEnd (_z_ptr);
      assert (err == 0);
      
      delete _z_ptr;
      _z_ptr = 0;
      
      char c_arr [4];
      
      for (int i = 0 ; i < 4 ; ++i)
      {
         archi::UByte byte = _length >> ((3 - i) * 8);
         c_arr [i] = byte;
      }
      
      if (_ofs_ptr != 0)
      {
         _ofs_ptr->seekp (4);
         
         _ofs_ptr->write (c_arr, 4);
      }
      else if (_data_ptr != 0)
      {
         for (int i = 0 ; i < 4 ; ++i)
         {
            (*_data_ptr) [i + 4] = c_arr [i];
         }
      }
      else if (_str_ptr != 0)
      {
         for (int i = 0 ; i < 4 ; ++i)
         {
            (*_str_ptr) [i + 4] = c_arr [i];
         }
      }
      else
      {
         assert (false);
      }
   }
   
   if (_ofs_ptr != 0)
   {
      _ofs_ptr->flush ();
      
      delete _ofs_ptr;
      _ofs_ptr = 0;
   }
   
   _data_ptr = 0;
   _str_ptr = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_reset
==============================================================================
*/

void  DataStreamOutput::do_reset ()
{
   // reset is not supported
   assert (_length == 0);
   
   _length = 0;
}



/*
==============================================================================
Name : do_is_truncated
==============================================================================
*/

bool  DataStreamOutput::do_is_truncated () const
{
   return false;
}



/*
==============================================================================
Name : do_get_string_len
==============================================================================
*/

long  DataStreamOutput::do_get_string_len () const
{
   return _length;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : flush
==============================================================================
*/

void  DataStreamOutput::flush ()
{
   write_filter (_buf, _buf_len);
   
   _buf_len = 0;
}



/*
==============================================================================
Name : write_filter
==============================================================================
*/

void  DataStreamOutput::write_filter (const char * buf_ptr, size_t buf_len)
{
   if (_z_ptr != 0)
   {
      char z_buf [Z_BUF_LEN];

      _z_ptr->avail_in = buf_len;
      _z_ptr->next_in = (Bytef *) buf_ptr;
      
      while (_z_ptr->avail_in > 0)
      {
         _z_ptr->avail_out = Z_BUF_LEN;
         _z_ptr->next_out = (Bytef *) z_buf;

         int err = deflate (_z_ptr, Z_NO_FLUSH);
         assert (err == 0);
         
         if (_z_ptr->avail_out < Z_BUF_LEN)
         {
            write_dst (z_buf, Z_BUF_LEN - _z_ptr->avail_out);
         }
      }
      
   }
   else
   {
      write_dst (buf_ptr, buf_len);
   }
}



/*
==============================================================================
Name : write_dst
==============================================================================
*/

void  DataStreamOutput::write_dst (const char * buf_ptr, size_t buf_len)
{
   if (_ofs_ptr != 0)
   {
      _ofs_ptr->write (buf_ptr, buf_len);
   }
   else if (_data_ptr != 0)
   {
      _data_ptr->insert (_data_ptr->end (), buf_ptr, buf_ptr + buf_len);
   }
   else if (_str_ptr != 0)
   {
      _str_ptr->insert (_str_ptr->end (), buf_ptr, buf_ptr + buf_len);
   }
   else
   {
      assert (false);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
