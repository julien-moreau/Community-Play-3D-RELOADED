/*****************************************************************************

        WidgetProgressBar.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61111

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



#if ! defined (ohm_opa_WidgetProgressBar_HEADER_INCLUDED)
#define  ohm_opa_WidgetProgressBar_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/SharedPtr.h"
#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/prim/PrimProgressBar.h"
#include "ohm/opak/AutoPrimPtr.h"



namespace ohm
{
namespace opa
{



class WidgetProgressBar;
typedef mem::SharedPtr <WidgetProgressBar>   WidgetProgressBarSPtr;

class WidgetProgressBar
:  public WidgetBase
,  public WidgetCreationHelper <WidgetProgressBar>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~WidgetProgressBar ();

   void           set_length (float length);
   void           set_length_auto_parent (float weight = 1.f);
   float          get_length ();
   bool           is_length_auto () const;
   
   void           set_indeterminate ();
   void           set_norm_value (float norm_value);
   float          get_norm_value () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  WidgetProgressBar ();

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

   ohm_opa_WidgetCreationHelper_INVITE (WidgetProgressBar);
   
   void           update_metrics ();


   opak::AutoPrimPtr <PrimProgressBar>
                  _prim_aptr;
   
   WidgetMetricAuto
                  _length;
   
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetProgressBar (const WidgetProgressBar &other);
   WidgetProgressBar &
                  operator = (const WidgetProgressBar &other);
   bool           operator == (const WidgetProgressBar &other) const;
   bool           operator != (const WidgetProgressBar &other) const;

}; // class WidgetProgressBar




}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/WidgetProgressBar.hpp"



#endif   // ohm_opa_WidgetProgressBar_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

