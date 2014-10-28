/*****************************************************************************

        EventBundleView.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 50151

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



#if ! defined (ohm_opa_EventBundleView_HEADER_INCLUDED)
#define  ohm_opa_EventBundleView_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/EventBase.h"



namespace ohm
{
namespace opa
{



class EventBundleView
:  public opa::EventBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Type
   {
                  Type_LAYOUT_CHANGED = 0,
   };
   
                  EventBundleView (Type type);
   virtual        ~EventBundleView ();
   
   const Type     _type;


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventBundleView ();
                  EventBundleView (const EventBundleView &other);
   EventBundleView & operator = (const EventBundleView &other);
   bool           operator == (const EventBundleView &other) const;
   bool           operator != (const EventBundleView &other) const;

}; // class EventBundleView



}  // namespace opa
}  // namespace ohm



//#include  "ohm/studio/client/layo/EventBundleView.hpp"



#endif   // ohm_opa_EventBundleView_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

