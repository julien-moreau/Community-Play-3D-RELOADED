/*****************************************************************************

        WidgetMenuRoot.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53326

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



#if ! defined (ohm_opa_WidgetMenuRoot_HEADER_INCLUDED)
#define  ohm_opa_WidgetMenuRoot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetMenuNodeBase.h"
#include "ohm/opa/base/WidgetEmbedderBase.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opa/prim/PrimMenuRoot.h"
#include "ohm/opal/Frame.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/LayerClip.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{


class WidgetMenuRoot;

typedef mem::SharedPtr <WidgetMenuRoot>   WidgetMenuRootSPtr;

class WidgetMenuRoot
:  public WidgetMenuNodeBase
,  public WidgetEmbedderBase
,  public WidgetCreationHelper <WidgetMenuRoot>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class SafeLink
   {
   public:
                  SafeLink ();
      virtual     ~SafeLink ();
      
      void        bind (WidgetMenuRoot & widget);
      
   private:
      friend class WidgetMenuRoot;
   
      WidgetMenuRoot *
                  _widget_ptr;
   };
   
   virtual        ~WidgetMenuRoot ();
   
   void           fade_out ();
   
   // from FrameMenuRoot
   void           req_close (const opal::Event & event);
   
   // from WidgetMenuItem
   bool           can_select ();

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           handle_event_default (Event & event);

   // WidgetBase
   virtual void   invalidate_layout ();
   virtual void   update_layout ();
   
   // internal
   void           link (SafeLink & safe_link);
   void           unlink (SafeLink & safe_link);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetMenuRoot ();
   
   // WidgetBase
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetMenuRoot);
   
   void           process_signal_left (opal::FrameSignal::Event & event);
   void           process_signal_right (opal::FrameSignal::Event & event);
   
   void           set_col (int col_x);
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _frame_content_aptr;

   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_scroll_left_aptr;
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_scroll_right_aptr;

   opak::AutoPrimPtr <PrimMenuRoot>
                  _prim_back_aptr;
   opak::AutoPrimPtr <opak::LayerClip>
                  _layer_clip_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _layer_content_aptr;

   util::Delegate _delegate;

   const double   _creation_time;
   SafeLink *     _safe_link_ptr;
   
   int            _nbr_total_col;
   int            _nbr_visible_col;
   int            _col_x;
   float          _col_width;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetMenuRoot (const WidgetMenuRoot &other);
   WidgetMenuRoot &  operator = (const WidgetMenuRoot &other);
   bool           operator == (const WidgetMenuRoot &other);
   bool           operator != (const WidgetMenuRoot &other);

}; // class WidgetMenuRoot



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetMenuRoot.hpp"



#endif   // ohm_opa_WidgetMenuRoot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
