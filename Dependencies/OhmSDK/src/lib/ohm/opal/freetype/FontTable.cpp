/*****************************************************************************

        FontTable.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 39877

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

#include "FontTable.h"

#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

FontTable::FontTable ()
:  _face (0)
,  _data ()
,  _pos (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FontTable::~FontTable ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : open
==============================================================================
*/

bool  FontTable::open (FT_Face face, FT_ULong table_tag)
{
   assert (_face == 0);
   
   _face = face;
   
   FT_ULong length = 0;
   FT_Error err = FT_Load_Sfnt_Table (face, table_tag, 0, 0, &length);
   
   if (err == 0)
   {
      _data.resize (length);
      
      FT_Load_Sfnt_Table (face, table_tag, 0, &_data [0], &length);
   }
   
   _pos = 0;
   
   return _data.size () > 0;
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  FontTable::save ()
{
   _saved_pos_arr.push_back (_pos);
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  FontTable::restore ()
{
   assert (_saved_pos_arr.size () > 0);
   
   _pos = _saved_pos_arr.back ();
   
   _saved_pos_arr.pop_back ();
}



/*
==============================================================================
Name : seek
==============================================================================
*/

void  FontTable::seek (long pos)
{
   _pos = pos;
}



/*
==============================================================================
Name : seek_relative
==============================================================================
*/

void  FontTable::seek_relative (long pos)
{
   _pos += pos;
}



/*
==============================================================================
Name : tell
==============================================================================
*/

long  FontTable::tell () const
{
   return _pos;
}



/*
==============================================================================
Name : read_uint32
==============================================================================
*/

archi::UInt32  FontTable::read_uint32 ()
{
   assert (!_data.empty ());
   assert (_pos + 4 <= long (_data.size ()));

   archi::UInt32 v0 = read_uint8 ();
   archi::UInt32 v1 = read_uint8 ();
   archi::UInt32 v2 = read_uint8 ();
   archi::UInt32 v3 = read_uint8 ();
   
   return ((v0 << 24) | (v1 << 16) | (v2 << 8) | v3);
}



/*
==============================================================================
Name : read_uint16
==============================================================================
*/

archi::UInt16  FontTable::read_uint16 ()
{
   assert (!_data.empty ());
   assert (_pos + 2 <= long (_data.size ()));

   archi::UInt32 v0 = read_uint8 ();
   archi::UInt32 v1 = read_uint8 ();
   
   return ((v0 << 8) | v1);
}



/*
==============================================================================
Name : read_uint8
==============================================================================
*/

archi::UByte   FontTable::read_uint8 ()
{
   assert (!_data.empty ());
   assert (_pos + 1 <= long (_data.size ()));
   
   archi::UByte ret_val = _data [_pos];
   
   _pos += 1;
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
