/*****************************************************************************

        WidgetBase.h
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



#if ! defined (ohm_opa_WidgetBase_HEADER_INCLUDED)
#define  ohm_opa_WidgetBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Bundle.h"
#include "ohm/opa/Margin.h"
#include "ohm/opa/Rect.h"
#include "ohm/opa/base/WidgetCreationHelper.h"
#include "ohm/opak/base/TransitionFunction.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opa/private/FrameWidgetBaseDebug.h"
#include "ohm/opa/private/PrimWidgetBaseDebug.h"

#include <list>
#include <vector>

#undef OHM_OPA_WIDGET_DEBUG

#if defined (OHM_OPA_WIDGET_DEBUG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_OPA_WIDGET_DEBUG is defined in shipping level code !
   #endif
   
#endif

namespace ohm
{
namespace opa
{



class WidgetProxy;

class WidgetBase
:  public Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  WidgetBase ();
   virtual        ~WidgetBase ();
   
   void           enable (bool enabled_flag = true);
   void           disable ();
   bool           is_enabled () const;
   
   void           show (bool show_flag = true);
   void           hide ();
   bool           is_visible () const;

   void           set_horizontal_weight (float weight);
   virtual float  get_horizontal_weight () const;

   void           set_vertical_weight (float weight);
   virtual float  get_vertical_weight () const;
   
   void           set_extra_margin (const Margin & margin);
   void           get_margin (Margin & margin);
   
   void           set_relative_origin (const Point & rel_origin);
   
   void           get_min_bbox (Rect & min_bbox);
   
   void           set_transition (double tt, opak::TransitionFunction tf);
   void           set_transition_default ();
   void           set_transition_fast ();
   void           set_transition_none ();
   double         get_transition_time () const;
   opak::TransitionFunction
                  get_transition_function () const;

   template <class T>
   void           set_data (const T & data);
   template <class T>
   const T &      get_data ();
   template <class T>
   bool           is_data_of_type () const;
   
   void           set_rect (const Rect & rect, double tt = 0.0, opak::TransitionFunction tf = opak::step);
   void           set_origin (const Point & origin, double tt = 0.0, opak::TransitionFunction tf = opak::step);
   void           set_bounds (const Size & bounds, double tt = 0.0, opak::TransitionFunction tf = opak::step);
   
   const Rect &   get_rect () const;
   const Point &  get_origin () const;
   const Size &   get_bounds () const;

   // internal
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   virtual void   spread_view (const Rect & rect);
   
   void           req_debug_entered ();
   void           req_debug_exited ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual void   do_get_min_bbox (Rect & min_bbox) = 0;
   virtual void   do_get_margin (Margin & margin) = 0;
   
   virtual void   do_set_rect (const Rect & rect, double tt, opak::TransitionFunction tf);
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf) = 0;
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf) = 0;
   
   friend class WidgetProxy;
   
   virtual void   do_enable (bool enabled_flag) = 0;
   virtual void   do_show (bool show_flag) = 0;
   virtual void   do_activate (bool activate_flag) = 0;
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef  void (*DtorFnc) (void * obj_ptr);
   
   void           release_data ();
   
   template <class T>
   static void    method_dtor (void * obj_ptr);
   
   void           init_debug ();
   
   bool           _visible_flag;
   bool           _enabled_flag;
   float          _horizontal_weight;
   float          _vertical_weight;
   Margin         _margin;
   Point          _rel_origin;
   double         _transition_time;
   opak::TransitionFunction
                  _transition_function;
   
   Rect           _rect;
   
   std::vector <archi::UByte>
                  _data;
   const std::type_info *
                  _data_type_info_ptr;
   DtorFnc        _dtor_fnc;
   
#if defined (OHM_OPA_WIDGET_DEBUG)
   opal::AutoFramePtr <FrameWidgetBaseDebug>
                  _frame_debug_aptr;
   opak::AutoPrimPtr <PrimWidgetBaseDebug>
                  _prim_debug_aptr;
   bool           _attached_flag;
#endif   // OHM_OPA_WIDGET_DEBUG
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetBase (const WidgetBase &other);
   WidgetBase &   operator = (const WidgetBase &other);
   bool           operator == (const WidgetBase &other);
   bool           operator != (const WidgetBase &other);

}; // class WidgetBase

typedef mem::SharedPtr <WidgetBase> WidgetSPtr;

typedef std::list <WidgetSPtr>   WidgetSPtrList;
typedef std::vector <WidgetSPtr> WidgetSPtrArr;



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.hpp"



#endif   // ohm_opa_WidgetBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
