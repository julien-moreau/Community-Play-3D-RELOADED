/*****************************************************************************

        ObjectRef.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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



#if ! defined (model_ObjectRef_HEADER_INCLUDED)
#define  model_ObjectRef_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ObjectRef.h"



namespace model
{



class ObjectRef
: public ohm::flip::Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static void    declare ();

                  ObjectRef (ohm::flip::DocumentBase & document);
   virtual        ~ObjectRef ();

   bool           operator < (const ObjectRef & other);
   
   virtual void   ctor (ohm::flip::Args & args);

   ohm::flip::Ref get_object_ref () const;
   ohm::flip::Object *
                  get_object_ptr () const;
   template <class T>
   T *            get_object_ptr () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm::flip::ObjectRef <ohm::flip::Object>
                  _object_ref;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObjectRef ();
                  ObjectRef (const ObjectRef &other);
   ObjectRef &    operator = (const ObjectRef &other);
   bool           operator == (const ObjectRef &other) const;
   bool           operator != (const ObjectRef &other) const;

}; // class ObjectRef



}  // namespace model



#include "model/ObjectRef.hpp"



#endif   // model_ObjectRef_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

