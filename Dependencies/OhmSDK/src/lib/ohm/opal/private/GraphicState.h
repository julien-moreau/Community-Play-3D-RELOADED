/*****************************************************************************

        GraphicState.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 30668

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



#if ! defined (ohm_opal_GraphicState_HEADER_INCLUDED)
#define  ohm_opal_GraphicState_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/private/BlendMode.h"
#include "ohm/opal/private/FragmentProgramProc.h"



namespace ohm
{
namespace opal
{



class GraphicState
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum {         MAX_FRAGMENT_VARS = 4 };

                  GraphicState ();
   virtual        ~GraphicState ();

   float          _alpha;
   Color          _stroke_color;
   Color          _fill_color;
   Color          _clear_color;
   BlendMode      _blend_mode;
   
   opa::Rect      _local_clip_rect;
   opa::Rect      _global_clip_rect;
   opa::Point     _total_translation;
   
   FragmentProgramProc
                  _fragment_program_proc;
   float          _fragment_var_arr [MAX_FRAGMENT_VARS];
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const GraphicState &other);
   bool           operator != (const GraphicState &other);

}; // class GraphicState



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/GraphicState.hpp"



#endif   // ohm_opal_GraphicState_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
