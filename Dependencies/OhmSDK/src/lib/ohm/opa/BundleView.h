/*****************************************************************************

        BundleView.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71208

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



#if ! defined (ohm_opa_BundleView_HEADER_INCLUDED)
#define  ohm_opa_BundleView_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Bundle.h"
#include "ohm/opa/WidgetMenuRoot.h"
#include "ohm/opa/prim/PrimToolTip.h"
#include "ohm/opa/private/FrameWidgetMenuClose.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opal/HIDEvent.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/LayerStaticAspect.h"
#include "ohm/opak/View.h"
#include "ohm/opak/base/AspectColorAB.h"
#include "ohm/mem/SharedPtr.h"

#include <map>
#include <string>



namespace ohm
{
namespace opal
{
class Window;
class TimeListener;
}



namespace opa
{



class ToolTipSafeLink;

class BundleView
:  public opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Slot
   {
                  Slot_WORKSPACE = 0,
                  Slot_DIALOG,
                  Slot_POPUP_MENU,
                  Slot_OVERLAY,
                  
                  NBR_Slot_TOTAL
   };
   
   enum  MarkId
   {
                  MarkId_ROOT = 'root',
   };

                  BundleView (opal::Window & window);
                  BundleView (opal::View & view);
   virtual        ~BundleView ();
   
   opal::Window & use_window ();
   
   void           run ();
   void           stop ();
   
   void           add (opal::TimeListener & listener);
   void           remove (opal::TimeListener & listener);

   void           add (opal::PostActionListener & listener);
   void           remove (opal::PostActionListener & listener);
   
   template <class LayerType>
   LayerType &    intercalate (const std::string & name);
   template <class LayerType>
   LayerType *    get (const std::string & name);
   void           deintercalate (const std::string & name);

   template <class LayerType>
   LayerType &    intercalate (Slot slot, const std::string & name);
   void           deintercalate (Slot slot, const std::string & name);
   
   const opa::Size &
                  get_bounds () const;
   
   WidgetMenuRoot &
                  create_popup_menu (WidgetMenuRoot::SafeLink & safe_link);
   void           stop_popup_menu ();
   
   void           create_tool_tip (ToolTipSafeLink & safe_link, const opa::Point & pt, const opa::Rect & rect, const std::string & label);
   void           release_tool_tip (ToolTipSafeLink & safe_link);

   void           debug_toggle_show_invalid_rect ();  

   void           set_mouse_wheel_inversion (bool invert_flag);
   
   void           post (const std::vector <opal::HIDEvent> & hid_event_arr);
   void           post_update_cursor ();
   
                                 

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef opak::AutoPrimPtr <opak::PrimitiveContainer> AutoContainerPtr;
   typedef mem::SharedPtr <AutoContainerPtr> AutoContainerPtrSPtr;
   
   typedef std::map <std::string, AutoContainerPtrSPtr> NamePrimMap;
   
   void           init ();
   
   friend class WidgetMenuItem;
   friend class WidgetMenuRoot;
   void           release_popup_menu ();
   
   friend class FrameWidgetMenuClose;
   void           req_release_popup_menu ();

   // delegate
   void           view_layout_changed ();


   opal::Window & _window;
   bool           _opal_view_allocated_flag;
   opal::View *   _opal_view_ptr;
   opak::View *   _opak_view_ptr;
   
   NamePrimMap    _intercalated_layer_map;
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr_arr [NBR_Slot_TOTAL];
   opak::AutoPrimPtr <opak::Layer>
                  _layer_aptr_arr [NBR_Slot_TOTAL];
   opal::AutoFramePtr <FrameWidgetMenuClose>
                  _frame_menu_close_aptr;
                  
   WidgetMenuRootSPtr
                  _menu_root_sptr;

   opak::AutoPrimPtr <PrimToolTip>
                  _prim_tool_tip_aptr;
   void *         _tool_tip_represented_obj_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleView ();
                  BundleView (const BundleView &other);
   BundleView &   operator = (const BundleView &other);
   bool           operator == (const BundleView &other);
   bool           operator != (const BundleView &other);

}; // class BundleView



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opa/BundleView.hpp"



#endif   // ohm_opa_BundleView_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
