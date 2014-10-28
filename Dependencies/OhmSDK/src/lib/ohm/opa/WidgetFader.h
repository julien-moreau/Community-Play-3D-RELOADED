/*****************************************************************************

        WidgetFader.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 61304

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



#if ! defined (ohm_opa_WidgetFader_HEADER_INCLUDED)
#define  ohm_opa_WidgetFader_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/SharedPtr.h"
#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/frame/FrameFader.h"
#include "ohm/opa/prim/PrimFader.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class WidgetFader;
typedef mem::SharedPtr <WidgetFader>   WidgetFaderSPtr;

class WidgetFader
:  public WidgetBase
,  public WidgetCreationHelper <WidgetFader>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Event
   {
   public:
      
      enum Type 
      {
                  Type_BEGIN = 0,
                  Type_GESTURE,
                  Type_COMMIT,
                  Type_CANCEL,
                  Type_RESET
      };
                  Event (WidgetFader & widget, Type type) : _widget (widget), _type (type) {}


      WidgetFader &
                  _widget;
      Type        _type;
      float       _begin_value;
      float       _previous_value;
      float       _suggested_value;
   };
   
   enum Type
   {
                  Type_HORIZONTAL = 0,
                  Type_VERTICAL
   };
   
   virtual        ~WidgetFader ();

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           set_type (Type type);

   void           set_length (float length);
   void           set_length_auto_parent (float weight = 1.f);
   float          get_length ();
   bool           is_length_auto () const;
   
   void           set_range (float val_min, float val_max);
   const std::pair <float, float> &
                  get_range () const;
   
   void           set_discrete (bool discrete_flag);
   bool           is_discrete () const;
   
   void           set_value (float value);
   float          get_value () const;
   
   void           set_norm_value (float norm_value);
   float          get_norm_value () const;
   
   bool           is_in_gesture ();

   // internal 
   void           req_set_value (float value, Event::Type type);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  WidgetFader ();

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

   ohm_opa_WidgetCreationHelper_INVITE (WidgetFader);
   
   void           update_metrics ();


   
   opal::AutoFramePtr <FrameFader>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimFader>
                  _prim_aptr;
   
   util::Delegate _delegate;
   Type           _type;
   WidgetMetricAuto
                  _length;
   
   std::pair <float, float>
                  _range;
   bool           _discrete_flag;
   
   bool           _editing_flag;
   float          _begin_value;
   
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetFader (const WidgetFader &other);
   WidgetFader &  operator = (const WidgetFader &other);
   bool           operator == (const WidgetFader &other) const;
   bool           operator != (const WidgetFader &other) const;

}; // class WidgetFader




}  // namespace opa
}  // namespace ohm



#include "ohm/opa/WidgetFader.hpp"



#endif   // ohm_opa_WidgetFader_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

