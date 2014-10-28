/*****************************************************************************

        FontTableGlyphSubstitution.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61875

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

#include "ohm/opal/freetype/FontTableGlyphSubstitution.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/private/Typesetter.h"
#include "ohm/util/trace.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H

#include <cassert>

#undef FONT_TABLE_GSUB_VERBOSE

#undef OHM_OPAL_BREAK_ON_UNIMPLEMENTED

#if defined (OHM_OPAL_BREAK_ON_UNIMPLEMENTED)
   #define break_unimplemented   assert (false)

#else // defined (OHM_OPAL_BREAK_ON_UNIMPLEMENTED)
   #define break_unimplemented   (void (0))

#endif // defined (OHM_OPAL_BREAK_ON_UNIMPLEMENTED)



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

FontTableGlyphSubstitution::FontTableGlyphSubstitution (FT_Face face)
{
   assert (face != 0);

   // http://partners.adobe.com/public/developer/opentype/index_table_formats1.html
   // http://www.microsoft.com/typography/otspec/
   
   bool ok_flag = _font_table.open (face, TTAG_GSUB);
   
   if (ok_flag)
   {
      archi::UInt32 version = _font_table.read_uint32 ();
      debugf ("version = %08x\n", version);
      
      _script_list_offset = _font_table.read_uint16 ();
      debugf ("script_list_offset = %04x\n", _script_list_offset);

      _feature_list_offset = _font_table.read_uint16 ();
      debugf ("feature_list_offset = %04x\n", _feature_list_offset);

      _lookup_list_offset = _font_table.read_uint16 ();
      debugf ("lookup_list_offset = %04x\n", _lookup_list_offset);
      
      // first read loopup list
      read_lookup_list_table ();

      read_script_list_table ();
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FontTableGlyphSubstitution::~FontTableGlyphSubstitution ()
{
   try
   {
      _look_up_sptr_arr.clear ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : process_substitution
==============================================================================
*/

