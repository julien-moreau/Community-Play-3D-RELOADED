/*****************************************************************************

        EnumMaster.h
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



#if ! defined (ohm_flip_EnumMaster_HEADER_INCLUDED)
#define  ohm_flip_EnumMaster_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/BasicType.h"
#include "ohm/flip/Err.h"



namespace ohm
{
namespace flip
{



class BinaryKey;

class EnumMaster
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  EnumMaster (DocumentBase & document);
   virtual        ~EnumMaster ();

   // Object
   virtual void   ctor (ohm::flip::Args & args);

   virtual archi::Int64
                  get () const;
   virtual archi::Int64
                  get_min_val () const;
   virtual archi::Int64
                  get_nbr_val () const;
   
   virtual int    get_name_for_val (const char * & name_0, archi::Int64 val) const;
   virtual int    get_val_for_name (archi::Int64 & val, const std::string & name) const;
   virtual const char *
                  get_enum_type_name () const;
   
   virtual void   ctor (archi::Int64 val);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class VirtualMachine;
   
   // VirtualMachine private access
   virtual void   set (archi::Int64 val);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EnumMaster ();
                  EnumMaster (const EnumMaster &other);
   EnumMaster &   operator = (const EnumMaster & other);
   bool           operator == (const EnumMaster &other);
   bool           operator != (const EnumMaster &other);

};



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_EnumMaster_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
