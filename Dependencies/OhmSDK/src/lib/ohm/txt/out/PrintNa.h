/*****************************************************************************

        PrintNa.h
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



#if ! defined (ohm_txt_out_PrintNa_HEADER_INCLUDED)
#define  ohm_txt_out_PrintNa_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/txt/out/PrintInterface.h"

#include <climits>



namespace ohm
{
namespace txt
{
namespace out
{



class PrintNa
:  public PrintInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PrintNa (char *output_0, long max_len = LONG_MAX - 1);
   virtual        ~PrintNa ();

   // DevInterface
   virtual void   print_begin ();
   virtual void   print_char (char c);
   virtual void   print_end ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // PrintInterface
   virtual void   do_reset ();
   virtual bool   do_is_truncated () const;
   virtual long   do_get_string_len () const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   char *         _output_0;           // Output string
   long           _max_len;            // Maximum length of the output string
   long           _cur_write_pos;      // Writing position in output string



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  PrintNa ();
                  PrintNa (const PrintNa &other);
   PrintNa &      operator = (const PrintNa &other);
   bool           operator == (const PrintNa &other);
   bool           operator != (const PrintNa &other);

}; // class PrintNa



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/PrintNa.hpp"



#endif   // ohm_txt_out_PrintNa_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
