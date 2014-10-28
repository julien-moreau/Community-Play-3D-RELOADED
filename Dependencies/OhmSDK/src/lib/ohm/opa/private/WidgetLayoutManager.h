/*****************************************************************************

        WidgetLayoutManager.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58088

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



#if ! defined (ohm_opa_WidgetLayoutManager_HEADER_INCLUDED)
#define  ohm_opa_WidgetLayoutManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TransitionFunction.h"
#include "ohm/util/SingletonBase.h"
#include "ohm/task/Observer.h"

#include <set>



namespace ohm
{
namespace opa
{



class WidgetEmbedderBase;

class WidgetLayoutManager
:  public util::SingletonBase <WidgetLayoutManager>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~WidgetLayoutManager ();

   void           init ();
   void           restore ();

   void           invalidate (WidgetEmbedderBase & widget);
   void           remove (WidgetEmbedderBase & widget);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetLayoutManager ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (WidgetLayoutManager);
   
   typedef std::set <WidgetEmbedderBase *> WidgetPtrSet;
   
   void           post_process ();
   
   task::Observer _observer;
   
   bool           _active_flag;
   WidgetPtrSet   _invalid_widget_ptr_set;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetLayoutManager (const WidgetLayoutManager &other);
   WidgetLayoutManager &   operator = (const WidgetLayoutManager &other);
   bool           operator == (const WidgetLayoutManager &other);
   bool           operator != (const WidgetLayoutManager &other);

}; // class WidgetLayoutManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/private/WidgetLayoutManager.hpp"



#endif   // ohm_opa_WidgetLayoutManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
