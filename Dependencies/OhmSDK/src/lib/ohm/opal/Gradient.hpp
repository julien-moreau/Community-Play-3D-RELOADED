/*****************************************************************************

        Gradient.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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



#if defined (ohm_opal_Gradient_CURRENT_CODEHEADER)
   #error Recursive inclusion of Gradient code header.
#endif
#define  ohm_opal_Gradient_CURRENT_CODEHEADER

#if ! defined (ohm_opal_Gradient_CODEHEADER_INCLUDED)
#define  ohm_opal_Gradient_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/opal/Gradient.h"
#include "ohm/util/trace.h"

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

template <class T>
Gradient <T>::Gradient ()
:  _norm_val_map ()
,  _nbr_val (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
Gradient <T>::~Gradient ()
{
   try
   {
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("Uncatched Exception");
      assert (false);
   }
}



/*
==============================================================================
Name : clear
==============================================================================
*/

template <class T>
void  Gradient <T>::clear ()
{
   _norm_val_map.clear ();
   _nbr_val = 0;
}



/*
==============================================================================
Name : add
==============================================================================
*/

template <class T>
void  Gradient <T>::add (float norm, const T & val)
{
   _norm_val_map [norm] = val;
   ++ _nbr_val;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
T  Gradient <T>::get (float norm) const
{
   T ret_val;
   
   if (_nbr_val != 0)
   {
      if (_nbr_val != 1)
      {
         typename NormValMap::const_iterator it = _norm_val_map.upper_bound (norm);
         
         if (it == _norm_val_map.begin ())
         {
            // before first point
            
            ret_val = it->second;
         }
         else if (it == _norm_val_map.end ())
         {
            // equal or next last point
            
            typename NormValMap::const_iterator it_prev = it;
            --it_prev;
            
            ret_val = it_prev->second;
         }
         else
         {
            typename NormValMap::const_iterator it_prev = it;
            --it_prev;
            
            // equal or great than it_prev
            
            float norm_min = it_prev->first;
            float norm_max = it->first;
            
            assert (norm_min < norm_max);
            
            lang::limit_in_place (norm, norm_min, norm_max);
            
            float factor = (norm - norm_min) / (norm_max - norm_min);
            
            ret_val = T::interpolate (it_prev->second, it->second, factor);
         }
      }
      else
      {
         ret_val = _norm_val_map.begin ()->second;
      }
   }
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_Gradient_CODEHEADER_INCLUDED

#undef ohm_opal_Gradient_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
