/*****************************************************************************

        WidgetMenuItem.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69920

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



#if ! defined (ohm_opa_WidgetMenuItem_HEADER_INCLUDED)
#define  ohm_opa_WidgetMenuItem_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetMenuNodeBase.h"
#include "ohm/opa/frame/FrameMenuItem.h"
#include "ohm/opa/prim/PrimMenuItem.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{


class WidgetMenuItem;
typedef mem::SharedPtr <WidgetMenuItem>   WidgetMenuItemSPtr;

class WidgetMenuItem
:  public WidgetMenuNodeBase
,  public WidgetCreationHelper <WidgetMenuItem>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~WidgetMenuItem ();
   
   void           set_label (const std::string & label);
   const std::string &
                  get_label () const;
   
   void           set_section ();
   
   void           mark (bool mark_flag = true, bool multi_flag = false);
   bool           is_marked () const;
   
   // from FrameMenuItem
   void           req_entered ();
   void           req_exited ();
   void           req_select (const opal::Event & event);
   bool           can_select ();

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetMenuItem ();
   
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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetMenuItem);

   opal::AutoFramePtr <FrameMenuItem>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimMenuItem>
                  _prim_aptr;

   std::string    _label;
   opal::Scripture
                  _scripture;
   float          _width;
   
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetMenuItem (const WidgetMenuItem &other);
   WidgetMenuItem &  operator = (const WidgetMenuItem &other);
   bool           operator == (const WidgetMenuItem &other);
   bool           operator != (const WidgetMenuItem &other);

}; // class WidgetMenuItem



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetMenuItem.hpp"



#endif   // ohm_opa_WidgetMenuItem_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
