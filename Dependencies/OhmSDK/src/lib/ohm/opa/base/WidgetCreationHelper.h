/*****************************************************************************

        WidgetCreationHelper.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38302

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



#if ! defined (ohm_opa_WidgetCreationHelper_HEADER_INCLUDED)
#define  ohm_opa_WidgetCreationHelper_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include "ohm/mem/SharedPtr.h"

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
   #define ohm_opa_WidgetCreationHelper_INVITE(x)                 \
   typedef  opa::WidgetCreationHelper <x>   Inherited;            \
   friend Inherited;
   
#else
   #define ohm_opa_WidgetCreationHelper_INVITE(x)                 \
   typedef  opa::WidgetCreationHelper <x>   Inherited;            \
   template <class> friend class opa::WidgetCreationHelper;       \
   friend class opa::WidgetCreationHelper <class x>;
   
#endif



namespace ohm
{
namespace opa
{



class Bundle;

template <class T>
class WidgetCreationHelper
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  WidgetCreationHelper ();
   virtual        ~WidgetCreationHelper ();

   static mem::SharedPtr <T>
                  create ();
   /*static mem::SharedPtr <T>
                  create (opa::Bundle & parent_bundle, size_t slot_nbr = 0);*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetCreationHelper (const WidgetCreationHelper &other);
   WidgetCreationHelper &  operator = (const WidgetCreationHelper &other);
   bool           operator == (const WidgetCreationHelper &other);
   bool           operator != (const WidgetCreationHelper &other);

}; // class WidgetCreationHelper



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetCreationHelper.hpp"



#endif   // ohm_opa_WidgetCreationHelper_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
