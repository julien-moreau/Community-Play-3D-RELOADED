/*****************************************************************************

        ArgcArgvParser.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18579

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



#if ! defined (ohm_util_ArgcArgvParser_HEADER_INCLUDED)
#define  ohm_util_ArgcArgvParser_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <string>
#include <vector>



namespace ohm
{
namespace util
{



class ArgcArgvParser
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  std::vector <std::string>  LexemList;

                  ArgcArgvParser ();
   virtual        ~ArgcArgvParser () {}

   void           parse_line (const char *line_0, bool includes_command_flag, const char *delimiter_list_0 = " \n\t\b\v");

   const LexemList &
                  use_lexem_list () const;
   void           get_argc_argv (int &argc, const char * const * &argv) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           is_delimiter (char c, const char *delimiter_list_0);

   typedef  std::vector <const char *> Lexem0List;

   bool           _parsed_flag;
   LexemList      _lexem_list;
   Lexem0List     _lexem_0_list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ArgcArgvParser (const ArgcArgvParser &other);
   ArgcArgvParser &
                  operator = (const ArgcArgvParser &other);
   bool           operator == (const ArgcArgvParser &other);
   bool           operator != (const ArgcArgvParser &other);

}; // class ArgcArgvParser



}  // namespace util
}  // namespace ohm



//#include  "ohm/util/ArgcArgvParser.hpp"



#endif   // ohm_util_ArgcArgvParser_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
