/*****************************************************************************

        ArgcArgvParser.cpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 45384

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

#include "ohm/util/ArgcArgvParser.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ArgcArgvParser::ArgcArgvParser ()
:  _parsed_flag (false)
,  _lexem_list ()
,  _lexem_0_list ()
{
   // Nothing
}



// Works with UTF-8 strings, but accepts only pure ASCII delimiters
void  ArgcArgvParser::parse_line (const char *line_0, bool includes_command_flag, const char *delimiter_list_0)
{
   assert (line_0 != 0);
   assert (delimiter_list_0 != 0);
   assert (delimiter_list_0 [0] != '\0');

#if ! defined (NDEBUG)
   {
      int            pos = 0;
      while (delimiter_list_0 [pos] != '\0')
      {
         const char     c = delimiter_list_0 [pos];
         assert (c >= 0);
         assert (int (c) <= 127);
         ++ pos;
      }
   }
#endif

   _lexem_list.clear ();
   if (! includes_command_flag)
   {
      _lexem_list.push_back ("");
   }
   int            argc = int (_lexem_list.size ());

   int            pos = 0;
   int            lex_pos = 0;
   bool           quote_flag = false;
   bool           argument_flag = false;
   std::string    lexem;

   while (line_0 [pos] != '\0')
   {
      const char     c = line_0 [pos];
      const bool     delimiter_flag = is_delimiter (c, delimiter_list_0);

      if (quote_flag)
      {
         if (! argument_flag)
         {
            lex_pos = pos;
            argument_flag = true;
         }

         if (c == '\"')
         {
            _lexem_list.push_back (std::string (line_0 + lex_pos, pos - lex_pos));
            ++ argc;
            quote_flag = false;
            argument_flag = false;
         }
      }

      else
      {
         if (c == '\"')
         {
            if (argument_flag)
            {
               _lexem_list.push_back (std::string (line_0 + lex_pos, pos - lex_pos));
               ++ argc;
               argument_flag = false;
            }

            quote_flag = true;
         }

         else if (argument_flag)
         {
            if (delimiter_flag)
            {
               _lexem_list.push_back (std::string (line_0 + lex_pos, pos - lex_pos));
               ++ argc;
               argument_flag = false;
            }
         }

         else if (! delimiter_flag)
         {
            lex_pos = pos;
            argument_flag = true;
         }
      }

      ++ pos;
   }

   if (argument_flag)
   {
      _lexem_list.push_back (std::string (line_0 + lex_pos, pos - lex_pos));
      ++ argc;
   }

   _lexem_0_list.clear ();
   for (int p = 0; p < argc; ++p)
   {
      _lexem_0_list.push_back (_lexem_list [p].c_str ());
   }

   _parsed_flag = true;
}



const ArgcArgvParser::LexemList &   ArgcArgvParser::use_lexem_list () const
{
   assert (_parsed_flag);

   return (_lexem_list);
}



void  ArgcArgvParser::get_argc_argv (int &argc, const char * const * &argv) const
{
   assert (_parsed_flag);
   assert (&argc != 0);
   assert (&argv != 0);

   argc = int (_lexem_list.size ());

   if (_lexem_0_list.size () > 0)
   {
      argv = &_lexem_0_list [0];
   }
   else
   {
      argv = 0;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  ArgcArgvParser::is_delimiter (char c, const char *delimiter_list_0)
{
   assert (delimiter_list_0 != 0);

   bool           delim_flag = false;

   while (*delimiter_list_0 != '\0' && ! delim_flag)
   {
      delim_flag = (c == *delimiter_list_0);
      ++delimiter_list_0;
   }

   return (delim_flag);
}



}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
