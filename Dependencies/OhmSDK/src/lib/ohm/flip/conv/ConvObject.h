/*****************************************************************************

        ConvObject.h
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



#if ! defined (ohm_flip_ConvObject_HEADER_INCLUDED)
#define  ohm_flip_ConvObject_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Ref.h"
#include "ohm/flip/conv/ConvSharedPtr.h"
#include "ohm/flip/private/DocumentFormatter.h"
#include "ohm/flip/private/DocumentParser.h"

#include <map>
#include <list>



namespace ohm
{
namespace txt
{
namespace out
{
class Print;
}
}
namespace flip
{



class DocumentParser;
class DataStreamOutput;
class ConvArray;
class ConvCollection;

typedef std::map <std::pair <archi::Int32, archi::Int32>, archi::Int64> UserIdThreadIdRefMap;
typedef std::map <archi::Int32, archi::Int64> UserIdRefMap;
typedef std::map <archi::Int64, archi::Int64> RefConvMap;

class ConvObject
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Attribute
   {
   public:
      std::string _name;
      ConvSharedPtr <ConvObject>
                  _object_sptr;
   };
   
   class ContextXml
   {
   public:
                  ContextXml (std::string & xml) : _xml (xml), _indent (0) {}
      
      void        add_line (const std::string & xml);
      void        indent ();
      void        dedent ();
      void        print_begin (txt::out::Print & printer, const ConvObject & object);
      
      std::string _attribute_name;
      std::string _key;

   private:
      std::string &
                  _xml;
      size_t      _indent;
   };
   
   typedef        std::list <Attribute> AttributeArr;

                  ConvObject ();
   virtual        ~ConvObject ();
   
   int            read (DocumentParser & dp, flip::Ref ref);
   virtual void   write (DataStreamOutput & dst) const;

   virtual void   write_xml (ContextXml & ctx) const;

   virtual bool   has_ref_collision (std::set <flip::Ref> & ref_set) const;
   void           remap (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);
   virtual void   propagate_remap (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);
   virtual void   rebind (RefConvMap & ref_conv_map);
   void           collect_max_id (UserIdRefMap & user_id_ref_map);
   virtual void   propagate_collect_max_id (UserIdRefMap & user_id_ref_map);
   virtual void   collect (std::list <ConvObject *> & obj_ptr_arr);
   virtual void   write_member_container (DataStreamOutput & dst) const;   
   void           trace ();
   virtual void   propagate_trace ();
      
   archi::Int64   get_nbr_ref () const;
   
   static archi::Int64 &
                  get_cur_ref (UserIdThreadIdRefMap & user_id_thread_id_ref_map, archi::Int64 ref);
   
   flip::Ref      _ref;
   std::string    _class_name;
   
   AttributeArr   _attribute_arr;
   ConvObject *   _parent_ptr; // indicative, set during read, not used for write, null for root



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual void   do_write_member (DataStreamOutput & dst) const;
   virtual void   do_write_member_container (DataStreamOutput & dst) const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   int            read_attribute (DocumentParser & dp, DocumentParser::Object & object, flip::Ref & sub_ref);
   
   int            read_attribute_object (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_bool (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_int64 (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_enum (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_float64 (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_blob (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_cue (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_object_ref (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);

   int            read_attribute_array (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_array_object (DocumentParser & dp, DocumentParser::Object & object, ConvArray & array);
   int            read_attribute_array_int64 (DocumentParser & dp, DocumentParser::Object & object, ConvArray & array);
   int            read_attribute_array_float64 (DocumentParser & dp, DocumentParser::Object & object, ConvArray & array);

   int            read_attribute_collection (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref);
   int            read_attribute_collection_object (DocumentParser & dp, DocumentParser::Object & object, ConvCollection & collection);
   int            read_attribute_collection_int64 (DocumentParser & dp, DocumentParser::Object & object, ConvCollection & collection);
   int            read_attribute_collection_float64 (DocumentParser & dp, DocumentParser::Object & object, ConvCollection & collection);

   void           remap (archi::Int64 & cur_ref, UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);
   void           remap_self (archi::Int64 & cur_ref, UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);
   void           remap_attributes (archi::Int64 & cur_ref, UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);
   void           remap_children (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map);


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ConvObject (const ConvObject &other);
   ConvObject &   operator = (const ConvObject & other);
   bool           operator == (const ConvObject &other);
   bool           operator != (const ConvObject &other);

};



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_ConvObject_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
