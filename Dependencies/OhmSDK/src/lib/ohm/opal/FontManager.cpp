/*****************************************************************************

        FontManager.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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

#include "ohm/opal/FontManager.h"
#include "ohm/opa/Point.h"



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

FontManager::~FontManager ()
{
// release_all ();

   assert (_ui_font_map.empty ());
   assert (_vt_font_map.empty ());
}



/*
==============================================================================
Name : register_user_input_thread
==============================================================================
*/

void  FontManager::register_user_input_thread ()
{
   _user_input_thread_id = sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : register_video_thread
==============================================================================
*/

void  FontManager::register_video_thread ()
{
   _video_thread_id = sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  FontManager::release (Engine & engine)
{
   {
      FontMap & font_map = _vt_font_map;
      
      FontMap::iterator it = font_map.begin ();
      FontMap::iterator it_end = font_map.end ();
      
      for (; it != it_end ; ++it)
      {
         Font * font_ptr = it->second;
         
         font_ptr->release (engine);
      }
   }
}



/*
==============================================================================
Name : release_all
==============================================================================
*/

void  FontManager::release_all ()
{
   {
      FontMap & font_map = _ui_font_map;
      
      FontMap::iterator it = font_map.begin ();
      FontMap::iterator it_end = font_map.end ();
      
      for (; it != it_end ; ++it)
      {
         Font * font_ptr = it->second;
         
         delete font_ptr;
      }
      
      font_map.clear ();
   }
   
   {
      FontMap & font_map = _vt_font_map;
      
      FontMap::iterator it = font_map.begin ();
      FontMap::iterator it_end = font_map.end ();
      
      for (; it != it_end ; ++it)
      {
         Font * font_ptr = it->second;
         
         delete font_ptr;
      }
      
      font_map.clear ();
   }
}



/*
==============================================================================
Name : get_font
Description :
   Returns a font using its name
   'size' is the size in pixel (not point) of the font.
   On Macintosh, the font sub-system is defined with a resolution of 72 ppi
   thus making a pixel equal a point.
   On Windows, the font sub-system is defined with a resolution of 96 ppi
   thus additionnal scaling must be performed.
   When a character does not have a corresponding glyph, it is substitued
   by a '?' char by default. This character can be changed later with
   the set_unmapped_char method.
   The direction is left to right by default. This setting can be changed
   with the
==============================================================================
*/

const Font &   FontManager::get_font (const std::string & name, float size)
{
   // we need to find a name that please every system
   // macos x prefers the postcript name, but can fallback
   // on other names like the full name in the truetype way
   // normally OpenType will follow the postscript name for
   // the full name
   
   sys::ThreadMgr::IdType cur_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   FontMap * font_map_ptr = 0;
   
   if (cur_thread_id == _user_input_thread_id)
   {
      font_map_ptr = &_ui_font_map;
   }
   else if (cur_thread_id == _video_thread_id)
   {
      font_map_ptr = &_vt_font_map;
   }
   else
   {
      assert (_user_input_thread_id != sys::ThreadMgr::get_invalid_thread_id ());
      assert (_video_thread_id != sys::ThreadMgr::get_invalid_thread_id ());
      assert (_user_input_thread_id != _video_thread_id);
   
      assert (false);
   }
   
   assert (font_map_ptr != 0);
   FontMap & font_map = *font_map_ptr;
   
   FontMap::iterator it = font_map.find (std::make_pair (name, size));
   
   if (it == font_map.end ())
   {
      Font * font_ptr = new Font (name, size);
      
      font_map [std::make_pair (name, size)] = font_ptr;
      
      return *font_ptr;
   }
   else
   {
      const Font * font_ptr = it->second;
      
      assert (font_ptr != 0);
      
      return *font_ptr;
   }
}



/*
==============================================================================
Name : get_font_for_thread
Description :
   Returns the font associated to this thread.
   If the font does not exists, create it by extracting needed info
==============================================================================
*/

const Font &   FontManager::get_font_for_thread (const Font & font)
{
   if (font.get_thread_id () == sys::ThreadMgr::get_current_thread_id ())
   {
      return font;
   }
   else
   {
      return get_font (font.get_name (), font.get_size ());
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

FontManager::FontManager ()
:  _user_input_thread_id (sys::ThreadMgr::get_invalid_thread_id ())
,  _video_thread_id (sys::ThreadMgr::get_invalid_thread_id ())
,  _ui_font_map ()
,  _vt_font_map ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
