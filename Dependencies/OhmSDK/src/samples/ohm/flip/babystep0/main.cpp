/*****************************************************************************

        main.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70401

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
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "BabystepGui.h"

#include "ohm/opa/ApplicationWindowSimple.h"
#include "ohm/util/ArgcArgvParser.h"
#include "ohm/util/trace.h"
#include "ohm/util/boot.hpp"

#include <cassert>




/*\\\ CLASS DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
int WINAPI  WinMain (::HINSTANCE instance, ::HINSTANCE prev_instance, ::LPSTR cmd_line_0, int cmd_show)
{
   int argc = 0;
   const char * const * argv = 0;
   ohm::util::Boot boot (instance, cmd_line_0, argc, argv);
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
int main (int argc, char* argv [])
{

#endif // ohm_archi_SYS
   
   try
   {
      using namespace ohm;
      
      opa::ApplicationWindowSimple window (argc, argv);
      BabystepGui gui (window);
      
      window.run ();
   }
   catch (...)
   {
      assert (false);
   }

   return 0;
}



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
