/*****************************************************************************

        ArgBase.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/out/ArgBase.h"
#include "ohm/txt/out/ArgNumBase.h"
#include "ohm/txt/out/DevInterface.h"
#include "ohm/txt/out/FmtInfo.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace out
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ArgBase::ArgBase (DevInterface &output, const FmtInfo &fmt)
:  _output (output)
,  _fmt (fmt)
,  _displayable_flag (true)
{
   // Nothing
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



DevInterface & ArgBase::use_output ()
{
   return (_output);
}



const FmtInfo &   ArgBase::use_fmt () const
{
   return (_fmt);
}



void  ArgBase::set_displayable (bool flag)
{
   _displayable_flag = flag;
}



bool  ArgBase::is_displayable () const
{
   return (_displayable_flag);
}



/*
==============================================================================
Name: print_not_displayable_value
Description:
   Print filling character for values which can't be displayed.
Throws: Nothing
==============================================================================
*/

void  ArgBase::print_not_displayable_value ()
{
   if (   _fmt.has_max_len ()
       && _fmt.get_justify_mode () != FmtInfo::JustifyMode_LEFT)
   {
      const long     len = _fmt.get_max_len ();
      for (long pos = 0; pos < len; ++pos)
      {
         _output.print_char ('#');
      }
   }
   else if (_fmt.get_max_len () > 0)
   {
      _output.print_char ('#');
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
