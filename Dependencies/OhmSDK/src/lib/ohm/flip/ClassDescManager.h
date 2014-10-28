/*****************************************************************************

        ClassDescManager.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if ! defined (ohm_flip_ClassDescManager_HEADER_INCLUDED)
#define  ohm_flip_ClassDescManager_HEADER_INCLUDED

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



class ClassDescriptionMaster;

class ClassDescManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum BasicTypeId
   {
                  BasicTypeId_BOOL = 1,
                  BasicTypeId_INT64,
                  BasicTypeId_ENUM,
                  BasicTypeId_FLOAT64,
                  BasicTypeId_BLOB,
                  BasicTypeId_ARRAY,
                  BasicTypeId_COLLECTION,
                  BasicTypeId_OBJECT_REF,
                  BasicTypeId_CUE,
                  
                  NBR_BasicTypeId_TOTAL
   };
   
   virtual        ~ClassDescManager ();
   
   static void    declare (ClassDescriptionMaster & class_description);

   static ClassDescManager &
                  use_instance ();

   void           declare_basic_types ();
   
   const std::string &
                  get_format_version () const;
   
   ClassDescriptionMaster &
                  get (int class_id);
   const char *   who (int class_id);
   ClassDescriptionMaster *
                  find (const std::string & class_name);
   ClassDescriptionMaster *
                  find_root ();
   bool           is_class_id_inheriting_from_class_id (int class_id, int supposed_base_class_id);

   void           post_check ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ClassDescManager ();
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::vector <ClassDescriptionMaster *> ClassIdClassDescriptionArr;
   
   void           do_declare (ClassDescriptionMaster & class_description);
   
   ClassIdClassDescriptionArr
                  _class_id_class_desc_arr;
   ClassDescriptionMaster *
                  _root_cdm_ptr;
   std::string    _format_version_str;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClassDescManager (const ClassDescManager &other);
   ClassDescManager &      operator = (const ClassDescManager &other);
   bool           operator == (const ClassDescManager &other);
   bool           operator != (const ClassDescManager &other);

}; // class ClassDescManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/ClassDescManager.hpp"



#endif   // ohm_flip_ClassDescManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
