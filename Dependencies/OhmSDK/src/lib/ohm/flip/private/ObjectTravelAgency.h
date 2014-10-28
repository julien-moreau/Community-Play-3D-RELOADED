/*****************************************************************************

        ObjectTravelAgency.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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



#if ! defined (ohm_flip_ObjectTravelAgency_HEADER_INCLUDED)
#define  ohm_flip_ObjectTravelAgency_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Ref.h"

#include <vector>



namespace ohm
{
namespace flip
{



class DocumentBase;
class ObjectManager;

class ObjectTravelAgency
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum  Flag
   {
                  Flag_DEPTH_FIRST        = 1 << 0,
                  Flag_REVERSE            = 1 << 1,
                  Flag_FOLLOW_CONTAINERS  = 1 << 2,
                  Flag_SKIP_FIRST         = 1 << 3,
   };
   
                  ObjectTravelAgency (DocumentBase & document);
   virtual        ~ObjectTravelAgency ();

   template <class UTF>
   void           travel (flip::Ref ref, archi::UInt32 flags, UTF & unary_traveler_function);
   
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <class UTF>
   void           travel_normal (flip::Ref ref, UTF & unary_traveler_function);
   template <class UTF>
   void           travel_reverse (flip::Ref ref, UTF & unary_traveler_function);
   template <class UTF>
   void           travel_normal_follow_containers (flip::Ref ref, UTF & unary_traveler_function);
   template <class UTF>
   void           travel_reverse_follow_containers (flip::Ref ref, UTF & unary_traveler_function);
   template <class UTF>
   void           travel_normal_depth_first (flip::Ref ref, UTF & unary_traveler_function);
   template <class UTF>
   void           travel_reverse_depth_first (flip::Ref ref, UTF & unary_traveler_function);

   template <class UTF>
   void           execute (flip::Ref ref, UTF & unary_traveler_function);
   
   ObjectManager &_object_manager;
   bool           _first_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObjectTravelAgency ();
                  ObjectTravelAgency (const ObjectTravelAgency &other);
   ObjectTravelAgency &    operator = (const ObjectTravelAgency &other);
   bool           operator == (const ObjectTravelAgency &other);
   bool           operator != (const ObjectTravelAgency &other);

}; // class ObjectTravelAgency



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/private/ObjectTravelAgency.hpp"



#endif   // ohm_flip_ObjectTravelAgency_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
