/*****************************************************************************

        WidgetMatrixBool.h
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



#if ! defined (ohm_opa_WidgetMatrixBool_HEADER_INCLUDED)
#define  ohm_opa_WidgetMatrixBool_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMatrixBoolCell.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/Frame.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/LayerTransform.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opa
{



class WidgetMatrixBool;
typedef mem::SharedPtr <WidgetMatrixBool> WidgetMatrixBoolSPtr;

class WidgetMatrixBool
:  public WidgetBase
,  public WidgetCreationHelper <WidgetMatrixBool>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (WidgetMatrixBool & widget) : _widget (widget) {}
                  
      WidgetMatrixBool &
                  _widget;
      size_t      _x;
      size_t      _y;
      bool        _cur_flag;
      bool        _suggested_flag;
   };
   
   virtual        ~WidgetMatrixBool ();
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           handle_event_default (Event & event);

   void           resize (size_t width, size_t height);
   size_t         width () const;
   size_t         height () const;
   
   void           push_back_row (const std::string & label);
   void           push_back_column (const std::string & label);
   
   void           set_row_label (size_t y, const std::string & label);
   void           set_column_label (size_t x, const std::string & label);
   
   void           set_val (size_t x, size_t y, bool val);
   bool           get_val (size_t x, size_t y) const;

   // WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetMatrixBool ();
   
   // delegates
   void           handle_cell_event (WidgetMatrixBoolCell::Event & event);

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetMatrixBool);
   
   enum Slot
   {
                  Slot_NORMAL = 0,
                  Slot_ROTATED,
                  
                  Slot_NBR_ELT
   };
   
   class CellPoint
   {
   public:
      size_t      _x;
      size_t      _y;
   };
   
   void           update ();
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;
   opak::AutoPrimPtr <opak::LayerTransform>
                  _layer_col_label_aptr;
   
   size_t         _width;
   size_t         _height;
   WidgetSPtrArr  _widget_sptr_arr;
   
   bool           _metrics_need_update_flag;
   float          _max_row_label_width;
   float          _max_col_label_height;
   float          _total_width;
   float          _total_height;

   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetMatrixBool (const WidgetMatrixBool &other);
   WidgetMatrixBool &   operator = (const WidgetMatrixBool &other);
   bool           operator == (const WidgetMatrixBool &other);
   bool           operator != (const WidgetMatrixBool &other);

}; // class WidgetMatrixBool



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetMatrixBool.hpp"



#endif   // ohm_opa_WidgetMatrixBool_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
