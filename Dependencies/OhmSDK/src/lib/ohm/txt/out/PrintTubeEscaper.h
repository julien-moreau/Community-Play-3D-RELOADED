/*****************************************************************************

        PrintTubeEscaper.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55091

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



#if ! defined (ohm_txt_out_PrintTubeEscaper_HEADER_INCLUDED)
#define  ohm_txt_out_PrintTubeEscaper_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/txt/out/PrintInterface.h"

#include <string>

#include <climits>



namespace ohm
{
namespace txt
{
namespace out
{



class PrintTubeEscaper
:  public PrintInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef void (*EscapeFnc) (PrintTubeEscaper & printer, char c);

   static void    escaper_xml (PrintTubeEscaper & printer, char c);
   
   static std::string
                  deescaper_xml (const std::string & str);

                  PrintTubeEscaper (PrintInterface & dst, EscapeFnc escape_fnc);
   virtual        ~PrintTubeEscaper ();
   
   PrintTubeEscaper &
                  esc_print (const char *format_0);
   PrintTubeEscaper &
                  esc_cat (const char *format_0);

   // DevInterface
   virtual void   print_begin ();
   virtual void   print_char (char c);
   virtual void   print_end ();
   
   // Escapers *ONLY* API
   void           print_dst (const char * str_0);
   void           print_dst (char c);
   
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // PrintInterface
   virtual void   do_reset ();
   virtual bool   do_is_truncated () const;
   virtual long   do_get_string_len () const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   PrintInterface &
                  _dst;
   const EscapeFnc
                  _fnc;
   bool           _escape_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  PrintTubeEscaper ();
                  PrintTubeEscaper (const PrintTubeEscaper &other);
   PrintTubeEscaper &      operator = (const PrintTubeEscaper &other);
   bool           operator == (const PrintTubeEscaper &other);
   bool           operator != (const PrintTubeEscaper &other);

}; // class PrintTubeEscaper



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/PrintTubeEscaper.hpp"



#endif   // ohm_txt_out_PrintTubeEscaper_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
