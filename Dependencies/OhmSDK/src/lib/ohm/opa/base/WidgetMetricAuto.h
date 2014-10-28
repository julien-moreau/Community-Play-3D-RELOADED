/*****************************************************************************

        WidgetMetricAuto.h
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



#if ! defined (ohm_opa_WidgetMetricAuto_HEADER_INCLUDED)
#define  ohm_opa_WidgetMetricAuto_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TransitionFunction.h"


namespace ohm
{
namespace opa
{



class WidgetBase;

class WidgetMetricAuto
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  WidgetMetricAuto (WidgetBase & widget);
   virtual        ~WidgetMetricAuto ();
   
   void           set (float val);
   void           set_auto ();
   void           set_ext ();
   float          get () const;
   bool           is_auto () const;
   bool           is_ext () const;
   
   void           invalidate ();
   bool           need_update () const;
   void           update (float val);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Type
   {
                  Type_FIXED = 0,
                  Type_AUTO,
                  Type_EXT,
   };
   
   WidgetBase &   _widget;

   Type           _type;
   float          _val;
   
   bool           _need_update_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetMetricAuto ();
                  WidgetMetricAuto (const WidgetMetricAuto &other);
   WidgetMetricAuto &   operator = (const WidgetMetricAuto &other);
   bool           operator == (const WidgetMetricAuto &other);
   bool           operator != (const WidgetMetricAuto &other);

}; // class WidgetMetricAuto



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/opa/base/WidgetMetricAuto.hpp"



#endif   // ohm_opa_WidgetMetricAuto_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
