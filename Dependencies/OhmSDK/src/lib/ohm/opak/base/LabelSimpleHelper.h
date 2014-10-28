/*****************************************************************************

        LabelSimpleHelper.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66928

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



#if ! defined (ohm_opak_LabelSimpleHelper_HEADER_INCLUDED)
#define  ohm_opak_LabelSimpleHelper_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Scripture.h"
#include "ohm/opal/private/Typesetter.h"



namespace ohm
{
namespace opal
{
class Font;
}



namespace opak
{



class GraphicContext;

class LabelSimpleHelper
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  LabelSimpleHelper (const opal::Font & font, bool enable_ellipsis_flag = true);
   virtual        ~LabelSimpleHelper ();
   
   const opal::Font &
                  get_font () const;

   void           set (const std::string & str);
   
   float          get_full_width ();
   
   void           set_available_width (float width);
   float          get_cur_width ();
   
   void           display (opak::GraphicContext & context, const opa::Point & pt, const opal::Color & color, float scale = 1.f);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           display (opak::GraphicContext & context, const opal::Font::Glyphs & glyphs, archi::UInt32 glyph_gid, const opa::Point & screen_pt, const opa::Point & line_pt, const opa::Size & vx, const opa::Size & vy, const opal::Color & color, float scale = 1.f);
   
   void           update ();
   
   const opal::Font &
                  _font;
   const bool     _enable_ellipsis_flag;
   
   std::string    _str;
   opal::Scripture
                  _scripture;
   
   float          _available_width;
   float          _cur_width;
   bool           _display_ellipsis_flag;
   std::auto_ptr <opal::Typesetter>
                  _typesetter_aptr;
   const opal::Typesetter::LineInfo *
                  _line_info_ptr;
   bool           _need_update_flag;

   float          _ellipsis_width;
   archi::UInt32  _ellipsis_gid;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  LabelSimpleHelper ();
                  LabelSimpleHelper (const LabelSimpleHelper &other);
   LabelSimpleHelper &  operator = (const LabelSimpleHelper &other);
   bool           operator == (const LabelSimpleHelper &other);
   bool           operator != (const LabelSimpleHelper &other);

}; // class LabelSimpleHelper



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/base/LabelSimpleHelper.hpp"



#endif   // ohm_opak_LabelSimpleHelper_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
