/*****************************************************************************

        FontTableKern.cpp
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

#include "ohm/opal/freetype/FontTableKern.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/private/Glyph.h"
#include "ohm/opal/private/TypescriptMeasure.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H

#include <cassert>

#undef FONT_TABLE_KERN_VERBOSE



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

FontTableKern::FontTableKern (FT_Face face, archi::UInt32 units_per_em, float size)
:  _font_table ()
,  _units_per_em (units_per_em)
,  _size (size)

,  _kern_map ()
{
   assert (face != 0);
   
   bool ok_flag = _font_table.open (face, TTAG_kern);
   
   if (ok_flag)
   {
      archi::UInt16 version = _font_table.read_uint16 ();
      debugf ("version = %08x\n", version);
      
      if (version == 0)
      {
         load_kerning_v0 ();
      }
      else
      {
         load_kerning_v1 ();
      }
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FontTableKern::~FontTableKern ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : measure
==============================================================================
*/

void  FontTableKern::measure (TypescriptMeasure & measure, const std::vector <Glyph> & glyph_arr, size_t start, size_t end)
{
   archi::UInt32 prev_gid = 0;
   
   for (size_t i = start ; i < end ; ++i)
   {
      const Glyph & glyph = glyph_arr [i];
      
      archi::UInt32 gid = glyph._gid;
      
      if ((prev_gid != 0) && (gid != 0) && (prev_gid <= 0xFFFFUL) && (gid <= 0xFFFFUL))
      {
         archi::UInt32 gid_pair = (prev_gid << 16) | gid;
         
         KernMap::iterator it = _kern_map.find (gid_pair);
         
         if (it != _kern_map.end ())
         {
            float kern = it->second;
            
            measure._width += kern;
         }
      }
      
      prev_gid = gid;
   }
}



/*
==============================================================================
Name : process_position
==============================================================================
*/

