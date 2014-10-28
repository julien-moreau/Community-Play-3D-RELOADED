/*****************************************************************************

        EnumDescManager.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_flip_EnumDescManager_HEADER_INCLUDED)
#define  ohm_flip_EnumDescManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <string>
#include <map>
#include <vector>



namespace ohm
{
namespace flip
{



class EnumDescriptionMaster;

class EnumDescManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~EnumDescManager ();
   
   static void    declare (EnumDescriptionMaster & class_description);

   static EnumDescManager &
                  use_instance ();

   EnumDescriptionMaster &
                  get (int class_id);
   EnumDescriptionMaster *
                  find (const std::string & enum_name);

   void           post_check ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  EnumDescManager ();
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::vector <EnumDescriptionMaster *> EnumIdEnumDescriptionArr;
   
   void           do_declare (EnumDescriptionMaster & class_description);
   
   EnumIdEnumDescriptionArr
                  _enum_id_enum_desc_arr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EnumDescManager (const EnumDescManager &other);
   EnumDescManager &
                  operator = (const EnumDescManager &other);
   bool           operator == (const EnumDescManager &other);
   bool           operator != (const EnumDescManager &other);

}; // class EnumDescManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/EnumDescManager.hpp"



#endif   // ohm_flip_EnumDescManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
