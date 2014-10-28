/*****************************************************************************

        FontTableGlyphSubstitution.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61876

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



#if ! defined (ohm_opal_FontTableGlyphSubstitution_HEADER_INCLUDED)
#define  ohm_opal_FontTableGlyphSubstitution_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/freetype/FontTable.h"
#include "ohm/opal/private/Glyph.h"
#include "ohm/mem/SharedPtr.h"

#include <list>
#include <vector>
#include <set>
#include <map>
#include <memory>



namespace ohm
{
namespace opal
{



class Font;
class Typesetter;

class FontTableGlyphSubstitution
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FontTableGlyphSubstitution (FT_Face face);
   virtual        ~FontTableGlyphSubstitution ();
   
   void           process_substitution (Typesetter & typesetter, GlyphRange & glyph_range);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class Coverage
   {
   public:
      virtual     ~Coverage () {}
      virtual bool
                  process (archi::UInt16 & coverage_index, archi::UInt32 gid) = 0;
   };
   
   class CoverageFormat1
   :  public Coverage
   {
   public:
      virtual     ~CoverageFormat1 () {}
      virtual bool
                  process (archi::UInt16 & coverage_index, archi::UInt32 gid);

      std::map <archi::UInt16, archi::UInt16>
                  _gid_coverage_index_map;
   };

   class CoverageFormat2
   :  public Coverage
   {
   public:
      virtual     ~CoverageFormat2 () {}
      virtual bool
                  process (archi::UInt16 & coverage_index, archi::UInt32 gid);
                  
      class Range
      {
      public:
         archi::UInt16
                  _last_gid;
         archi::UInt16
                  _start_coverage_index;
      };          
      
      std::map <archi::UInt16, Range>
                  _first_gid_range_map;
   };
   
   class STLookUp
   {
   public:
      virtual     ~STLookUp () {}
      
      virtual void
                  process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos) = 0;

      std::auto_ptr <Coverage>
                  _coverage_aptr;
   };
   
   typedef mem::SharedPtr <STLookUp>   STLookUpSPtr;

   
   class STLookUpNotImplemented
   :  public STLookUp
   {
   public:
      virtual     ~STLookUpNotImplemented () {}
      
      virtual void
                  process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos) {}
   };
   

   class STLookUpSingleFormat1
   :  public STLookUp
   {
   public:
      virtual     ~STLookUpSingleFormat1 () {}
      
      virtual void
                  process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos);

      archi::UInt16
                  _delta_gid;
   };
   
   class STLookUpSingleFormat2
   :  public STLookUp
   {
   public:
      virtual     ~STLookUpSingleFormat2 () {}
      
      virtual void
                  process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos);

      std::vector <archi::UInt16>
                  _gid_arr;
   };
   
   class STLookUpLigatureFormat1
   :  public STLookUp
   {
   public:
      virtual     ~STLookUpLigatureFormat1 () {}
      
      virtual void
                  process (Typesetter & typesetter, GlyphRange & glyph_range, archi::UInt16 coverage_index, size_t pos);
      
      class Ligature
      {
      public:
         archi::UInt16
                  _gid;
         std::vector <archi::UInt16>
                  _gid_arr;   // except first
      };
      
      std::vector <std::vector <Ligature> >
                  _ligature_set;
   };
   
   class LookUp
   {
   public:
      virtual     ~LookUp ();
      
      archi::UInt32
                  _feature_tag;
      
      std::vector <STLookUpSPtr>
                  _st_look_up_sptr_arr;
   };
   
   typedef mem::SharedPtr <LookUp>  LookUpSPtr;
   
   void           read_script_list_table ();
   void           read_feature_list_table (archi::UInt16 feature_index);
   
   void           read_lookup_list_table ();
   
   Coverage *     read_coverage (archi::UInt16 offset);
   
   STLookUp *     read_single ();
   STLookUp *     read_multiple ();
   STLookUp *     read_alternate ();
   STLookUp *     read_ligature ();
   STLookUp *     read_context ();
   STLookUp *     read_chaining_context ();
   STLookUp *     read_extension_subs ();
   STLookUp *     read_rev_chaining_context_single ();
   
   void           debugf (const char * format_0, ...);

   //Font &       _font;
   FontTable      _font_table;
   
   archi::UInt16  _script_list_offset;
   archi::UInt16  _feature_list_offset;
   archi::UInt16  _lookup_list_offset;
   
   std::vector <LookUpSPtr>
                  _look_up_sptr_arr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FontTableGlyphSubstitution ();
                  FontTableGlyphSubstitution (const FontTableGlyphSubstitution &other);
   FontTableGlyphSubstitution &operator = (const FontTableGlyphSubstitution &other);
   bool           operator == (const FontTableGlyphSubstitution &other);
   bool           operator != (const FontTableGlyphSubstitution &other);

}; // class FontTableGlyphSubstitution



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_FontTableGlyphSubstitution_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
