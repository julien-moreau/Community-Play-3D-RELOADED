/*****************************************************************************

        AllocToolsPod.hpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 15581

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



#if defined (ohm_mem_mmgr_AllocToolsPod_CURRENT_CODEHEADER)
   #error Recursive inclusion of AllocToolsPod code header.
#endif
#define  ohm_mem_mmgr_AllocToolsPod_CURRENT_CODEHEADER

#if ! defined (ohm_mem_mmgr_AllocToolsPod_CODEHEADER_INCLUDED)
#define  ohm_mem_mmgr_AllocToolsPod_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cstring>



namespace ohm
{
namespace mem
{
namespace mmgr
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
void  AllocToolsPod <T>::copy_zone (T dest_ptr [], const T src_ptr [], long len)
{
   assert (dest_ptr != 0);
   assert (src_ptr != 0);
   assert (src_ptr != dest_ptr);
   assert (len > 0);

   using namespace std;

   memcpy (dest_ptr, src_ptr, len * sizeof (dest_ptr [0]));
}



template <class T>
void  AllocToolsPod <T>::init_zone (T zone_ptr [], long len)
{
   assert (zone_ptr != 0);
   assert (len > 0);

   // Nothing
}



template <class T>
void  AllocToolsPod <T>::copy_and_init_zone (T dest_ptr [], const T src_ptr [], long len, long copy_len)
{
   assert (dest_ptr != 0);
   assert (src_ptr != 0 || copy_len == 0);
   assert (src_ptr != dest_ptr);
   assert (len > 0);
   assert (copy_len >= 0);
   assert (copy_len <= len);

   if (copy_len > 0)
   {
      copy_zone (dest_ptr, src_ptr, copy_len);
   }
}



template <class T>
void  AllocToolsPod <T>::delete_zone (T zone_ptr [], long len)
{
   assert (zone_ptr != 0);
   assert (len > 0);

   // Nothing
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_mmgr_AllocToolsPod_CODEHEADER_INCLUDED

#undef ohm_mem_mmgr_AllocToolsPod_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
