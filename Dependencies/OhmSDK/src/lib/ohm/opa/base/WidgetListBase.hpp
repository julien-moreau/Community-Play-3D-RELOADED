/*****************************************************************************

        WidgetListBase.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57590

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



#if defined (ohm_opa_WidgetListBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetListBase code header.
#endif
#define  ohm_opa_WidgetListBase_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetListBase_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetListBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : push
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetListBase::push ()
{
   mem::SharedPtr <T> widget_sptr = T::create ();
   
   push (widget_sptr);
   
   return widget_sptr;
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class T>
void  WidgetListBase::push (mem::SharedPtr <T> widget_sptr)
{
   insert (end (), widget_sptr);
}



template <class T>
void  WidgetListBase::remove (mem::SharedPtr <T> widget_sptr)
{
   iterator it = find (widget_sptr);
   
   if (it != end ())
   {
      erase (it);
   }
}



/*
==============================================================================
Name : insert
==============================================================================
*/

template <class T>
WidgetListBase::iterator   WidgetListBase::insert (iterator it, mem::SharedPtr <T> widget_sptr)
{
   Element element;
   element._widget_sptr = mem::shared_dynamic_cast <WidgetBase, T > (widget_sptr);
   element._state = Element::State_ADDED;
   
   _need_sync_flag = true;
   invalidate_layout ();
   
   return iterator (_element_arr.insert (it._it, element), _element_arr.end ());
}



/*
==============================================================================
Name : find
==============================================================================
*/

template <class T>
WidgetListBase::iterator   WidgetListBase::find (mem::SharedPtr <T> widget_sptr)
{
   assert (widget_sptr.get () != 0);
   
   WidgetBase * widget_ptr = widget_sptr.get ();
   
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      if (&*it == widget_ptr)
      {
         break;
      }
   }
   
   return it;
}



/*
==============================================================================
Name : sort
==============================================================================
*/

template <class T>
void  WidgetListBase::sort ()
{
   Comparator <T> comp;
   
   _element_arr.sort (comp);
   
   invalidate_layout ();
}



/*
==============================================================================
Name : Comparator::operator ()
==============================================================================
*/

template <class T>
bool  WidgetListBase::Comparator <T>::operator () (const Element & l_op, const Element & r_op)
{
   bool less_flag = true;
   
   WidgetBase * l_op_ptr = l_op._widget_sptr.get ();
   assert (l_op_ptr != 0);
   
   WidgetBase * r_op_ptr = r_op._widget_sptr.get ();
   assert (r_op_ptr != 0);
   
   T * l_ptr = dynamic_cast <T *> (l_op_ptr);
   assert (l_ptr != 0);

   T * r_ptr = dynamic_cast <T *> (r_op_ptr);
   assert (r_ptr != 0);
   
   if ((l_ptr != 0) && (r_ptr != 0))
   {
      less_flag = *l_ptr < *r_ptr;
   }
   
   return less_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetListBase_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetListBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
