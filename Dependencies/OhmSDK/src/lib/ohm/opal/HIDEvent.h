/*****************************************************************************

        HIDEvent.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70945

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



#if ! defined (ohm_opal_HIDEvent_HEADER_INCLUDED)
#define  ohm_opal_HIDEvent_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opal/Key.h"

#include <string>



namespace ohm
{
namespace util
{
class BinaryStreamOutputBase;
class BinaryStreamInput;
}


namespace opal
{



class HIDEvent
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Type
   {
                  Type_NONE = -1,
                  
                  Type_MOUSE_BUTTON_PRESSED = 0,
                  Type_MOUSE_BUTTON_RELEASED,
                  Type_MOUSE_MOVED,
                  Type_MOUSE_WHEEL_MOVED,
                  
                  Type_KEY_PRESSED,
                  Type_KEY_REPEATED,
                  Type_KEY_RELEASED,
                  Type_CHARACTERS_TYPED,
   };
   
                  HIDEvent ();
   virtual        ~HIDEvent ();

   void           write (util::BinaryStreamOutputBase & bso) const;
   int            read (util::BinaryStreamInput & bsi);
   
   void           trace () const;
   
   Type           _type;
   int            _button_nbr;
   int            _click_count;
   opa::Point     _location;
   opa::Point     _delta;
   Key            _key;
   std::string    _str;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const HIDEvent &other);
   bool           operator != (const HIDEvent &other);

}; // class HIDEvent



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_HIDEvent_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
