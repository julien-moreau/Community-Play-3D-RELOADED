/*****************************************************************************

        ClassDescription.h
        Copyright (c) 2008 Ohm Force

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



#if ! defined (ohm_flip_ClassDescription_HEADER_INCLUDED)
#define  ohm_flip_ClassDescription_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/ClassDescriptionMaster.h"



namespace ohm
{
namespace flip
{



template <class T, archi::Int64, archi::Int64>  class Enum;
template <class T>   class ObjectRef;
template <class T>   class Array;
template <class T>   class Collection;

template <class T>
class ClassDescription
:  public ClassDescriptionMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ClassDescription ();
   
   static ClassDescription &
                  use ();
   
   // ClassDescriptionMaster
   virtual int    get_class_id () const;
   virtual const char *
                  get_name () const;
   virtual ClassDescriptionMaster *
                  get_super_cdm_ptr () const;
   virtual int    get_size_of () const;
   virtual const VarInfoArr &
                  get_var_info_arr () const;
   virtual const VarInfo *
                  get_var_info (const std::string & var_name) const;
   virtual bool   is_root () const;
   virtual const std::string &
                  get_format_version () const;
   
   virtual archi::Int64
                  get_nbr_ref () const;
                                 
   virtual void   set_class_id (int class_id);

   virtual void   post_check ();
   
   void           set_name (const char * name_0);
   template <class U>
   void           inherit ();
   template <class U>
   void           push_var_desc (U T::*var, const char * var_name_0);
   template <class U, archi::Int64 MIN, archi::Int64 NBR>
   void           push_var_desc (Enum <U, MIN, NBR> T::*var, const char * var_name_0);
   template <class U>
   void           push_var_desc (ObjectRef <U> T::*var, const char * var_name_0);
   template <class U>
   void           push_var_desc (Array <U> T::*var, const char * var_name_0);
   template <class U>
   void           push_var_desc (Collection <U> T::*var, const char * var_name_0);
   void           enable_root (const std::string & format_version_str);
   void           forbid_instantiation ();
   
   virtual Object *
                  create (void * ptr, DocumentBase & document);
   virtual Object *
                  create (void * ptr, DocumentBase & document, Args & args);
   virtual void   release (Object * obj_ptr);

   virtual void   ctor (Object * obj_ptr, Args & args);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ClassDescription ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   int            _class_id;
   const char *   _name_0;

   ClassDescriptionMaster *
                  _super_cdm_ptr;

   const int      _size_of;
   VarInfoArr     _var_info_arr;
   bool           _root_flag;
   std::string    _format_version_str;
   bool           _forbid_instantiation_flag;
   
   archi::Int64   _nbr_ref;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ClassDescription &other);
   bool           operator != (const ClassDescription &other);

}; // class ClassDescription



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/ClassDescription.hpp"



#endif   // ohm_flip_ClassDescription_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
