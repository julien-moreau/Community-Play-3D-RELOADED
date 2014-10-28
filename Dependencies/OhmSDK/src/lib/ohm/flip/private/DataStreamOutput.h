/*****************************************************************************

        DataStreamOutput.h
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



#if ! defined (ohm_flip_DataStreamOutput_HEADER_INCLUDED)
#define  ohm_flip_DataStreamOutput_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/txt/out/PrintInterface.h"
#include "ohm/flip/Err.h"

#include "zlib.h"

#include <fstream>
#include <string>
#include <vector>


namespace ohm
{
namespace flip
{



class BinaryKey;

class DataStreamOutput
:  public txt::out::PrintInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DataStreamOutput ();
   virtual        ~DataStreamOutput ();
   
   int            set_file_output (const char * path_0);
   void           set_mem_output (std::vector <archi::UByte> & data);
   void           set_mem_output (std::string & data);
   
   void           set_filter_zlib ();
   
   size_t         tell ();
   
   void           print_raw (const std::vector <archi::UByte> & data);
   
   // DevInterface
   virtual void   print_begin ();
   virtual void   print_char (char c);
   virtual void   print_end ();
   
   void           close ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // PrintInterface
   virtual void   do_reset ();
   virtual bool   do_is_truncated () const;
   virtual long   do_get_string_len () const;
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         BUF_LEN = 2048 };
   enum {         Z_BUF_LEN = 4096  };
   
   void           flush ();
   void           write_filter (const char * buf_ptr, size_t buf_len);
   void           write_dst (const char * buf_ptr, size_t buf_len);

   std::ofstream *_ofs_ptr;
   std::vector <archi::UByte> *
                  _data_ptr;
   std::string *  _str_ptr;
   
   char           _buf [BUF_LEN];
   size_t         _buf_len;
   
   size_t         _length;
   
   z_stream *     _z_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataStreamOutput (const DataStreamOutput &other);
   DataStreamOutput &   operator = (const DataStreamOutput &other);
   bool           operator == (const DataStreamOutput &other);
   bool           operator != (const DataStreamOutput &other);

}; // class DataStreamOutput



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DataStreamOutput.hpp"



#endif   // ohm_flip_DataStreamOutput_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
