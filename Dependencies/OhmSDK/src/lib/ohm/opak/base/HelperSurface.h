/*****************************************************************************

        HelperSurface.h
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 66883

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



#if ! defined (ohm_opak_HelperSurface_HEADER_INCLUDED)
#define  ohm_opak_HelperSurface_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"

#include <utility>



namespace ohm
{
namespace opal
{
   class Color;
}


namespace opak
{



class GraphicContext;

// DON'T INHERIT FROM THIS CLASS

class HelperSurface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  HelperSurface ();
                  ~HelperSurface () {} // NOT VIRTUAL

                  
   HelperSurface &
                  display_begin (opak::GraphicContext & context);
   HelperSurface &
                  color (const opal::Color & color);
   HelperSurface &   
                  display (const opa::Point & pt_1, const opa::Point & pt_2);

   void           display_end ();




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         RING_LEN = 2      };

   typedef std::pair <opa::Point, opa::Point> PointPair;
   
   static void    display (opak::GraphicContext & context, const PointPair & p1, const PointPair & p2);
                  
                  
                  
   opak::GraphicContext *
                  _context_ptr;
   PointPair      _ring [RING_LEN];
   size_t         _ring_pos;
   size_t         _counter;
   
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HelperSurface (const HelperSurface &other);
   HelperSurface &   operator = (const HelperSurface &other);
   bool           operator == (const HelperSurface &other) const;
   bool           operator != (const HelperSurface &other) const;

}; // class HelperSurface



}  // namespace opak
}  // namespace ohm



//#include "ohm/opak/HelperSurface.hpp"



#endif   // ohm_opak_HelperSurface_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

