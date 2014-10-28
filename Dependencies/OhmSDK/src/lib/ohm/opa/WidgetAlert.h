/*****************************************************************************

        WidgetAlert.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 40388

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



#if ! defined (ohm_opa_WidgetAlert_HEADER_INCLUDED)
#define  ohm_opa_WidgetAlert_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetDialog.h"
#include "ohm/opa/WidgetCheckBox.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class BundleView;
class WidgetAlert;
typedef mem::SharedPtr <WidgetAlert>   WidgetAlertSPtr;

class WidgetAlert
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      
      enum Type
      {
                  Type_OK = 0,
      };

                  Event (WidgetAlert & widget) : _widget (widget) {}
      
      WidgetAlert &
                  _widget;
      Type        _type;
      bool        _show_next_time_flag;
   };
   
   virtual        ~WidgetAlert ();
   
   static WidgetAlertSPtr
                  create (opa::BundleView & view);

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   void           set_title (const std::string & title);
   void           set_error (const std::string & error);
   void           set_explanation (const std::string & explanation);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetAlert (opa::BundleView & view);

   // delegates
   void           handle_push_button_event (WidgetPushButton::Event & event);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           fill_content ();
   void           fill_footer ();
   
   WidgetDialogSPtr
                  _widget_dialog_sptr;
   WidgetCheckBoxSPtr
                  _widget_sne_sptr;
   WidgetLabelSPtr
                  _widget_error_sptr;
   WidgetLabelSPtr
                  _widget_explanation_sptr;

   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetAlert ();
                  WidgetAlert (const WidgetAlert &other);
   WidgetAlert &  operator = (const WidgetAlert &other);
   bool           operator == (const WidgetAlert &other);
   bool           operator != (const WidgetAlert &other);

}; // class WidgetAlert



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetAlert.hpp"



#endif   // ohm_opa_WidgetAlert_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
