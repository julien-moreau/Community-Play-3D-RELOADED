/*****************************************************************************

        TypeSignal.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53796

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



#if ! defined (ohm_opak_TypeSignal_HEADER_INCLUDED)
#define  ohm_opak_TypeSignal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/AnimatorListener.h"
#include "ohm/opak/base/TransitionFunction.h"
#include "ohm/opak/base/TransitionDelay.h"
#include "ohm/opak/private/TransactionDebug.h"
#include "ohm/opak/private/TypeDebug.h"

#include <list>



namespace ohm
{
namespace opak
{



template <class Target>
class TypeSignal
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
:  public TypeDebug
#endif
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeSignal (Target & target, Proc proc);
   virtual        ~TypeSignal ();
   
   void           signal ();
   
   // Video Thread
   static void    do_signal_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);

   void           do_signal ();
   
   // TypeDebug
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   virtual void   notify_parent_destroyable ();
#endif
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           call_proc ();
   
   // Immutable data
   
   Target &       _target;
   Proc           _proc;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeSignal ();
                  TypeSignal (const TypeSignal &other);
   bool           operator == (const TypeSignal &other);
   bool           operator != (const TypeSignal &other);

}; // class TypeSignal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeSignal.hpp"



#endif   // ohm_opak_TypeSignal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
