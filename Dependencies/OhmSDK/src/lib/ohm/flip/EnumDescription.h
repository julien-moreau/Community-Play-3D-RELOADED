/*****************************************************************************

        EnumDescription.h
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



#if ! defined (ohm_flip_EnumDescription_HEADER_INCLUDED)
#define  ohm_flip_EnumDescription_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/EnumDescriptionMaster.h"



namespace ohm
{
namespace flip
{



template <class T>
class EnumDescription
:  public EnumDescriptionMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~EnumDescription ();
   
   static EnumDescription &
                  use ();
   
   // EnumDescriptionMaster
   virtual int    get_enum_id () const;
   virtual const char *
                  get_name () const;
   virtual archi::Int64
                  get_min () const;
   virtual archi::Int64
                  get_nbr_total () const;
   virtual const EnumInfoArr &
                  get_enum_info_arr () const;
   virtual int    get_value_for_name (archi::Int64 & val, const std::string & name) const;
   virtual int    get_name_for_value (const char * & name_0, archi::Int64 val) const;

   virtual void   set_enum_id (int enum_id);

   virtual void   post_check ();
   
   void           set_name (const char * name_0);
   template <class U>
   void           inherit ();
   void           push_enum_desc (T val, const char * val_name_0);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  EnumDescription ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   int            _enum_id;
   const char *   _name_0;

   EnumInfoArr    _enum_info_arr;
   
   archi::Int64   _min;
   archi::Int64   _max;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const EnumDescription &other);
   bool           operator != (const EnumDescription &other);

}; // class EnumDescription



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/EnumDescription.hpp"



#endif   // ohm_flip_EnumDescription_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
