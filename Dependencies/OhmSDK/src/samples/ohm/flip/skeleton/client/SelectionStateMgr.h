/*****************************************************************************

        SelectionStateMgr.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70477

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



#if ! defined (client_SelectionStateMgr_HEADER_INCLUDED)
#define  client_SelectionStateMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Object.h"

#include <vector>



namespace model
{
class Selection;
}



namespace client
{



class SelectionStateMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  SelectionStateMgr (ohm::flip::Object & object);
   virtual        ~SelectionStateMgr ();

   void           update (bool selected_flag, ohm::archi::Int32 user_id);
   template <class Prim>
   void           apply (Prim & prim);

   template <class Prim>
   void           conflict (Prim & prim);

   template <class Prim>
   void           stop (Prim & prim);
   
   bool           get_selected () const;
   bool           get_ext_selected () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           init ();
   void           init (model::Selection & selection);
   
   ohm::flip::Object &
                  _object;
   
   std::vector <bool>
                  _ext_selection_arr;
   bool           _main_selected_flag;
   bool           _ext_selected_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SelectionStateMgr ();
                  SelectionStateMgr (const SelectionStateMgr &other);
   SelectionStateMgr &
                  operator = (const SelectionStateMgr &other);
   bool           operator == (const SelectionStateMgr &other);
   bool           operator != (const SelectionStateMgr &other);

}; // class SelectionStateMgr



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "client/SelectionStateMgr.hpp"



#endif   // client_SelectionStateMgr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
