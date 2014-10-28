/*****************************************************************************

        PrintTubeEscaper.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55041

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
#include "ohm/txt/out/PrintTubeEscaper.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace out
{


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : escaper_xml
Description :
   static tube function to escape for an xml document.
Note :
   Note that :
   - we assume static tables that are programmed dynamically
   - we can produce escaper that are allocation free
==============================================================================
*/

void  PrintTubeEscaper::escaper_xml (PrintTubeEscaper & printer, char c)
{
   if (c == '\"')
   {
      printer.print_dst ("&quot;");
   }
   else if (c == '\'')
   {
      printer.print_dst ("&apos;");
   }
   else if (c == '<')
   {
      printer.print_dst ("&lt;");
   }
   else if (c == '>')
   {
      printer.print_dst ("&gt;");
   }
   else if (c == '&')
   {
      printer.print_dst ("&amp;");
   }
   else
   {
      printer.print_dst (c);
   }
}



/*
==============================================================================
Name : deescaper_xml
Description :
   De-escape an escaped str from the tube, when 'escaper_xml' is used.
==============================================================================
*/

std::string PrintTubeEscaper::deescaper_xml (const std::string & str)
{
   std::string ret_val;
   
   bool loop_flag = true;
   
   size_t pos = 0;
   
   while (loop_flag)
   {
      size_t pos2 = str.find ('&', pos);
      
      if (pos2 == std::string::npos)
      {
         ret_val += str.substr (pos);
         
         loop_flag = false;
      }
      else
      {
         ret_val += str.substr (pos, pos2 - pos);
         
         size_t pos3 = str.find (';', pos2);
         assert (pos3 != std::string::npos);
         
         pos3 += 1;
         
         std::string esc_str = str.substr (pos2, pos3 - pos2);
         
         if (esc_str == "&quot;")
         {
            ret_val += '\"';
         }
         else if (esc_str == "&apos;")
         {
            ret_val += '\'';
         }
         else if (esc_str == "&lt;")
         {
            ret_val += '<';
         }
         else if (esc_str == "&gt;")
         {
            ret_val += '>';
         }
         else if (esc_str == "&amp;")
         {
            ret_val += '&';
         }
         else
         {
            assert (false);
         }
         
         pos = pos3;
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

PrintTubeEscaper::PrintTubeEscaper (PrintInterface & dst, EscapeFnc escape_fnc)
:  _dst (dst)
,  _fnc (escape_fnc)
,  _escape_flag (false)
{
   assert (&dst != 0);
   assert (escape_fnc != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PrintTubeEscaper::~PrintTubeEscaper ()
{
   // Nothing
}



/*
==============================================================================
Name : esc_print
Description :
   Begins a print session which needs escaping.
Note :
   Sessions which do not need escaping can use the regular 'print' function
   member from PrintInterface
==============================================================================
*/

PrintTubeEscaper &   PrintTubeEscaper::esc_print (const char *format_0)
{
   _escape_flag = true;
   
   print (format_0);
   
   return *this;
}



/*
==============================================================================
Name : esc_cat
Description :
   Begins a cat session which needs escaping.
Note :
   Sessions which do not need escaping can use the regular 'cat' function
   member from PrintInterface
==============================================================================
*/

PrintTubeEscaper &   PrintTubeEscaper::esc_cat (const char *format_0)
{
   _escape_flag = true;
   
   cat (format_0);
   
   return *this;
}



/*
==============================================================================
Name : print_begin
==============================================================================
*/

void  PrintTubeEscaper::print_begin ()
{
   _dst.print_begin ();
}



/*
==============================================================================
Name : print_char
==============================================================================
*/

void  PrintTubeEscaper::print_char (char c)
{
   if (_escape_flag)
   {
      _fnc (*this, c);
   }
   else
   {
      _dst.print_char (c);
   }
}



/*
==============================================================================
Name : print_end
==============================================================================
*/

void  PrintTubeEscaper::print_end ()
{
   _escape_flag = false;
   
   _dst.print_end ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_reset
==============================================================================
*/

void  PrintTubeEscaper::do_reset ()
{
   _dst.reset ();
}



/*
==============================================================================
Name : do_is_truncated
==============================================================================
*/

bool  PrintTubeEscaper::do_is_truncated () const
{
   return _dst.is_truncated ();
}



/*
==============================================================================
Name : do_get_string_len
==============================================================================
*/

long  PrintTubeEscaper::do_get_string_len () const
{
   return get_string_len ();
}



/*
==============================================================================
Name : print_dst
==============================================================================
*/

void  PrintTubeEscaper::print_dst (const char * str_0)
{
   char c = str_0 [0];
   
   while (c != '\0')
   {
      _dst.print_char (c);
      
      ++str_0;
      c = str_0 [0];
   }
}



/*
==============================================================================
Name : print_dst
==============================================================================
*/

void  PrintTubeEscaper::print_dst (char c)
{
   _dst.print_char (c);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