void  FontTableKern::process_position (TypescriptMeasure & measure, std::vector <Glyph> & glyph_arr, size_t start, size_t end)
{
   float relative_pos = 0.f;  // cumulative
   
   archi::UInt32 prev_gid = 0;
   
   for (size_t i = start ; i < end ; ++i)
   {
      Glyph & glyph = glyph_arr [i];
      
      archi::UInt32 gid = glyph._gid;
      
      if ((prev_gid != 0) && (gid != 0) && (prev_gid <= 0xFFFFUL) && (gid <= 0xFFFFUL))
      {
         archi::UInt32 gid_pair = (prev_gid << 16) | gid;
         
         KernMap::iterator it = _kern_map.find (gid_pair);
         
         if (it != _kern_map.end ())
         {
            float kern = it->second;
            
            relative_pos += kern;
            
            glyph._x += relative_pos;
            glyph._width += kern;
            
            measure._width += kern;
            measure._image_right += kern;
         }
      }
      
      prev_gid = gid;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_kerning_v0
==============================================================================
*/

void  FontTableKern::load_kerning_v0 ()
{
   archi::UInt16 nbr_tables = _font_table.read_uint16 ();
   
   debugf ("nbr tables = %d\n", nbr_tables);
   
   for (size_t i = 0 ; i < nbr_tables ; ++i)
   {
      debugf ("subtable #%d\n", i);
      
      archi::UInt16 version = _font_table.read_uint16 ();
      debugf ("  version = %d\n", version);

      archi::UInt16 length = _font_table.read_uint16 ();
      debugf ("  length = %d\n", length);
      
      archi::UInt16 coverage = _font_table.read_uint16 ();
      debugf ("  coverage = %04x\n", coverage);

      /*bool horizontal_flag = (coverage & (1 << 0)) != 0;
      bool minimum_flag = (coverage & (1 << 1)) != 0;
      bool cross_stream_flag = (coverage & (1 << 2)) != 0;
      bool override_flag = (coverage & (1 << 3)) != 0;*/
      
      archi::UByte format = coverage >> 8;
      debugf ("  format = %d\n", format);
      
      if (format == 0)
      {
         archi::UInt16 nbr_pairs = _font_table.read_uint16 ();
         debugf ("  nbr_pairs = %d\n", nbr_pairs);
         
         _font_table.read_uint16 ();   // search range
         _font_table.read_uint16 ();   // entry selector
         _font_table.read_uint16 ();   // range shift
         
         for (size_t j = 0 ; j < nbr_pairs ; ++j)
         {
            archi::UInt16 left_gid = _font_table.read_uint16 ();
            debugf ("  left_gid = U+%04x\n", left_gid);

            archi::UInt16 right_gid = _font_table.read_uint16 ();
            debugf ("  right_gid = U+%04x\n", right_gid);

            archi::Int16 value = _font_table.read_uint16 ();
            debugf ("  value = %d\n", value);
            
            archi::UInt32 gid_pair;
            gid_pair = (left_gid << 16) | right_gid;
            
            float kern = (float (value) / float (_units_per_em)) * _size;
            
            _kern_map [gid_pair] = kern;
         }
      }
      else if (format == 2)
      {
      }
   }
}



/*
==============================================================================
Name : load_kerning_v1
==============================================================================
*/

void  FontTableKern::load_kerning_v1 ()
{
   // http://developer.apple.com/textfonts/TTRefMan/RM06/Chap6kern.html
   
   _font_table.read_uint16 ();   // skip remainder of version
   
   archi::UInt32 nbr_tables = _font_table.read_uint32 ();
   
   debugf ("nbr tables = %d\n", nbr_tables);
   
   for (size_t i = 0 ; i < nbr_tables ; ++i)
   {
      debugf ("subtable #%d\n", i);
      
      archi::UInt32 length = _font_table.read_uint32 ();
      debugf ("  length = %d\n", length);
      
      archi::UInt16 coverage = _font_table.read_uint16 ();
      debugf ("  coverage = %04x\n", coverage);

      archi::UInt16 tuple_index = _font_table.read_uint16 ();
      debugf ("  tuple_index = %04x\n", tuple_index);

      /*bool horizontal_flag = (coverage & (1 << 0)) != 0;
      bool minimum_flag = (coverage & (1 << 1)) != 0;
      bool cross_stream_flag = (coverage & (1 << 2)) != 0;
      bool override_flag = (coverage & (1 << 3)) != 0;*/
      
      archi::UByte format = coverage >> 8;
      debugf ("  format = %d\n", format);
      
      if (format == 0)
      {
         archi::UInt16 nbr_pairs = _font_table.read_uint16 ();
         debugf ("  nbr_pairs = %d\n", nbr_pairs);
         
         _font_table.read_uint16 ();   // search range
         _font_table.read_uint16 ();   // entry selector
         _font_table.read_uint16 ();   // range shift
         
         for (size_t j = 0 ; j < nbr_pairs ; ++j)
         {
            archi::UInt16 left_gid = _font_table.read_uint16 ();
            debugf ("  left_gid = U+%04x\n", left_gid);

            archi::UInt16 right_gid = _font_table.read_uint16 ();
            debugf ("  right_gid = U+%04x\n", right_gid);

            archi::Int16 value = _font_table.read_uint16 ();
            debugf ("  value = %d\n", value);
            
            archi::UInt32 gid_pair;
            gid_pair = (left_gid << 16) | right_gid;
            
            float kern = (float (value) / float (_units_per_em)) * _size;
            
            _kern_map [gid_pair] = kern;
         }
      }
      else
      {
         assert (false);
      }
   }
}



/*
==============================================================================
Name : debugf
==============================================================================
*/

void  FontTableKern::debugf (const char * format_0, ...)
{
#if defined (FONT_TABLE_KERN_VERBOSE)
   va_list ap;
   va_start (ap, format_0);
   vprintf (format_0, ap);
   va_end (ap);
#endif
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
