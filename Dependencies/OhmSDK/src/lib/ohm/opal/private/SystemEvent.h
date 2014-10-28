/*****************************************************************************

        SystemEvent.h
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 70201

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



#if ! defined (ohm_opal_SystemEvent_HEADER_INCLUDED)
#define  ohm_opal_SystemEvent_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opal/Key.h"



namespace ohm
{
namespace opal
{



class SystemEvent
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum MouseButton
   {
                  MouseButton_L = 0,
                  MouseButton_R,    

                  MouseButton_NBR_ELT
   };

   enum KeyChange
   {  
                  KeyChange_NONE = 0,
                  KeyChange_PRESSED,
                  KeyChange_REPEATED,
                  KeyChange_RELEASED,
   };

                  SystemEvent ();
   virtual        ~SystemEvent ();

   bool           operator == (const SystemEvent &other) const;
   bool           operator != (const SystemEvent &other) const;

   void           reset_transient ();
   
   opa::Point     _mouse_location;
   int            _click_cnt_arr [MouseButton_NBR_ELT];
   opal::Key      _key;
   KeyChange      _key_change;
   std::string    _char_utf8;
   opa::Point     _wheel_delta;
   bool           _layout_changed_flag;
   bool           _mouse_exited_view_flag;
   bool           _has_drag_data_flag;
   bool           _drag_data_dropped_flag;
   bool           _keyboard_focus_lost_flag;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SystemEvent (const SystemEvent &other);
   SystemEvent &  operator = (const SystemEvent &other);

}; // class SystemEvent



}  // namespace opal
}  // namespace ohm



//#include "ohm/opal/SystemEvent.hpp"



#endif   // ohm_opal_SystemEvent_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

