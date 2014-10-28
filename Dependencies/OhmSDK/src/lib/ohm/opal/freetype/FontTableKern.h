/*****************************************************************************

        FontTableKern.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 29121

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



#if ! defined (ohm_opal_FontTableKern_HEADER_INCLUDED)
#define  ohm_opal_FontTableKern_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/freetype/FontTable.h"
#include "ohm/opal/private/Glyph.h"

#include <map>
#include <vector>



namespace ohm
{
namespace opal
{



class Scripture;
class TypescriptMeasure;

class FontTableKern
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FontTableKern (FT_Face face, archi::UInt32 units_per_em, float size);
   virtual        ~FontTableKern ();
   
   void           measure (TypescriptMeasure & measure, const std::vector <Glyph> & glyph_arr, size_t start, size_t end);
   void           process_position (TypescriptMeasure & measure, std::vector <Glyph> & glyph_arr, size_t start, size_t end);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <archi::UInt32, float>   KernMap;

   void           load_kerning_v0 ();
   void           load_kerning_v1 ();
   void           debugf (const char * format_0, ...);
   
   FontTable      _font_table;
   archi::UInt32  _units_per_em;
   float          _size;

   KernMap        _kern_map;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FontTableKern ();
                  FontTableKern (const FontTableKern &other);
   FontTableKern &operator = (const FontTableKern &other);
   bool           operator == (const FontTableKern &other);
   bool           operator != (const FontTableKern &other);

}; // class FontTableKern



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_FontTableKern_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
