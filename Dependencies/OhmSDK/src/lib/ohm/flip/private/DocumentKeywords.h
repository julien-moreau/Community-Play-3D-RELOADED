/*****************************************************************************

        DocumentKeywords.h
        Copyright (c) 2007 Ohm Force

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



#if ! defined (ohm_flip_DocumentKeywords_HEADER_INCLUDED)
#define  ohm_flip_DocumentKeywords_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



class DocumentKeywords
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~DocumentKeywords () {}

   static const char *
                  _lf_0;
   static const char *
                  _space_0;
   static const char *
                  _header_0;
   static const char *
                  _obj_0;
   static const char *
                  _endobj_0;
   static const char *
                  _class_name_0;
   static const char *
                  _attribute_name_0;
   static const char *
                  _type_0;
   static const char *
                  _value_0;
   static const char *
                  _ref_0;
   static const char *
                  _object_0;
   static const char *
                  _object_ref_0;
   static const char *
                  _array_0;
   static const char *
                  _begin_array_0;
   static const char *
                  _end_array_0;
   static const char *
                  _keys_0;
   static const char *
                  _collection_0;
   static const char *
                  _begin_collection_0;
   static const char *
                  _end_collection_0;
   static const char *
                  _begin_dict_0;
   static const char *
                  _end_dict_0;
   static const char *
                  _begin_stream_0;
   static const char *
                  _end_stream_0;
   static const char *
                  _bool_0;
   static const char *
                  _true_0;
   static const char *
                  _false_0;
   static const char *
                  _int64_0;
   static const char *
                  _enum_0;
   static const char *
                  _float64_0;
   static const char *
                  _blob_0;
   static const char *
                  _cue_0;
   static const char *
                  _empty_0;
   static const char *
                  _inline_0;
   static const char *
                  _offset_0;
   static const char *
                  _length_0;
   static const char *
                  _filter_0;
   static const char *
                  _base64_decode_0;
   static const char *
                  _flate_decode_0;
   static const char *
                  _escaped_ascii_decode_0;
   static const char *
                  _hexadecimal_decode_0;
   static const char *
                  _xref_0;
   static const char *
                  _trailer_0;
   static const char *
                  _root_0;
   static const char *
                  _revision_0;
   static const char *
                  _version_0;
   static const char *
                  _user_0;
   static const char *
                  _id_0;
   static const char *
                  _startxref_0;
   static const char *
                  _eof_0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentKeywords ();
                  DocumentKeywords (const DocumentKeywords &other);
   DocumentKeywords &      operator = (const DocumentKeywords &other);
   bool           operator == (const DocumentKeywords &other);
   bool           operator != (const DocumentKeywords &other);

}; // class DocumentKeywords



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentKeywords.hpp"



#endif   // ohm_flip_DocumentKeywords_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
