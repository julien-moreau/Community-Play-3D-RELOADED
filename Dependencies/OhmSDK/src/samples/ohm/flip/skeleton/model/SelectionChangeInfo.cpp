/*****************************************************************************

        SelectionChangeInfo.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/SelectionChangeInfo.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: SelectionChangeInfo
==============================================================================
*/

SelectionChangeInfo::SelectionChangeInfo ()
:  _type (-1)
,  _data_type_info_ptr (0)
,  _data ()
,  _dtor_fnc (0)
{
}



/*
==============================================================================
Name: ~SelectionChangeInfo
==============================================================================
*/

SelectionChangeInfo::~SelectionChangeInfo ()
{
   destroy_data ();
}



/*
==============================================================================
Name: write
==============================================================================
*/

void SelectionChangeInfo::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   assert (&bso != 0);
   assert (_type >= 0);

   bso.write (_type);
   bso.write (
      reinterpret_cast <const archi::UByte *> (&_data_type_info_ptr),
      sizeof (_data_type_info_ptr)
   );
   bso.write (_data.begin (), _data.end ());
   bso.write (
      reinterpret_cast <const archi::UByte *> (&_dtor_fnc),
      sizeof (_dtor_fnc)
   );
}



/*
==============================================================================
Name: read
==============================================================================
*/

int SelectionChangeInfo::read (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   assert (&bsi != 0);

   const archi::UByte * data_ptr;

   bsi.read (_type);

   bsi.read (data_ptr, sizeof (_data_type_info_ptr));
   _data_type_info_ptr = * reinterpret_cast <const std::type_info * const *> (data_ptr);

   bsi.read (_data);
   
   bsi.read (data_ptr, sizeof (_dtor_fnc));
   _dtor_fnc = * reinterpret_cast <const DtorFnc *> (data_ptr);

   assert (_type >= 0);
   
   return 0;
}



/*
==============================================================================
Name: get_change_type
==============================================================================
*/

int SelectionChangeInfo::get_change_type () const
{
   assert (_type >= 0);
   
   return _type;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: destroy_data
==============================================================================
*/

void SelectionChangeInfo::destroy_data ()
{
   if (_type >= 0)
   {
      assert (_dtor_fnc != 0);
      
      (*_dtor_fnc) (*this);
   }
}



}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

