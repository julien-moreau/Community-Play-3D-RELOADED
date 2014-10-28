/*****************************************************************************

        BundleView.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44886

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



#if defined (ohm_opa_BundleView_CURRENT_CODEHEADER)
   #error Recursive inclusion of BundleView code header.
#endif
#define  ohm_opa_BundleView_CURRENT_CODEHEADER

#if ! defined (ohm_opa_BundleView_CODEHEADER_INCLUDED)
#define  ohm_opa_BundleView_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/View.h"



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : intercalate
==============================================================================
*/

template <class LayerType>
LayerType & BundleView::intercalate (const std::string & name)
{
   NamePrimMap::iterator it = _intercalated_layer_map.find (name);
   
   if (it != _intercalated_layer_map.end ())
   {
      deintercalate (name);
   }
   
   AutoContainerPtrSPtr & prim_aptr_sptr
      = _intercalated_layer_map [name];
   
   LayerType * layer_ptr = new LayerType;
   
   prim_aptr_sptr = AutoContainerPtrSPtr (new AutoContainerPtr (layer_ptr));
   
   opak::Layer & root_layer = *_opak_view_ptr->_layer_aptr;
   
   root_layer.intercalate (*layer_ptr);
   
   return *layer_ptr;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class LayerType>
LayerType * BundleView::get (const std::string & name)
{
   NamePrimMap::iterator it = _intercalated_layer_map.find (name);
   
   LayerType * layer_ptr = 0;
   
   if (it != _intercalated_layer_map.end ())
   {
      AutoContainerPtrSPtr & prim_aptr_sptr = it->second;
      
      opak::PrimitiveContainer * container_ptr = prim_aptr_sptr->get ();
      
      layer_ptr = dynamic_cast <LayerType *> (container_ptr);
   }
   
   return layer_ptr;
}



/*
==============================================================================
Name : intercalate
==============================================================================
*/

template <class LayerType>
LayerType & BundleView::intercalate (Slot slot, const std::string & name)
{
   NamePrimMap::iterator it = _intercalated_layer_map.find (name);
   
   if (it != _intercalated_layer_map.end ())
   {
      deintercalate (name);
   }
   
   AutoContainerPtrSPtr & prim_aptr_sptr
      = _intercalated_layer_map [name];
   
   LayerType * layer_ptr = new LayerType;
   
   prim_aptr_sptr = AutoContainerPtrSPtr (new AutoContainerPtr (layer_ptr));
   
   opak::Layer & slot_layer = *_layer_aptr_arr [slot];
   
   slot_layer.intercalate (*layer_ptr);
   
   return *layer_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_BundleView_CODEHEADER_INCLUDED

#undef ohm_opa_BundleView_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
