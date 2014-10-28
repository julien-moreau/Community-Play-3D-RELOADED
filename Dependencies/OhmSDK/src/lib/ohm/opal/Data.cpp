/*****************************************************************************

        Data.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59820

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

#include "ohm/opal/Data.h"
#include "ohm/sys/FileRefFactory.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Data::Data ()
:  _mime_type_data_map_arr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Data::~Data ()
{
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  Data::empty () const
{
   return _mime_type_data_map_arr.empty ();
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  Data::clear ()
{
   _mime_type_data_map_arr.clear ();
}



/*
==============================================================================
Name : get_file_ref_sptr
==============================================================================
*/

sys::FileRefSPtr  Data::get_file_ref_sptr (const MimeTypeDataMap & mime_type_data_map)
{
   sys::FileRefSPtr ret_val_sptr;
   
   bool continue_flag = true;
   
   if (continue_flag)
   {
      MimeTypeDataMap::const_iterator it = mime_type_data_map.find (_uti_file_url_0);
      
      if (it != mime_type_data_map.end ())
      {
         const std::vector <archi::UByte> & data = it->second;
         
         std::string file_url_str;
         file_url_str.assign (data.begin (), data.end ());
         
         ret_val_sptr = sys::FileRefFactory::from_file_url (file_url_str);
         
         continue_flag = false;
      }
   }
   
   return ret_val_sptr;
}



/*
==============================================================================
Name : get_path_list
==============================================================================
*/

void  Data::get_path_list (std::list <std::string> & path_list) const
{
   MimeTypeDataMapArr::const_iterator it = _mime_type_data_map_arr.begin ();
   const MimeTypeDataMapArr::const_iterator it_end = _mime_type_data_map_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const MimeTypeDataMap & mime_type_data_map = *it;
      
      sys::FileRefSPtr file_ref_sptr = get_file_ref_sptr (mime_type_data_map);
      
      if (file_ref_sptr.get () != 0)
      {
         std::string path = file_ref_sptr->to_utf8_path ();
         
         path_list.push_back (path);
      }
   }
}



/*
==============================================================================
Name : _mime_type_data_map
Description :
   public holder of gui data, which serves to hold drag data and pasteboard
   data.
   the mime type identify the type of data, for example "text/plain"
   see http://www.iana.org/assignments/media-types/
==============================================================================
*/

const char *   Data::_mime_text_plain_0 = "text/plain";
const char *   Data::_uti_file_url_0 = "public.file-url";




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
