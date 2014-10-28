/*****************************************************************************

        ObjectRef.h
        Copyright (c) 2007 Ohm Force

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



#if ! defined (ohm_flip_ObjectRef_HEADER_INCLUDED)
#define  ohm_flip_ObjectRef_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/ObjectRefMaster.h"



namespace ohm
{
namespace flip
{



template <class T>
class ObjectRef
:  public ObjectRefMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    declare ();
   
                  ObjectRef (DocumentBase & document);
   virtual        ~ObjectRef ();

   // Object
   virtual void   ctor (ohm::flip::Args & args);
   virtual void   ctor (Ref ref);

   ObjectRef &    operator = (const T * ptr);
   ObjectRef &    operator = (Ref ref);
                  operator T * () const;
   
   void           reset (const T * ptr = 0);
   
   // ObjectRefMaster
   virtual Ref    get () const;
   virtual void   set (Ref ref);
   
   bool           did_ref_change () const;
   Ref            get_old_ref () const;
   T *            get_old_ptr () const;
   
   virtual void   backup ();
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   virtual void   set_ref (Ref ref);
   void           make_transaction_and_set (Ref ref);

   Ref            _ref;
   Ref            _old_ref;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObjectRef ();
   ObjectRef &    operator = (const ObjectRef &other);
   bool           operator == (const ObjectRef &other);
   bool           operator != (const ObjectRef &other);

}; // class ObjectRef



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/ObjectRef.hpp"



#endif   // ohm_flip_ObjectRef_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
