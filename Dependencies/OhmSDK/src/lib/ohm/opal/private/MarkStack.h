/*****************************************************************************

        MarkStack.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52274

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



#if ! defined (ohm_opal_MarkStack_HEADER_INCLUDED)
#define  ohm_opal_MarkStack_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <vector>

#include <typeinfo>



namespace ohm
{
namespace opal
{



class MarkStack
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  MarkStack ();
   virtual        ~MarkStack ();
   
   MarkStack &    operator = (const MarkStack & other);

   void           reset ();
   void           save ();
   void           restore ();
   
   template <class FrameType>
   void           set (archi::UInt32 mark_id);
   void           set (const std::type_info * info_ptr, archi::UInt32 mark_id);
   template <class FrameType>
   bool           has (archi::UInt32 mark_id) const;
   bool           has (archi::UInt32 mark_id) const;
   template <class FrameType>
   size_t         find (archi::UInt32 mark_id) const;
   size_t         find (archi::UInt32 mark_id) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class TypeInfoId
   {
   public:
      const std::type_info *
                  _info_ptr;
      archi::UInt32
                  _id;
      size_t      _stack_size;
   };
   
   typedef std::vector <TypeInfoId> TypeInfoIdArr;
   typedef std::vector <size_t> PosArr;
   
   TypeInfoIdArr  _type_info_id_arr;
   PosArr         _pos_arr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MarkStack (const MarkStack &other);
   bool           operator == (const MarkStack &other);
   bool           operator != (const MarkStack &other);

}; // class MarkStack



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opal/private/MarkStack.hpp"



#endif   // ohm_opal_MarkStack_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
