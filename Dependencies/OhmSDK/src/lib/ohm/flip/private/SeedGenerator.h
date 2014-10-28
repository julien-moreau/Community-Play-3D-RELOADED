/*****************************************************************************

        SeedGenerator.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55766

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



#if ! defined (ohm_flip_SeedGenerator_HEADER_INCLUDED)
#define  ohm_flip_SeedGenerator_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace flip
{



class SeedGenerator
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum
   {
                  SIGN_BIT = 1,
                  NBR_USER_BITS = 12,
                  NBR_THREAD_BITS = 3,
                  NBR_THREAD_MAX = (1 << NBR_THREAD_BITS) - 1,
                  
                  SERVER_USER_ID = 0,
                  EDITOR_USER_ID_START_NBR = 1,
                  EDITOR_USER_ID_MAX = (1 << NBR_USER_BITS) - 2,
                  SPECTATOR_USER_ID = (1 << NBR_USER_BITS) - 1,
   };


   virtual        ~SeedGenerator ();
   
   static archi::Int64
                  get (archi::Int32 user_id, archi::Int32 thread_id);
   static archi::Int32
                  to_user_id (archi::Int64 val);
   static archi::Int32
                  to_thread_id (archi::Int64 val);
   static archi::Int64
                  to_per_user_per_thread_obj_id (archi::Int64 val);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  SeedGenerator ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SeedGenerator (const SeedGenerator &other);
   SeedGenerator &      operator = (const SeedGenerator &other);
   bool           operator == (const SeedGenerator &other);
   bool           operator != (const SeedGenerator &other);

}; // class SeedGenerator



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/SeedGenerator.hpp"



#endif   // ohm_flip_SeedGenerator_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
