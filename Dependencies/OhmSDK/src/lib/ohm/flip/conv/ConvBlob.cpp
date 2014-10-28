/*****************************************************************************

        ConvBlob.cpp
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

#include "ohm/flip/conv/ConvBlob.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/DocumentKeywords.h"

#include "ohm/txt/out/Print.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ConvBlob::ConvBlob ()
:  ConvObject ()
,  _encoder_arr ()
,  _inline_flag (false)
,  _data ()
{
   _class_name = get_class_name ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConvBlob::~ConvBlob ()
{
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  ConvBlob::write (DataStreamOutput & dst) const
{
   assert (!_inline_flag);
   
   DocumentFormatter::write_begin_blob (dst, _ref, _data, _encoder_arr);
   DocumentFormatter::write_blob (dst, _data);
   DocumentFormatter::write_end_blob (dst);
}



/*
==============================================================================
Name : collect
Description :
   collect writable objects :
   - BasicTypes are not writable
   - Blobs are writable iff they are non empty or non inline
   - All other objets are writable
==============================================================================
*/

void  ConvBlob::collect (std::list <ConvObject *> & obj_ptr_arr)
{
   // "Blobs are writable iff they are non empty or non inline"
   
   bool writable_flag = true;
   writable_flag &= !_data.empty ();
   writable_flag &= !_inline_flag;
   
   if (writable_flag)
   {
      obj_ptr_arr.push_back (this);
   }
}



/*
==============================================================================
Name : write_xml
==============================================================================
*/

void  ConvBlob::write_xml (ContextXml & ctx) const
{
   txt::out::Print printer;
   
   ctx.print_begin (printer, *this);
   printer.cat (" inline=\"%1%\">").arg (_inline_flag ? "true" : "false");
   ctx.add_line (printer);
   
   ctx.indent ();
   
   if (_encoder_arr.empty ())
   {
      ctx.add_line ("<encoders/>");
   }
   else
   {
      ctx.add_line ("<encoders>");
      ctx.indent ();
      
      for (int i = _encoder_arr.size () - 1 ; i >= 0 ; --i)
      {
         Codec::Encoder encoder = _encoder_arr [i];
         
         std::string xml;
         
         xml += "<encoder type=\"";
         
         std::string encoder_str;
         
         switch (encoder)
         {
         case Codec::Encoder_BASE64:
            encoder_str = DocumentKeywords::_base64_decode_0;
            break;

         case Codec::Encoder_DEFLATE:
            encoder_str = DocumentKeywords::_flate_decode_0;
            break;
         
         case Codec::Encoder_ESCAPED_ASCII:
            encoder_str = DocumentKeywords::_escaped_ascii_decode_0;
            break;
         
         case Codec::Encoder_HEXADECIMAL:
            encoder_str = DocumentKeywords::_hexadecimal_decode_0;
            break;
         
         default:
            assert (false);
            break;
         }
         
         encoder_str.erase (0, 1);
         
         xml += encoder_str;
         
         xml += "\"/>";
         
         ctx.add_line (xml);
      }
      
      ctx.dedent ();
      ctx.add_line ("</encoders>");
   }
   
   if (_data.empty ())
   {
      ctx.add_line ("<data/>");
   }
   else
   {
      // bool check to write ascii & xml compatible
      
      bool ok_flag = true;
      
      for (size_t i = 0 ; i < _data.size () ; ++i)
      {
         char c = _data [i];
         
         ok_flag &= (c >= 32) && (c <= 126);
         ok_flag &= c != '\"';
         ok_flag &= c != '\'';
         ok_flag &= c != '<';
         ok_flag &= c != '>';
         ok_flag &= c != '&';
         
         if (!ok_flag)
         {
            break;
         }
      }
      
      bool base64_flag = !ok_flag;
      
      std::string xml;
      
      if (base64_flag)
      {
         xml += "<data base64=\"true\">";
      }
      else
      {
         xml += "<data>";
      }
      
      std::vector <archi::UByte> data;
      
      if (base64_flag)
      {
         xml += "\n";
         
         Codec::encode_base64 (data, &_data [0], _data.size ());
      }
      else
      {
         data = _data;
      }
      
      xml.insert (xml.end (), data.begin (), data.end ());
      
      if (base64_flag)
      {
         ctx.add_line (xml);
         ctx.add_line ("</data>");
      }
      else
      {
         xml += "</data>";
         ctx.add_line (xml);
      }
   }
   
   ctx.dedent ();
   
   printer.print ("</%1%>").arg (_class_name.c_str ());
   ctx.add_line (printer);
}



/*
==============================================================================
Name: get_class_name
==============================================================================
*/

const char * ConvBlob::get_class_name ()
{
   return BasicTypesName::_blob_0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_write_member
==============================================================================
*/

void  ConvBlob::do_write_member (DataStreamOutput & dst) const
{
   if (_data.empty ())
   {
      DocumentFormatter::write_member_blob_empty (dst, _inline_flag);
   }
   else if (_inline_flag)
   {
      DocumentFormatter::write_member_blob_inline (dst, _data, _encoder_arr);
   }
   else
   {
      DocumentFormatter::write_member_blob (dst, _ref);
   }
}




/*
==============================================================================
Name : do_write_member_container
==============================================================================
*/

void  ConvBlob::do_write_member_container (DataStreamOutput & dst) const
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
