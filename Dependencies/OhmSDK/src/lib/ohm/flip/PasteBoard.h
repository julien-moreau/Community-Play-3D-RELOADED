/*****************************************************************************

        PasteBoard.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70444

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



#if ! defined (ohm_flip_PasteBoard_HEADER_INCLUDED)
#define  ohm_flip_PasteBoard_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"
#include "ohm/flip/ObjectMold.h"
#include "ohm/flip/private/TypeTraits.h"

#include <list>
#include <map>
#include <string>



namespace ohm
{
namespace flip
{



class Object;

class PasteBoard
:  public util::SingletonBase <PasteBoard>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
public:
   
   typedef ObjectMoldSPtrList::iterator iterator;
   
   virtual        ~PasteBoard ();
   
   iterator       begin ();
   iterator       end ();
   
   bool           empty () const;
   void           clear ();
   ObjectMold &   add (Object & object);
   
   void           paste ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  PasteBoard ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (PasteBoard)
   
   ObjectMoldSPtrList
                  _mold_sptr_list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PasteBoard (const PasteBoard &other);
   PasteBoard &      operator = (const PasteBoard &other);
   bool           operator == (const PasteBoard &other);
   bool           operator != (const PasteBoard &other);

}; // class PasteBoard



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/PasteBoard.hpp"



#endif   // ohm_flip_PasteBoard_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
