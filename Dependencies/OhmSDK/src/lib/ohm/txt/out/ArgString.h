/*****************************************************************************

        ArgString.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 20570

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



#if ! defined (ohm_txt_out_ArgString_HEADER_INCLUDED)
#define  ohm_txt_out_ArgString_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/out/ArgBase.h"



namespace ohm
{
namespace txt
{
namespace out
{



class FmtInfo;

class ArgString
:  public ArgBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  const char *   StringType;

                  ArgString (DevInterface &output, const FmtInfo &fmt);
   virtual        ~ArgString () {}

   void           print (const StringType &val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           evaluate_format_and_size (const StringType txt_0);
   void           format_filler ();
   void           print_value ();
   void           print_string ();
   void           print_string_upper ();
   void           print_string_lower ();

   const char *   _txt_0;
   long           _size;
   long           _display_offset;
   long           _displayed_size;
   long           _pre_fill_size;
   long           _post_fill_size;
   char           _pre_fill_char;
   char           _post_fill_char;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ArgString ();
                  ArgString (const ArgString &other);
   ArgString &    operator = (const ArgString &other);
   bool           operator == (const ArgString &other);
   bool           operator != (const ArgString &other);

}; // class ArgString



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/ArgString.hpp"



#endif   // ohm_txt_out_ArgString_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
