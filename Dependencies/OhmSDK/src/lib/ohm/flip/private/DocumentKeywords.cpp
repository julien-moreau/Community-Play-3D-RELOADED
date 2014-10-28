/*****************************************************************************

        DocumentKeywords.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/DocumentKeywords.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char * DocumentKeywords::_lf_0 = "\n";
const char * DocumentKeywords::_space_0 = " ";
const char * DocumentKeywords::_header_0 = "%OSF-1.0";
const char * DocumentKeywords::_obj_0 = "obj";
const char * DocumentKeywords::_endobj_0 = "endobj";
const char * DocumentKeywords::_class_name_0 = "/ClassName";
const char * DocumentKeywords::_attribute_name_0 = "/AttributeName";
const char * DocumentKeywords::_type_0 = "/Type";
const char * DocumentKeywords::_value_0 = "/Value";
const char * DocumentKeywords::_ref_0 = "/Ref";
const char * DocumentKeywords::_object_0 = "/Object";
const char * DocumentKeywords::_object_ref_0 = "/ObjectRef";
const char * DocumentKeywords::_array_0 = "/Array";
const char * DocumentKeywords::_begin_array_0 = "[";
const char * DocumentKeywords::_end_array_0 = "]";
const char * DocumentKeywords::_keys_0 = "/Keys";
const char * DocumentKeywords::_collection_0 = "/Collection";
const char * DocumentKeywords::_begin_collection_0 = "(";
const char * DocumentKeywords::_end_collection_0 = ")";
const char * DocumentKeywords::_begin_dict_0 = "<<";
const char * DocumentKeywords::_end_dict_0 = ">>";
const char * DocumentKeywords::_begin_stream_0 = "stream";
const char * DocumentKeywords::_end_stream_0 = "endstream";
const char * DocumentKeywords::_bool_0 = "/Bool";
const char * DocumentKeywords::_true_0 = "/true";
const char * DocumentKeywords::_false_0 = "/false";
const char * DocumentKeywords::_int64_0 = "/Int64";
const char * DocumentKeywords::_enum_0 = "/Enum";
const char * DocumentKeywords::_float64_0 = "/Float64";
const char * DocumentKeywords::_blob_0 = "/Blob";
const char * DocumentKeywords::_cue_0 = "/Cue";
const char * DocumentKeywords::_empty_0 = "/Empty";
const char * DocumentKeywords::_inline_0 = "/Inline";
const char * DocumentKeywords::_offset_0 = "/Offset";
const char * DocumentKeywords::_length_0 = "/Length";
const char * DocumentKeywords::_filter_0 = "/Filter";
const char * DocumentKeywords::_base64_decode_0 = "/Base64Decode";
const char * DocumentKeywords::_flate_decode_0 = "/FlateDecode";
const char * DocumentKeywords::_escaped_ascii_decode_0 = "/EscapedAsciiDecode";
const char * DocumentKeywords::_hexadecimal_decode_0 = "/HexadecimalDecode";
const char * DocumentKeywords::_xref_0 = "xref";
const char * DocumentKeywords::_trailer_0 = "trailer";
const char * DocumentKeywords::_root_0 = "/Root";
const char * DocumentKeywords::_revision_0 = "/Revision";
const char * DocumentKeywords::_version_0 = "/Version";
const char * DocumentKeywords::_user_0 = "/User";
const char * DocumentKeywords::_id_0 = "/Id";
const char * DocumentKeywords::_startxref_0 = "startxref";
const char * DocumentKeywords::_eof_0 = "%%EOF";




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
