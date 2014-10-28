/*****************************************************************************

        Int64.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49394

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



#if ! defined (ohm_flip_Int64_HEADER_INCLUDED)
#define  ohm_flip_Int64_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/BasicType.h"



namespace ohm
{
namespace flip
{



class DocumentBase;

class Int64
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    declare ();
   
                  Int64 (DocumentBase & document);
   virtual        ~Int64 ();
   
   // Object
   virtual void   ctor (ohm::flip::Args & args);
   void           ctor (archi::Int64 val);

   Int64 &        operator = (archi::Int64 val);
   Int64 &        operator += (archi::Int64 val);
   Int64 &        operator -= (archi::Int64 val);
   Int64 &        operator *= (archi::Int64 val);
   Int64 &        operator /= (archi::Int64 val);
   
                  operator archi::Int64 () const;
   
   bool           operator == (archi::Int64 val) const;
   bool           operator != (archi::Int64 val) const;

   bool           operator == (const Int64 &other) const;
   bool           operator != (const Int64 &other) const;
   
   bool           did_value_change () const;
   archi::Int64   get_old_value () const;
   
   virtual void   backup ();
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;
   
   virtual void   trace ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class VirtualMachine;

   void           set (archi::Int64 val);
   void           make_transaction_and_set (archi::Int64 val);

   archi::Int64   _val;
   archi::Int64   _old_val;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Int64 ();
                  Int64 (const Int64 &other);
   Int64 &        operator = (const Int64 &other);


}; // class Int64



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_Int64_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
