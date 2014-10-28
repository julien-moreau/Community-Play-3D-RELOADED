/*****************************************************************************

        ConvArray.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70008

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



#if ! defined (ohm_flip_ConvArray_HEADER_INCLUDED)
#define  ohm_flip_ConvArray_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/conv/ConvObject.h"
#include "ohm/flip/conv/ConvSharedPtr.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/mem/SafeList.h"



namespace ohm
{
namespace flip
{



class ConvArray
:  public ConvObject
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Element
   {
   public:
      BinaryKey   _key;
      flip::ConvSharedPtr <ConvObject>
                  _object_sptr;
   };
   
   typedef        mem::SafeList <Element> ElementList;
   
                  ConvArray ();
   virtual        ~ConvArray ();

   virtual bool   has_ref_collision (std::set <flip::Ref> & ref_set) const;
   virtual void   propagate_remap (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);
   virtual void   rebind (RefConvMap & ref_conv_map);
   virtual void   propagate_collect_max_id (UserIdRefMap & user_id_ref_map);
   virtual void   collect (std::list <ConvObject *> & obj_ptr_arr);
   virtual void   propagate_trace ();

   virtual void   write_xml (ContextXml & ctx) const;

   static const char *
                  get_class_name ();
                     
   int            _elem_class_id;
   ElementList    _content;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual void   do_write_member (DataStreamOutput & dst) const;
   virtual void   do_write_member_container (DataStreamOutput & dst) const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ConvArray (const ConvArray &other);
   ConvArray &    operator = (const ConvArray & other);
   bool           operator == (const ConvArray &other);
   bool           operator != (const ConvArray &other);

};



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_ConvArray_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
