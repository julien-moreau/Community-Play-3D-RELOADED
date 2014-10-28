/*****************************************************************************

        FontManager.h
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



#if ! defined (ohm_opal_FontManager_HEADER_INCLUDED)
#define  ohm_opal_FontManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Font.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/txt/unicode/Char.h"
#include "ohm/sys/ThreadMgr.h"

#include "ohm/archi/def.h"
#include "ohm/opa/Constants.h"
#include "ohm/opal/def.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/util/SingletonBase.h"

#include <string>
#include <map>



namespace ohm
{
namespace opal
{



class Color;
class Point;

class FontManager
:  public util::SingletonBase <FontManager>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~FontManager ();

   void           register_user_input_thread ();
   void           register_video_thread ();
   
   void           release (Engine & engine);
   void           release_all ();
   
   const Font &   get_font (const std::string & name, float size);
   const Font &   get_font_for_thread (const Font & font);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  FontManager ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (FontManager);

   typedef std::map <std::pair <std::string, float>, Font *> FontMap;
   
   sys::ThreadMgr::IdType
                  _user_input_thread_id;
   sys::ThreadMgr::IdType
                  _video_thread_id;
   
   FontMap        _ui_font_map;
   FontMap        _vt_font_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FontManager (const FontManager &other);
   FontManager &  operator = (const FontManager &other);
   bool           operator == (const FontManager &other);
   bool           operator != (const FontManager &other);

}; // class FontManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_FontManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
