/*****************************************************************************

        FontPaths.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59820

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/freetype/FontPaths.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/def.h"
#include "ohm/archi/def.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/sys/FileRef.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FontPaths::~FontPaths ()
{
}



/*
==============================================================================
Name : get
==============================================================================
*/

const std::string &  FontPaths::get (const std::string & name) const
{
   NamePathMap::const_iterator it = _name_path_map.find (name);
   assert (it != _name_path_map.end ());
   
   return it->second;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FontPaths::FontPaths ()
{
   // for now let's do it manually

#if 0

   sys::FileRefSPtr  font_ref_sptr = sys::FileRefFactory::get_fonts ();
   assert (font_ref_sptr.get () != 0);
   
   std::string base_path = font_ref_sptr->to_utf8_path ();
   assert (base_path.size () > 0);

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   if (base_path [base_path.size () - 1] != '/')
   {
      base_path += '/';
   }
   
   _name_path_map ["Arial"] = base_path + "Arial.ttf";
   _name_path_map ["Arial Bold"] = base_path + "Arial Bold.ttf";
   _name_path_map ["Arial Narrow"] = base_path + "Arial Narrow.ttf";
   _name_path_map ["Times"] = base_path + "Times New Roman.ttf";
   _name_path_map ["Zapfino"] = base_path + "Zapfino.dfont";

#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)

   if (base_path [base_path.size () - 1] != '\\')
   {
      base_path += '\\';
   }
   
   _name_path_map ["Arial"] = base_path + "ARIAL.TTF";
   _name_path_map ["Arial Bold"] = base_path + "ARIALBD.TTF";
   _name_path_map ["Arial Narrow"] = base_path + "ARIAL.TTF";
   _name_path_map ["Times"] = base_path + "TIMES.TTF";
   
#else
   #error Unsupported operating system.
#endif

#endif   // if 0
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
