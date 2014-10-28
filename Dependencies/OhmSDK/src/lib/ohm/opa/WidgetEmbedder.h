/*****************************************************************************

        WidgetEmbedder.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58576

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



#if ! defined (ohm_opa_WidgetEmbedder_HEADER_INCLUDED)
#define  ohm_opa_WidgetEmbedder_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetEmbedderBase.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/Frame.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/AutoPrimPtr.h"



namespace ohm
{
namespace opa
{



class WidgetEmbedder;
typedef mem::SharedPtr <WidgetEmbedder>   WidgetEmbedderSPtr;

class WidgetEmbedder
:  public virtual WidgetBase
,  public WidgetEmbedderBase
,  public WidgetCreationHelper <WidgetEmbedder>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~WidgetEmbedder ();

   void           set_width (float width);
   float          get_width () const;

   void           set_height (float height);
   float          get_height () const;
   
   template <class T>
   mem::SharedPtr <T>
                  set ();
   template <class T>
   void           set (mem::SharedPtr <T> widget_sptr);
   void           unset ();
   template <class T>
   T &            use ();
   template <class T>
   T *            use_ptr ();
   bool           empty () const;
   
   // WidgetBase
   virtual bool   layout (const Size & bounds);
   virtual void   spread_view (const Rect & rect);

   // WidgetEmbedderBase
   virtual void   update_layout ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetEmbedder);
   
                  WidgetEmbedder ();

   // WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;
   
   float          _width;
   float          _height;
   
   opa::WidgetSPtr
                  _widget_sptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetEmbedder (const WidgetEmbedder &other);
   WidgetEmbedder &  operator = (const WidgetEmbedder &other);
   bool           operator == (const WidgetEmbedder &other);
   bool           operator != (const WidgetEmbedder &other);

}; // class WidgetEmbedder



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetEmbedder.hpp"



#endif   // ohm_opa_WidgetEmbedder_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
