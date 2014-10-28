/*****************************************************************************

        ClassDescriptionMaster.h
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



#if ! defined (ohm_flip_ClassDescriptionMaster_HEADER_INCLUDED)
#define  ohm_flip_ClassDescriptionMaster_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <string>
#include <vector>



namespace ohm
{
namespace flip
{



class Object;
class DocumentBase;
class Args;



class ClassDescriptionMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class VarInfo
   {
   public:
      int         _type_id;
      ClassDescriptionMaster *
                  _cdm_ptr;
      const char *
                  _name_0;
      archi::Int64
                  _var_offset;
   };
   
   typedef std::vector <VarInfo> VarInfoArr;

   virtual        ~ClassDescriptionMaster () {}
   
   virtual int    get_class_id () const = 0;
   virtual const char *
                  get_name () const = 0;
   virtual ClassDescriptionMaster *
                  get_super_cdm_ptr () const = 0;
   virtual int    get_size_of () const = 0;
   virtual const VarInfoArr &
                  get_var_info_arr () const = 0;
   virtual const VarInfo *
                  get_var_info (const std::string & var_name) const = 0;
   virtual bool   is_root () const = 0;
   virtual const std::string &
                  get_format_version () const = 0;

   virtual Object *
                  create (void * ptr, DocumentBase & document) = 0;
   virtual Object *
                  create (void * ptr, DocumentBase & document, Args & args) = 0;
   virtual void   release (Object * obj_ptr) = 0;
   
   virtual void   ctor (Object * obj_ptr, Args & args) = 0;

   virtual archi::Int64
                  get_nbr_ref () const = 0;

   virtual void   set_class_id (int class_id) = 0;

   virtual void   post_check () = 0;
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ClassDescriptionMaster &other);
   bool           operator != (const ClassDescriptionMaster &other);

}; // class ClassDescriptionMaster



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/ClassDescriptionMaster.hpp"



#endif   // ohm_flip_ClassDescriptionMaster_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