void  FontTableGlyphSubstitution::process_substitution (Typesetter & typesetter, GlyphRange & glyph_range)
{
   // http://www.microsoft.com/typography/otspec/chapter2.htm
   // http://www.microsoft.com/typography/otspec/gsub.htm
   // http://www.microsoft.com/typography/otspec/features_fj.htm
   
   
   assert (glyph_range._start < glyph_range._end);
   
   bool direction = typesetter.get_glyph (glyph_range._start)._direction;
         
   for (size_t i = 0 ; i < _look_up_sptr_arr.size () ; ++i)
   {
      LookUp & look_up = *_look_up_sptr_arr [i];
      assert (&look_up != 0);
      
      archi::UInt32 feature_tag = look_up._feature_tag;
      
      bool feature_flag
         = (feature_tag == 'init')
         || (feature_tag == 'medi')
         || (feature_tag == 'fina')
         || (feature_tag == 'isol')
         || (feature_tag == 'liga')
      ;
      
      if (!feature_flag)
      {
         // for now, we do not support other features
         
         continue;
      }
      
      for (size_t j = 0 ; j < look_up._st_look_up_sptr_arr.size () ; ++j)
      {
         if (look_up._st_look_up_sptr_arr [j].get () != 0)
         {
            STLookUp & st_look_up = *look_up._st_look_up_sptr_arr [j];
            
            size_t pos;
            int inc = 0;
            
            if (direction)
            {
               pos = glyph_range._end - 1;
               inc = -1;
            }
            else
            {
               pos = glyph_range._start;
               inc = 1;
            }
            
            for (
               ;
               (pos >= glyph_range._start) && (pos < glyph_range._end) ;
               pos += inc
               )
            {
               Glyph & glyph = typesetter.get_glyph (pos);
               
               if (glyph.has_feature (feature_tag))
               {
                  archi::UInt16 coverage_index = 0;

                  bool found_flag = st_look_up._coverage_aptr->process (coverage_index, glyph._gid);
                  
                  if (found_flag)
                  {
                     st_look_up.process (typesetter, glyph_range, coverage_index, pos);
                  }
               }
            }
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : read_script_list_table
==============================================================================
*/

void  FontTableGlyphSubstitution::read_script_list_table ()
{
   _font_table.seek (_script_list_offset);
   
   archi::UInt16 script_count = _font_table.read_uint16 ();
   debugf ("script_count = %04x\n", script_count);
   
   for (archi::UInt16 i = 0 ; i < script_count ; ++i)
   {
      archi::UInt32 script_tag = _font_table.read_uint32 ();
      debugf (
         "script_tag = %c%c%c%c\n",
         script_tag >> 24, script_tag >> 16, script_tag >> 8, script_tag
      );
      
      archi::UInt16 script_offset = _font_table.read_uint16 ();
      
      _font_table.save ();
      
      _font_table.seek (_script_list_offset + script_offset);
      
      // maybe null
      archi::UInt16 default_lang_sys_offset = _font_table.read_uint16 ();
      debugf ("default_lang_sys_offset = %04x\n", default_lang_sys_offset);
      
      // just ignore lang sys variant we are just going to consider default lang sys
      
      _font_table.save ();
      _font_table.seek (_script_list_offset + script_offset + default_lang_sys_offset);
      
      // null
      archi::UInt16 look_up_order_offset = _font_table.read_uint16 ();
      assert (look_up_order_offset == 0);
      
      archi::UInt16 req_feature_index = _font_table.read_uint16 ();
      debugf ("req_feature_index = %04x\n", req_feature_index);
      
      archi::UInt16 feature_count = _font_table.read_uint16 ();
      debugf ("feature_count = %04x\n", feature_count);
      
      for (archi::UInt16 j = 0 ; j < feature_count ; ++j)
      {
         // zero based
         archi::UInt16 feature_index = _font_table.read_uint16 ();
         
         read_feature_list_table (feature_index);
      }
      
      _font_table.restore ();
      _font_table.restore ();
   }
}



/*
==============================================================================
Name : read_feature_list_table
==============================================================================
*/

void  FontTableGlyphSubstitution::read_feature_list_table (archi::UInt16 feature_index)
{
   _font_table.save ();

   _font_table.seek (_feature_list_offset);
   
   archi::UInt16 feature_count = _font_table.read_uint16 ();
   assert (feature_index < feature_count);
   
   _font_table.seek_relative ((4 + 2) * feature_index);
   
   archi::UInt32 feature_tag = _font_table.read_uint32 ();
   debugf (
      "feature_tag = %c%c%c%c\n",
      feature_tag >> 24, feature_tag >> 16, feature_tag >> 8, feature_tag
   );
   
   archi::UInt16 feature_offset = _font_table.read_uint16 ();
   debugf ("feature_offset = %04x\n", feature_offset);
   
   _font_table.seek (_feature_list_offset + feature_offset);
   
   // null
   archi::UInt16 feature_params = _font_table.read_uint16 ();
   assert (feature_params == 0);
   
   archi::UInt16 look_up_count = _font_table.read_uint16 ();
   debugf ("look_up_count = %04x\n", look_up_count);
   
   for (archi::UInt16 i = 0 ; i < look_up_count ; ++i)
   { 
      archi::UInt16 look_up_list_index = _font_table.read_uint16 ();
      debugf ("look_up_list_index = %04x\n", look_up_list_index);
      
      assert (look_up_list_index < _look_up_sptr_arr.size ());
      
      _look_up_sptr_arr [look_up_list_index]->_feature_tag = feature_tag;
   }
   
   
   _font_table.restore ();
}



/*
==============================================================================
Name : read_lookup_list_table
==============================================================================
*/

void  FontTableGlyphSubstitution::read_lookup_list_table ()
{
   _font_table.save ();

   _font_table.seek (_lookup_list_offset);
   
   archi::UInt16 look_up_count = _font_table.read_uint16 ();
   
   _look_up_sptr_arr.resize (look_up_count);
   
   for (archi::UInt16 i = 0 ; i < look_up_count ; ++i)
   {
      LookUp * look_up_ptr = new LookUp;
      
      _look_up_sptr_arr [i] = LookUpSPtr (look_up_ptr);
      
      archi::UInt16 look_up_offset = _font_table.read_uint16 ();
      _font_table.save ();
      _font_table.seek (_lookup_list_offset + look_up_offset);
      
      archi::UInt16 look_up_type = _font_table.read_uint16 ();
      archi::UInt16 look_up_flag = _font_table.read_uint16 ();
      archi::UInt16 sub_table_count = _font_table.read_uint16 ();
      
      look_up_ptr->_st_look_up_sptr_arr.resize (sub_table_count);
         
      for (archi::UInt16 j = 0 ; j < sub_table_count ; ++j)
      {
         archi::UInt16 sub_table_offset = _font_table.read_uint16 ();
         
         _font_table.save ();
         _font_table.seek (_lookup_list_offset + look_up_offset + sub_table_offset);
         
         STLookUp * sub_table_look_up_ptr = 0;
         
         switch (look_up_type)
         {
         case 1:
            sub_table_look_up_ptr = read_single ();
            break;

         case 2:
            sub_table_look_up_ptr = read_multiple ();
            break;

         case 3:
            sub_table_look_up_ptr = read_alternate ();
            break;

         case 4:
            sub_table_look_up_ptr = read_ligature ();
            break;

         case 5:
            sub_table_look_up_ptr = read_context ();
            break;

         case 6:
            sub_table_look_up_ptr = read_chaining_context ();
            break;

         case 7:
            sub_table_look_up_ptr = read_extension_subs ();
            break;

         case 8:
            sub_table_look_up_ptr = read_rev_chaining_context_single ();
            break;
         
         default:
            assert (false);
            break;
         }
         
         look_up_ptr->_st_look_up_sptr_arr [j] = STLookUpSPtr (sub_table_look_up_ptr);
         
         _font_table.restore ();
      }

      _font_table.read_uint16 ();   // mark filtering set

      _font_table.restore ();
   }
   
   _font_table.restore ();
}



/*
==============================================================================
Name : read_coverage
==============================================================================
*/

FontTableGlyphSubstitution::Coverage * FontTableGlyphSubstitution::read_coverage (archi::UInt16 offset)
{
   _font_table.save ();
   _font_table.seek (offset);
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   if (format == 1)
   {
      CoverageFormat1 * coverage_ptr = new CoverageFormat1;
      
      std::map <archi::UInt16, archi::UInt16> & gid_coverage_index_map
         = coverage_ptr->_gid_coverage_index_map;
      
      archi::UInt16 glyph_count = _font_table.read_uint16 ();
      
      // to optimize
      
      for (archi::UInt16 i = 0 ; i < glyph_count ; ++i)
      {
         archi::UInt16 gid = _font_table.read_uint16 ();
         
         gid_coverage_index_map.insert (
            gid_coverage_index_map.end (),
            std::make_pair (gid, i)
         );
      }
      
      _font_table.restore ();

      return coverage_ptr;
   }
   else if (format == 2)
   {
      CoverageFormat2 * coverage_ptr = new CoverageFormat2;
      
      std::map <archi::UInt16, CoverageFormat2::Range> & first_gid_range_map
         = coverage_ptr->_first_gid_range_map;
      
      archi::UInt16 range_count = _font_table.read_uint16 ();
      
      for (archi::UInt16 i = 0 ; i < range_count ; ++i)
      {
         archi::UInt16 first_gid = _font_table.read_uint16 ();
         archi::UInt16 last_gid = _font_table.read_uint16 ();
         archi::UInt16 start_coverage_index = _font_table.read_uint16 ();
         
         CoverageFormat2::Range range;
         range._last_gid = last_gid;
         range._start_coverage_index = start_coverage_index;
         first_gid_range_map.insert (first_gid_range_map.end (), std::make_pair (
            first_gid, range
         ));
      }
      
      _font_table.restore ();

      return coverage_ptr;
   }
   
   _font_table.restore ();

   break_unimplemented;
   return 0;
}
   


/*
==============================================================================
Name : read_single
Reference:
http://partners.adobe.com/public/developer/opentype/index_table_formats1.html#SS
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_single ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
   if (format == 1)
   {
      STLookUpSingleFormat1 * look_up_ptr = new STLookUpSingleFormat1;
      
      look_up_ptr->_coverage_aptr = std::auto_ptr <Coverage> (
         read_coverage (archi::UInt16 (base_offset) + coverage_offset)
      );
      
      archi::UInt16 delta_glyph_id = _font_table.read_uint16 ();
      
      look_up_ptr->_delta_gid = delta_glyph_id;
      
      return look_up_ptr;
   }
   else if (format == 2)
   {
      STLookUpSingleFormat2 * look_up_ptr = new STLookUpSingleFormat2;
      
      look_up_ptr->_coverage_aptr = std::auto_ptr <Coverage> (
         read_coverage (archi::UInt16 (base_offset) + coverage_offset)
      );
      
      archi::UInt16 glyph_count = _font_table.read_uint16 ();
      look_up_ptr->_gid_arr.resize (glyph_count);
      
      for (archi::UInt16 i = 0 ; i < glyph_count ; ++i)
      {
         archi::UInt16 gid = _font_table.read_uint16 ();
         
         look_up_ptr->_gid_arr [i] = gid;
      }
      
      return look_up_ptr;
   }
   
   ohm_util_TRACE_WARNING1 ("Single Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_multiple
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_multiple ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
// ohm_util_TRACE_WARNING1 ("Multiple Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_alternate
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_alternate ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
   ohm_util_TRACE_WARNING1 ("Alternate Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_ligature
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_ligature ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
   if (format == 1)
   {
      STLookUpLigatureFormat1 * look_up_ptr = new STLookUpLigatureFormat1;
      
      look_up_ptr->_coverage_aptr = std::auto_ptr <Coverage> (
         read_coverage (archi::UInt16 (base_offset) + coverage_offset)
      );
      
      archi::UInt16 lig_set_count = _font_table.read_uint16 ();
      look_up_ptr->_ligature_set.resize (lig_set_count);
      
      for (archi::UInt16 i = 0 ; i < lig_set_count ; ++i)
      {
         std::vector <STLookUpLigatureFormat1::Ligature> & ligature_arr
            = (look_up_ptr->_ligature_set) [i];
         
         archi::UInt16 ligature_set_offset = _font_table.read_uint16 ();
         
         _font_table.save ();
         _font_table.seek (base_offset + ligature_set_offset);
         
         archi::UInt16 ligature_count = _font_table.read_uint16 ();
         
         ligature_arr.resize (ligature_count);
         
         for (archi::UInt16 j = 0 ; j < ligature_count ; ++j)
         {
            STLookUpLigatureFormat1::Ligature & ligature = ligature_arr [j];
            
            archi::UInt16 ligature_offset = _font_table.read_uint16 ();
            
            _font_table.save ();
            _font_table.seek (base_offset + ligature_set_offset + ligature_offset);
            
            archi::UInt16 lig_glyph = _font_table.read_uint16 ();
            ligature._gid = lig_glyph;
            
            archi::UInt16 comp_count = _font_table.read_uint16 ();
            
            ligature._gid_arr.resize (comp_count - 1);
            
            for (archi::UInt16 k = 0 ; k < comp_count - 1 ; ++k)
            {
               archi::UInt16 component = _font_table.read_uint16 ();
               
               ligature._gid_arr [k] = component;
            }
            
            _font_table.restore ();
         }
         
         _font_table.restore ();
      }
      
      return look_up_ptr;
   }
   
   ohm_util_TRACE_WARNING1 ("Ligature Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_context
Reference :
http://partners.adobe.com/public/developer/opentype/index_table_formats1.html#CS
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_context ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
   // ArialUnicodeMS use format 2
   
// ohm_util_TRACE_WARNING1 ("Context Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_chaining_context
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_chaining_context ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
// ohm_util_TRACE_WARNING1 ("Chaining Contextual Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_extension_subs
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_extension_subs ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
// ohm_util_TRACE_WARNING1 ("Extension Substitution Format %1% not implemented", format);
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : read_rev_chaining_context_single
==============================================================================
*/

FontTableGlyphSubstitution::STLookUp * FontTableGlyphSubstitution::read_rev_chaining_context_single ()
{
   long base_offset = _font_table.tell ();
   
   archi::UInt16 format = _font_table.read_uint16 ();
   
   archi::UInt16 coverage_offset = _font_table.read_uint16 ();
   
   ohm_util_TRACE_WARNING1 (
      "Reverse Chaining Contextual Single Substitution Format %1% not implemented", format
   );
   
   break_unimplemented;
   return 0;
}



/*
==============================================================================
Name : debugf
==============================================================================
*/

void  FontTableGlyphSubstitution::debugf (const char * format_0, ...)
{
#if defined (FONT_TABLE_GSUB_VERBOSE)
   va_list ap;
   va_start (ap, format_0);
   vprintf (format_0, ap);
   va_end (ap);
#endif
}



/*
==============================================================================
Name : CoverageFormat1::process
==============================================================================
*/

bool  FontTableGlyphSubstitution::CoverageFormat1::process (archi::UInt16 & coverage_index, archi::UInt32 gid)
{
   std::map <archi::UInt16, archi::UInt16>::iterator it
      = _gid_coverage_index_map.find (gid);
   
   bool found_flag = it != _gid_coverage_index_map.end ();
   
   if (found_flag)
   {
      coverage_index = it->second;
   }
   
   return found_flag;
}



/*
==============================================================================
Name : CoverageFormat2::process
==============================================================================
*/

bool  FontTableGlyphSubstitution::CoverageFormat2::process (archi::UInt16 & coverage_index, archi::UInt32 gid)
{
   std::map <archi::UInt16, Range>::iterator it
      = _first_gid_range_map.upper_bound (gid);
      
   if (it != _first_gid_range_map.begin ())
   {
      --it;
      
      bool found_flag = (it->first <= gid) && (gid <= it->second._last_gid);
      
      if (found_flag)
      {
         coverage_index = it->second._start_coverage_index + gid - it->first;
      }
      
      return found_flag;
   }
   else
   {
      coverage_index = 0;
      
      return false;
   }
}



/*
==============================================================================
Name : STLookUpSingleFormat1::process
==============================================================================
*/

void  FontTableGlyphSubstitution::STLookUpSingleFormat1::process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos)
{
   Glyph & glyph = typesetter.get_glyph (pos);
   
   glyph._gid += _delta_gid;
}



/*
==============================================================================
Name : STLookUpSingleFormat2::process
==============================================================================
*/

void  FontTableGlyphSubstitution::STLookUpSingleFormat2::process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos)
{
   Glyph & glyph = typesetter.get_glyph (pos);
   
   assert (coverage_index < _gid_arr.size ());
   
   glyph._gid = _gid_arr [coverage_index];
}



/*
==============================================================================
Name : STLookUpLigatureFormat1::process
==============================================================================
*/

void  FontTableGlyphSubstitution::STLookUpLigatureFormat1::process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos)
{
   const std::vector <Ligature> & ligature_arr = _ligature_set [coverage_index];
   
   bool direction = typesetter.get_glyph (pos)._direction;
   
   for (size_t i = 0 ; i < ligature_arr.size () ; ++i)
   {
      const Ligature & ligature = ligature_arr [i];
      
      const std::vector <archi::UInt16> & gid_arr = ligature._gid_arr;
      
      bool in_range_flag = false;
      size_t np_pos = 0;
      int inc;
      
      if (direction)
      {
         in_range_flag = pos >= glyph_range._start + gid_arr.size ();
         np_pos = pos - 1;
         inc = -1;
      }
      else
      {
         in_range_flag = pos + gid_arr.size () < glyph_range._end;
         np_pos = pos + 1;
         inc = 1;
      }
      
      if (in_range_flag)
      {
         bool found_flag = true;
         
         for (size_t j = 0 ; j < gid_arr.size () ; ++j, np_pos += inc)
         {
            archi::UInt16 comp_gid = gid_arr [j];
            archi::UInt16 gid = typesetter.get_glyph (np_pos)._gid;
            
            if (comp_gid != gid)
            {
               found_flag = false;
               break;
            }
         }
         
         if (found_flag)
         {
            Glyph & glyph = typesetter.get_glyph (pos);
            glyph._gid = ligature._gid;
            
            size_t d_pos;
            
            if (direction)
            {
               d_pos = pos - 1;
            }
            else
            {
               d_pos = pos + 1;
            }
            
            for (size_t j = 0 ; j < gid_arr.size () ; ++j)
            {
               typesetter.erase_glyph (d_pos, glyph_range);
               
               if (direction)
               {
                  d_pos -= 1;
               }
            }
         }
      }
   }
}



/*
==============================================================================
Name : LookUp::LookUp
==============================================================================
*/

FontTableGlyphSubstitution::LookUp::~LookUp ()
{
   try
   {
      _st_look_up_sptr_arr.clear ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
