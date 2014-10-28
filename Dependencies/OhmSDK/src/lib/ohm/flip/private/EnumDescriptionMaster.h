/*****************************************************************************

        EnumDescriptionMaster.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if ! defined (ohm_flip_EnumDescriptionMaster_HEADER_INCLUDED)
#define  ohm_flip_EnumDescriptionMaster_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Err.h"

#include <string>
#include <vector>



namespace ohm
{
namespace flip
{



class Object;
class DocumentBase;
class Args;

class EnumDescriptionMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class EnumInfo
   {
   public:
      archi::Int64
                  _val;
      const char *
                  _name_0;
   };
   
   typedef std::vector <EnumInfo>   EnumInfoArr;

   virtual        ~EnumDescriptionMaster () {}
   
   virtual int    get_enum_id () const = 0;
   virtual const char *
                  get_name () const = 0;
   virtual archi::Int64
                  get_min () const = 0;
   virtual archi::Int64
                  get_nbr_total () const = 0;
   virtual const EnumInfoArr &
                  get_enum_info_arr () const = 0;
   virtual int    get_value_for_name (archi::Int64 & val, const std::string & name) const = 0;
   virtual int    get_name_for_value (const char * & name_0, archi::Int64 val) const = 0;

   virtual void   set_enum_id (int enum_id) = 0;

   virtual void   post_check () = 0;
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const EnumDescriptionMaster &other);
   bool           operator != (const EnumDescriptionMaster &other);

}; // class EnumDescriptionMaster



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/EnumDescriptionMaster.hpp"



#endif   // ohm_flip_EnumDescriptionMaster_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
