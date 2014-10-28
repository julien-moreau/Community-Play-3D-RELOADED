/*****************************************************************************

        Enum.h
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



#if ! defined (ohm_flip_Enum_HEADER_INCLUDED)
#define  ohm_flip_Enum_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/EnumMaster.h"



namespace ohm
{
namespace flip
{



template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
class Enum
:  public EnumMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    declare ();
   
                  Enum (DocumentBase & document);
   virtual        ~Enum ();

   // Object
   virtual void   ctor (ohm::flip::Args & args);
   void           ctor (EnumType val);

   // EnumMaster
   virtual void   ctor (archi::Int64 val);
   
   // EnumMaster
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
   
   Enum &         operator = (EnumType val);
                  operator EnumType () const;
   
   bool           operator == (EnumType val) const;
   bool           operator != (EnumType val) const;
   
   bool           did_value_change () const;
   EnumType       get_old_value () const;
   
   virtual void   backup ();
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;
   
   virtual void   trace () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   // EnumMaster
   virtual void   set (archi::Int64 val);

   void           make_transaction_and_set (EnumType val);

   EnumType       _val;
   EnumType       _old_val;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Enum ();
   Enum &         operator = (const Enum &other);
   bool           operator == (const Enum &other);
   bool           operator != (const Enum &other);

}; // class Enum



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/Enum.hpp"



#endif   // ohm_flip_Enum_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
