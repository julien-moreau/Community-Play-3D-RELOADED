/*****************************************************************************

        GraphicContext.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67384

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



#if ! defined (ohm_opak_GraphicContext_HEADER_INCLUDED)
#define  ohm_opak_GraphicContext_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opa/Quad.h"
#include "ohm/opa/AffineTransform.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/TypePoint.h"
#include "ohm/opak/private/GraphicState.h"
#include "ohm/opak/private/AspectStack.h"
#include "ohm/opak/private/MarkStack.h"

#include <vector>



namespace ohm
{
namespace opal
{
class GraphicContextInterface;
}



namespace opak
{



class GraphicContext
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  GraphicContext (opal::GraphicContextInterface & context);
   virtual        ~GraphicContext ();
   
   opal::GraphicContextInterface &
                  get ();
   
   void           apply (opa::Point & pt, bool round_flag = false) const;
   void           apply (opa::Size & sz, bool round_flag = false) const;
   void           apply (opa::Rect & rect, bool round_flag = false) const;
   void           apply (opa::Quad & quad, bool round_flag = false) const;
   
   void           apply_inverse (opa::Point & pt, bool round_flag = false) const;
   void           apply_inverse (opa::Size & sz, bool round_flag = false) const;
   void           apply_inverse (opa::Rect & rect, bool round_flag = false) const;
   void           apply_inverse (opa::Quad & quad, bool round_flag = false) const;
   
   opa::Point     get_inv_px ();
   
   template <class PrimitiveType>
   void           convert_to_mark (opa::Point & pt, archi::UInt32 mark_id = 0) const;
   template <class PrimitiveType>
   void           convert_to_mark (opa::Size & sz, archi::UInt32 mark_id = 0) const;

   template <class PrimitiveType>
   void           convert_from_mark (opa::Point & pt, archi::UInt32 mark_id = 0) const;
   template <class PrimitiveType>
   void           convert_from_mark (opa::Size & sz, archi::UInt32 mark_id = 0) const;
   
   const opa::AffineTransform &
                  use_transform ();
   
   template <class AspectType>
   const AspectType &
                  get () const;
   template <class AspectType>
   const AspectType *
                  get_ptr () const;
   template <class AspectType>
   AspectType &   use () const;
   
   void           save ();
   void           restore ();
   
   void           translate (float tx, float ty);
   void           translate (float & old_tx, float & old_ty, float tx, float ty);
   void           override_translate (float tx, float ty);
   void           scale (float sx, float sy);
   void           rotate (float rot);

   template <class AspectType>
   void           set (AspectType & aspect);
   
   template <class PrimitiveType>
   void           mark (archi::UInt32 mark_id);
   void           mark (const std::type_info * info_ptr, archi::UInt32 mark_id);
   
   size_t         get_stack_size () const;
   
   void           copy_stack (GraphicContext & other);
   void           reset_stack ();
   void           reset_to_identity ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::vector <GraphicState>  GraphicStateStack;
   
   opal::GraphicContextInterface &
                  _context;
   
   GraphicStateStack
                  _graphic_state_stack;
   AspectStack    _aspect_stack;
   MarkStack      _mark_stack;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  GraphicContext ();
                  GraphicContext (const GraphicContext &other);
   GraphicContext &
                  operator = (const GraphicContext &other);
   bool           operator == (const GraphicContext &other) const;
   bool           operator != (const GraphicContext &other) const;

}; // class GraphicContext



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/GraphicContext.hpp"



#endif   // ohm_opak_GraphicContext_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
