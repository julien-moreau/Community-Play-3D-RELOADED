/*****************************************************************************

        Cache.h
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



#if ! defined (ohm_opal_Cache_HEADER_INCLUDED)
#define  ohm_opal_Cache_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/def.h"
#include "ohm/opal/private/GraphicContextBase.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/directx/CacheImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #if (ohm_opal_API == ohm_opal_API_OPENGL)
      #include "ohm/opal/opengl/CacheImpl.h"
   
   #endif
   
#else
   #error Operating System not supported

#endif



namespace ohm
{
namespace opal
{



class GraphicContextInterface;

class Cache
:  public GraphicContextBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Cache (GraphicContextInterface & gc, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode);
   virtual        ~Cache ();
   
                  operator bool ();
   
   void           resize (const opa::Size & size);
   const opa::Size &
                  get_size ();
   
   Image          get_image () const;

   void           invalidate_all ();
   void           invalidate (const opa::Rect & rect);
   void           validate ();
   
   const opa::Rect &
                  get_invalid_rect () const;
   
   void           clear_invalid_rect ();
   


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
   
   Engine &       _engine;
   CacheImpl      _cache_impl;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Cache (const Cache &other);
   Cache &  operator = (const Cache &other);
   bool           operator == (const Cache &other);
   bool           operator != (const Cache &other);

}; // class Cache



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Cache_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
