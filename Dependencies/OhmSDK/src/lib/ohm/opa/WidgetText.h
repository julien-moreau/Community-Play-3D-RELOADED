/*****************************************************************************

        WidgetText.h
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 69169

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



#if ! defined (ohm_opa_WidgetText_HEADER_INCLUDED)
#define  ohm_opa_WidgetText_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimText.h"

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"



namespace ohm
{
namespace opa
{



class WidgetText;
typedef mem::SharedPtr <WidgetText> WidgetTextSPtr;

class WidgetText
:  public opa::WidgetBase
,  public opa::WidgetCreationHelper <WidgetText>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~WidgetText ();

   void           set_width (float width);
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_text (const std::string & text);
                  
   // opa::WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const opa::Size & bounds);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  WidgetText ();

   // WidgetBase
   virtual void   do_get_min_bbox (opa::Rect & min_bbox);
   virtual void   do_get_margin (opa::Margin & margin);
   
   virtual void   do_set_origin (const opa::Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const opa::Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm_opa_WidgetCreationHelper_INVITE (WidgetText);
   
   void           update_metrics (float width);
   

   opak::AutoPrimPtr <PrimText>
                  _prim_aptr;

   opa::WidgetMetricAuto
                  _width;
   float          _height;
   bool           _invalid_flag;
   opa::Size      _last_size;
   
   std::string    _text;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetText (const WidgetText &other);
   WidgetText &   operator = (const WidgetText &other);
   bool           operator == (const WidgetText &other) const;
   bool           operator != (const WidgetText &other) const;

}; // class WidgetText



}  // namespace opa
}  // namespace ohm



//#include "ohm/opa/WidgetText.hpp"



#endif   // ohm_opa_WidgetText_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

