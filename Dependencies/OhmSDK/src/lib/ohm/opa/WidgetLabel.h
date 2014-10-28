/*****************************************************************************

        WidgetLabel.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60933

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



#if ! defined (ohm_opa_WidgetLabel_HEADER_INCLUDED)
#define  ohm_opa_WidgetLabel_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/prim/PrimLabel.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opak/AutoPrimPtr.h"



namespace ohm
{
namespace opal
{
class Typesetter;
}



namespace opa
{


class WidgetLabel;
typedef mem::SharedPtr <WidgetLabel>   WidgetLabelSPtr;

class WidgetLabel
:  public WidgetBase
,  public WidgetCreationHelper <WidgetLabel>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~WidgetLabel ();
   
   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();
   bool           is_width_auto () const;
   
   void           set_label (const std::string & label);
   const std::string &
                  get_label () const;

   void           set_flush_factor (float flush_factor);
   float          get_flush_factor () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetLabel ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetLabel);
   
   void           update_metrics ();
   
   opak::AutoPrimPtr <PrimLabel>
                  _prim_aptr;
   float          _ascent;
   float          _descent;

   WidgetMetricAuto
                  _width;
   std::string    _label;
   float          _flush_factor;
   opal::Scripture
                  _scripture;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetLabel (const WidgetLabel &other);
   WidgetLabel &  operator = (const WidgetLabel &other);
   bool           operator == (const WidgetLabel &other);
   bool           operator != (const WidgetLabel &other);

}; // class WidgetLabel



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/opa/WidgetLabel.hpp"



#endif   // ohm_opa_WidgetLabel_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
