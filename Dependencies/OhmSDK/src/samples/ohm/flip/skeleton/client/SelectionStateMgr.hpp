/*****************************************************************************

        SelectionStateMgr.hpp
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



#if defined (client_SelectionStateMgr_CURRENT_CODEHEADER)
   #error Recursive inclusion of SelectionStateMgr code header.
#endif
#define  client_SelectionStateMgr_CURRENT_CODEHEADER

#if ! defined (client_SelectionStateMgr_CODEHEADER_INCLUDED)
#define  client_SelectionStateMgr_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : apply
Description :
   apply state to primitive, receiver of selection state change.
   The primitive must have the following members :
   - TypeBool::_selected_flag
   - TypeBool::_ext_selection_arr
   'update' must have been called before this call
==============================================================================
*/

template <class Prim>
void  SelectionStateMgr::apply (Prim & prim)
{
   assert (&prim != 0);
   
   if (_main_selected_flag)
   {
      if (_ext_selected_flag)
      {
         double blink_time = 0.5;
         
         prim._selected_flag
            .stop ().push (false, 0.0)
            .stop ().push (true, blink_time).push (false, blink_time).loop ();

         prim._ext_selected_flag
            .stop ().push (true, 0.0)
            .stop ().push (false, blink_time).push (true, blink_time).loop ();
      }
      else
      {
         prim._selected_flag = true;
         prim._ext_selected_flag = false;
      }
   }
   else
   {
      prim._selected_flag = false;
      prim._ext_selected_flag = _ext_selected_flag;
   }
}



/*
==============================================================================
Name : conflict
Description :
   apply conflict state to primitive, receiver of selection state change.
   The primitive must have the following members :
   - TypeBool::_conflict_flag
==============================================================================
*/

template <class Prim>
void  SelectionStateMgr::conflict (Prim & prim)
{
   assert (&prim != 0);
   
   prim._conflict_flag.stop ();
   
   double blink_time = 0.25;
   
   for (int i = 0 ; i < 4 ; ++i)
   {
      prim._conflict_flag
         .push (true, blink_time)
         .push (false, blink_time);
   }
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Prim>
void  SelectionStateMgr::stop (Prim & prim)
{
   assert (&prim != 0);
   
   prim._conflict_flag.stop ();
   prim._selected_flag.stop ();
   prim._ext_selected_flag.stop ();
   
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



#endif   // client_SelectionStateMgr_CODEHEADER_INCLUDED

#undef client_SelectionStateMgr_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
