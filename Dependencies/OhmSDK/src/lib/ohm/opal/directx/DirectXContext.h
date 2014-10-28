/*****************************************************************************

        DirectXContext.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59386

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



#if ! defined (ohm_opal_DirectXContext_HEADER_INCLUDED)
#define  ohm_opal_DirectXContext_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/GraphicContextBase.h"



namespace ohm
{
namespace opal
{



class DirectXEngine;

class DirectXContext
:  public GraphicContextBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DirectXContext (DirectXEngine & engine);
   virtual        ~DirectXContext ();
   
   void           set_context_size (const opa::Size & size);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual Engine &
                  do_use_engine ();

   virtual void   do_update_context (const GraphicState & graphic_state);
   virtual const opa::Size &
                  do_get_context_size ();
   
   virtual void   do_fill (const opa::Path & path, const Color & color);
   virtual void   do_fill (const opa::Rect & rect, const Color & color);
   virtual void   do_fill (const opa::Quad & vertices, const Color & color);

   virtual void   do_stroke (const opa::Path & path, const Color & color);
   
   virtual void   do_draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha);
   virtual void   do_draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   DirectXEngine &
                  _engine;

   opa::Size      _context_size;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  DirectXContext ();
                  DirectXContext (const DirectXContext &other);
   DirectXContext &operator = (const DirectXContext &other);
   bool           operator == (const DirectXContext &other);
   bool           operator != (const DirectXContext &other);

}; // class DirectXContext



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_DirectXContext_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
