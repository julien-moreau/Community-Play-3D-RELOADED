/*****************************************************************************

        Bundle.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54225

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

#include "ohm/opa/Bundle.h"
#include "ohm/opa/base/EventBase.h"
#include "ohm/opal/Frame.h"
#include "ohm/opak/base/PrimitiveContainer.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Bundle::Bundle (const char * name_0)
:  _parent_ptr (0)
,  _parent_slot_nbr (size_t (-1))
,  _name_0 (name_0)

,  _embeddable_frame_ptr (0)
,  _embeddable_prim_ptr (0)
,  _slot_frame_ptr_arr ()
,  _slot_layer_ptr_arr ()
{
   // name_0 may be null
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Bundle::Bundle (Bundle & parent_bundle, const char * name_0)
:  _parent_ptr (0)
,  _parent_slot_nbr (size_t (-1))
,  _name_0 (name_0)

,  _embeddable_frame_ptr (0)
,  _embeddable_prim_ptr (0)
,  _slot_frame_ptr_arr ()
,  _slot_layer_ptr_arr ()
{
   // name_0 may be null
   
   parent_bundle.embed (*this);

   assert (_parent_ptr != 0);
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Bundle::Bundle (opal::View & opal_view, opak::View & opak_view)
:  _parent_ptr (0)
,  _parent_slot_nbr (size_t (-1))
,  _name_0 ("view")

,  _embeddable_frame_ptr (0)
,  _embeddable_prim_ptr (0)
,  _slot_frame_ptr_arr ()
,  _slot_layer_ptr_arr ()
{
   // name_0 may be null
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Bundle::~Bundle ()
{
   try
   {
#if defined (_MSC_VER)
   #pragma message ("TO ACTIVATE AT SOME POINT OF DEV")
#else
   #warning TO ACTIVATE AT SOME POINT OF DEV
#endif
      // we need to correct all bundles before !

#if 0    
      assert (_embeddable_frame_ptr == 0);
      assert (_embeddable_prim_ptr == 0);
      assert (_slot_frame_ptr_arr.empty ());
      assert (_slot_layer_ptr_arr.empty ());
#endif
      
      // at this point, all frames and primitives are already
      // detached, so reflect this
      
      _embeddable_frame_ptr = 0;
      _embeddable_prim_ptr = 0;
      
      _slot_frame_ptr_arr.clear ();
      _slot_layer_ptr_arr.clear ();
      
      // unlink child bundles
      
      BundlePtrSet::iterator it = _bundle_ptr_set.begin ();
      const BundlePtrSet::iterator it_end = _bundle_ptr_set.end ();
      
      for (; it != it_end ;)
      {
         BundlePtrSet::iterator it_next = it;
         ++it_next;
         
         Bundle * bundle_ptr = *it;
         assert (bundle_ptr != 0);
         
         unembed (*bundle_ptr);
         
         it = it_next;
      }
      
      // unlink parent bundle
      
      if (has_parent ())
      {
         get_parent ().unembed (*this);
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name: has_parent
==============================================================================
*/

bool Bundle::has_parent () const
{
   return (_parent_ptr != 0);
}



/*
==============================================================================
Name : get_parent
==============================================================================
*/

Bundle & Bundle::get_parent ()
{
   assert (_parent_ptr != 0);
   assert (_parent_slot_nbr != size_t (-1));
   
   return *_parent_ptr;
}



/*
==============================================================================
Name : get_parent_slot_nbr
==============================================================================
*/

