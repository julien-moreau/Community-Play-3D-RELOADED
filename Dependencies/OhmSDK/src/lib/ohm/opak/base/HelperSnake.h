/*****************************************************************************

        HelperSnake.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64279

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



#if ! defined (ohm_opak_HelperSnake_HEADER_INCLUDED)
#define  ohm_opak_HelperSnake_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Margin.h"
#include "ohm/opa/Rect.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/Image.h"



namespace ohm
{
namespace opak
{



class GraphicContext;

// DON'T INHERIT FROM THIS CLASS

class HelperSnake
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  HelperSnake ();
                  ~HelperSnake () {}   // NOT VIRTUAL
   
   HelperSnake &  head (const opal::Image & image);
   HelperSnake &  medial (const opal::Image & image);
   HelperSnake &  tail (const opal::Image & image);
   HelperSnake &  offset (float y);

   HelperSnake &  display_begin (opak::GraphicContext & context);
   HelperSnake &  point (const opa::Point & pt);
   void           display_end ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           display_head (opak::GraphicContext & context);
   void           display_medial (opak::GraphicContext & context);
   void           display_tail (opak::GraphicContext & context);
   
   void           calc_pt (opa::Point & pt0, opa::Point & pt1, const opa::Point & pt, const opa::Size & vec);
   
   const opa::Point &
                  get_pt (size_t m_pos);
   
   opal::Image    _image_head;
   opal::Image    _image_medial;
   opal::Image    _image_tail;
   float          _offset_y;
   float          _height;
   
   opak::GraphicContext *
                  _context_ptr;
   
   opa::Point     _pt_ring [4];
   size_t         _pt_pos;
   size_t         _cur_pt_nbr;
   
   opa::Point     _pt_0;
   opa::Point     _pt_1;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HelperSnake (const HelperSnake &other);
   HelperSnake &  operator = (const HelperSnake &other);
   bool           operator == (const HelperSnake &other);
   bool           operator != (const HelperSnake &other);

}; // class HelperSnake



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/base/HelperSnake.hpp"



#endif   // ohm_opak_HelperSnake_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
