/*****************************************************************************

        TypeBlob.h
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



#if ! defined (ohm_opak_TypeBlob_HEADER_INCLUDED)
#define  ohm_opak_TypeBlob_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/opak/private/TransactionDebug.h"
#include "ohm/opak/private/TypeDebug.h"

#include <vector>



namespace ohm
{
namespace opak
{



template <class Target>
class TypeBlob
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
:  public TypeDebug
#endif
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeBlob (Target & target, Proc proc);
   virtual        ~TypeBlob ();
   
   void           set (const std::vector <archi::UByte> & data);
   
   // Video Thread
   static void    do_set_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);

   void           do_set (const archi::UByte * data_ptr, size_t data_size);

   const std::vector <archi::UByte> &
                  get_cur_value ();
   
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

   // Main thread
   
   // Video thread
   std::vector <archi::UByte>
                  _cur_val;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeBlob ();
                  TypeBlob (const TypeBlob &other);
   bool           operator == (const TypeBlob &other);
   bool           operator != (const TypeBlob &other);

}; // class TypeBlob



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeBlob.hpp"



#endif   // ohm_opak_TypeBlob_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