size_t   Bundle::get_parent_slot_nbr ()
{
   assert (_parent_ptr != 0);
   assert (_parent_slot_nbr != size_t (-1));
   
   return _parent_slot_nbr;
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

const char *   Bundle::get_name () const
{
   return _name_0;
}



/*
==============================================================================
Name : set_embeddable
==============================================================================
*/

void  Bundle::set_embeddable (opal::Frame & frame)
{
   assert (&frame != 0);
   assert (_embeddable_frame_ptr == 0);
   
   _embeddable_frame_ptr = &frame;
}



/*
==============================================================================
Name : set_embeddable
==============================================================================
*/

void  Bundle::set_embeddable (opak::Primitive & prim)
{
   assert (&prim != 0);
   assert (_embeddable_prim_ptr == 0);
   
   _embeddable_prim_ptr = &prim;
}



/*
==============================================================================
Name : set_nbr_slot
==============================================================================
*/

void  Bundle::set_nbr_slot (size_t nbr_slot)
{
   _slot_frame_ptr_arr.resize (nbr_slot, 0);
   _slot_layer_ptr_arr.resize (nbr_slot, 0);
}



/*
==============================================================================
Name : set_slot
==============================================================================
*/

void  Bundle::set_slot (size_t slot_nbr, opal::Frame & frame)
{
   assert (&frame != 0);
   assert (slot_nbr < _slot_frame_ptr_arr.size ());
   assert (_slot_frame_ptr_arr [slot_nbr] == 0);
   
   _slot_frame_ptr_arr [slot_nbr] = &frame;
}



/*
==============================================================================
Name : set_slot
==============================================================================
*/

void  Bundle::set_slot (size_t slot_nbr, opak::PrimitiveContainer & layer)
{
   assert (&layer != 0);
   assert (slot_nbr < _slot_layer_ptr_arr.size ());
   assert (_slot_layer_ptr_arr [slot_nbr] == 0);
   
   _slot_layer_ptr_arr [slot_nbr] = &layer;
}



/*
==============================================================================
Name : set_slot
==============================================================================
*/

void  Bundle::unset_all ()
{
   _embeddable_frame_ptr = 0;
   _embeddable_prim_ptr = 0;
   
   _slot_frame_ptr_arr.clear ();
   _slot_layer_ptr_arr.clear ();
}



/*
==============================================================================
Name : embed
==============================================================================
*/

void  Bundle::embed (Bundle & bundle, size_t slot_nbr)
{
   assert (_bundle_ptr_set.find (&bundle) == _bundle_ptr_set.end ());
   
   if (bundle.has_embeddable_frame ())
   {
      opal::Frame & frame = get_slot_frame (slot_nbr);

      frame.insert_top (bundle.get_embeddable_frame ());
   }
   
   if (bundle.has_embeddable_primitive ())
   {
      opak::PrimitiveContainer & layer = get_slot_layer (slot_nbr);
   
      layer.push_back (bundle.get_embeddable_primitive ());
   }
   
   bundle.set_parent (*this, slot_nbr);
   
   _bundle_ptr_set.insert (&bundle);
}



/*
==============================================================================
Name : unembed
==============================================================================
*/

void  Bundle::unembed (Bundle & bundle)
{
   assert (_bundle_ptr_set.find (&bundle) != _bundle_ptr_set.end ());
   
   size_t slot_nbr = bundle.get_parent_slot_nbr ();
   
   if (bundle.has_embeddable_frame () && (slot_nbr < _slot_frame_ptr_arr.size ()))
   {
      opal::Frame & frame = get_slot_frame (slot_nbr);
      
      frame.remove (bundle.get_embeddable_frame ());
   }
   
   if (bundle.has_embeddable_primitive () && (slot_nbr < _slot_layer_ptr_arr.size ()))
   {
      opak::PrimitiveContainer & layer = get_slot_layer (slot_nbr);

      layer.remove (bundle.get_embeddable_primitive ());
   }
   
   bundle.set_no_parent ();
   
   _bundle_ptr_set.erase (&bundle);
}



/*
==============================================================================
Name : unembed
==============================================================================
*/

void  Bundle::unembed_immediate (Bundle & bundle)
{
   assert (_bundle_ptr_set.find (&bundle) != _bundle_ptr_set.end ());
   
   size_t slot_nbr = bundle.get_parent_slot_nbr ();
   
   if (bundle.has_embeddable_frame ())
   {
      opal::Frame & frame = get_slot_frame (slot_nbr);
      
      frame.remove (bundle.get_embeddable_frame ());
   }
   
   if (bundle.has_embeddable_primitive ())
   {
      opak::PrimitiveContainer & layer = get_slot_layer (slot_nbr);

      layer.remove_immediate (bundle.get_embeddable_primitive ());
   }
   
   bundle.set_no_parent ();
   
   _bundle_ptr_set.erase (&bundle);
}



/*
==============================================================================
Name : has_embeddable_frame
==============================================================================
*/

bool  Bundle::has_embeddable_frame () const
{
   return (_embeddable_frame_ptr != 0);
}



/*
==============================================================================
Name : get_embeddable_frame
==============================================================================
*/

opal::Frame &  Bundle::get_embeddable_frame ()
{
   assert (_embeddable_frame_ptr != 0);
   
   return *_embeddable_frame_ptr;
}



/*
==============================================================================
Name : get_embeddable_frame
==============================================================================
*/

const opal::Frame &  Bundle::get_embeddable_frame () const
{
   assert (_embeddable_frame_ptr != 0);
   
   return *_embeddable_frame_ptr;
}



/*
==============================================================================
Name : has_embeddable_primitive
==============================================================================
*/

bool  Bundle::has_embeddable_primitive () const
{
   return (_embeddable_prim_ptr != 0);
}



/*
==============================================================================
Name : get_embeddable_primitive
==============================================================================
*/

opak::Primitive & Bundle::get_embeddable_primitive ()
{
   assert (_embeddable_prim_ptr != 0);
   
   return *_embeddable_prim_ptr;
}



/*
==============================================================================
Name : get_embeddable_primitive
==============================================================================
*/

const opak::Primitive & Bundle::get_embeddable_primitive () const
{
   assert (_embeddable_prim_ptr != 0);
   
   return *_embeddable_prim_ptr;
}



/*
==============================================================================
Name : get_slot_frame
==============================================================================
*/

opal::Frame &  Bundle::get_slot_frame (size_t slot_nbr)
{
   assert (slot_nbr < _slot_frame_ptr_arr.size ());
   
   opal::Frame * frame_ptr = _slot_frame_ptr_arr [slot_nbr];
   
   assert (frame_ptr != 0);
   
   return *frame_ptr;
}



/*
==============================================================================
Name : get_slot_layer
==============================================================================
*/

opak::PrimitiveContainer & Bundle::get_slot_layer (size_t slot_nbr)
{
   assert (slot_nbr < _slot_layer_ptr_arr.size ());
   
   opak::PrimitiveContainer * layer_ptr = _slot_layer_ptr_arr [slot_nbr];
   
   assert (layer_ptr != 0);
   
   return *layer_ptr;
}



/*
==============================================================================
Name : process_event
==============================================================================
*/

void  Bundle::process_event (EventBase & event)
{
   assert (!event.is_blocked ());
   
   bool propagate_flag = true;
   
   if (propagate_flag)
   {
      DelegateSet::iterator it = _delegate_set.begin ();
      const DelegateSet::iterator it_end = _delegate_set.end ();
      
      for (; it != it_end ; ++it)
      {
         const util::Delegate & delegate = *it;
         
         delegate.param_ref_signal_if_match (event);
         
         propagate_flag = !event.is_blocked ();
         
         if (!propagate_flag)
         {
            break;
         }
      }
   }
   
   if (propagate_flag)
   {
      BundlePtrSet::iterator it = _bundle_ptr_set.begin ();
      const BundlePtrSet::iterator it_end = _bundle_ptr_set.end ();
      
      for (; it != it_end ; ++it)
      {
         Bundle * bundle_ptr = *it;
         assert (bundle_ptr != 0);
         
         bundle_ptr->process_event (event);
         
         propagate_flag = !event.is_blocked ();
         
         if (!propagate_flag)
         {
            break;
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_parent
==============================================================================
*/

void Bundle::set_parent (Bundle & parent_bundle, size_t slot_nbr)
{
   assert (_parent_ptr == 0);
   assert (_parent_slot_nbr == size_t (-1));
   assert (&parent_bundle != 0);
   
   _parent_ptr = &parent_bundle;
   _parent_slot_nbr = slot_nbr;
}



/*
==============================================================================
Name : set_no_parent
==============================================================================
*/

void Bundle::set_no_parent ()
{
   assert (_parent_ptr != 0);
   assert (_parent_slot_nbr != size_t (-1));
   
   _parent_ptr = 0;
   _parent_slot_nbr = size_t (-1);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
