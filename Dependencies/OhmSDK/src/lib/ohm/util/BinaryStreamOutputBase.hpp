/*****************************************************************************

        BinaryStreamOutputBase.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49215

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



#if defined (ohm_util_BinaryStreamOutputBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of BinaryStreamOutputBase code header.
#endif
#define  ohm_util_BinaryStreamOutputBase_CURRENT_CODEHEADER

#if ! defined (ohm_util_BinaryStreamOutputBase_CODEHEADER_INCLUDED)
#define  ohm_util_BinaryStreamOutputBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : write
==============================================================================
*/

template <class InputIterator>
void  BinaryStreamOutputBase::write (InputIterator it, InputIterator it_end)
{
   size_t size = std::distance (it, it_end);
   
   // !!! WARNING !!!
   // keep it an archi::UInt32 (ie. not a size_t)
   write (archi::UInt32 (size));
   
   for (; it != it_end ; ++it)
   {
      write (*it);
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

template <class T, class U>
void  BinaryStreamOutputBase::write (const std::pair <T, U> & val)
{
   write (val.first);
   write (val.second);
}



/*
==============================================================================
Name : write
==============================================================================
*/

template <class T>
void  BinaryStreamOutputBase::write (const T & val)
{
   val.write (*this);
}



/*
==============================================================================
Name : write
==============================================================================
*/

template <class T>
void  BinaryStreamOutputBase::write (const mem::SharedPtr <T> & val_sptr)
{
   val_sptr->write (*this);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



#undef dd_break

#endif   // ohm_util_BinaryStreamOutputBase_CODEHEADER_INCLUDED

#undef ohm_util_BinaryStreamOutputBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
