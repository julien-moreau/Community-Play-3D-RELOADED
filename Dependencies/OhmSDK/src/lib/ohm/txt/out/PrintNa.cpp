/*****************************************************************************

        PrintNa.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 20573

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/txt/out/PrintNa.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace out
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



PrintNa::PrintNa (char *output_0, long max_len)
:  _output_0 (output_0)
,  _max_len (max_len)
,  _cur_write_pos (0)
{
   assert (max_len >= 0);
}



PrintNa::~PrintNa ()
{
   // Nothing
}



void  PrintNa::print_begin ()
{
   _cur_write_pos = 0;
}



void  PrintNa::print_char (char c)
{
   assert (_output_0 != 0);

   if (_cur_write_pos < _max_len)
   {
      _output_0 [_cur_write_pos] = c;
   }

   ++_cur_write_pos;
}



void  PrintNa::print_end ()
{
   const long     term_pos = lang::min (_cur_write_pos, _max_len);
   _output_0 [term_pos] = '\0';
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  PrintNa::do_reset ()
{
   _cur_write_pos = 0;
}



bool  PrintNa::do_is_truncated () const
{
   return (_cur_write_pos > _max_len);
}



long  PrintNa::do_get_string_len () const
{
   return (_cur_write_pos);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
