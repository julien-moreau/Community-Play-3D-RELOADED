/*****************************************************************************

        WidgetListBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54666

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

#include "ohm/opa/base/WidgetListBase.h"
#include "ohm/lang/fnc.h"

#include <cassert>



/*
==============================================================================
Model :
   This class is the basis of widget sequence containers.
   
   All actions modifying the sequence will not be executed directly.
   It will trigger an invalidate, and will be updated in the 'synchronize'
   call.
   This allow us to have widgets which remove themselves in their delegate.
   
==============================================================================
*/



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

WidgetListBase::iterator::iterator ()
:  _it ()
,  _it_end ()
{
}



/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

WidgetListBase::iterator::iterator (const iterator & other)
:  _it (other._it)
,  _it_end (other._it_end)
{
}



/*
==============================================================================
Name : iterator::dtor
==============================================================================
*/

WidgetListBase::iterator::~iterator ()
{
}



/*
==============================================================================
Name : iterator::operator =
==============================================================================
*/

WidgetListBase::iterator & WidgetListBase::iterator::operator = (const iterator & other)
{
   _it = other._it;
   _it_end = other._it_end;
   
   return *this;
}



/*
==============================================================================
Name : iterator::operator *
==============================================================================
*/

WidgetBase &   WidgetListBase::iterator::operator * ()
{
   WidgetBase & widget_base = *_it->_widget_sptr;
   assert (&widget_base != 0);
   
   return widget_base;
}



/*
==============================================================================
Name : iterator::operator ->
==============================================================================
*/

WidgetBase *   WidgetListBase::iterator::operator -> ()
{
   WidgetBase & widget_base = *_it->_widget_sptr;
   assert (&widget_base != 0);
   
   return &widget_base;
}



/*
==============================================================================
Name : iterator::operator ++ (pre)
==============================================================================
*/

WidgetListBase::iterator & WidgetListBase::iterator::operator ++ ()
{
   assert (_it != _it_end);
   assert (_it->_state != Element::State_REMOVED);
   
   while (true)
   {
      ++_it;
      
      if (_it == _it_end)
      {
         break;
      }
      
      bool break_flag
         = (_it->_state == Element::State_RESIDENT)
         || (_it->_state == Element::State_ADDED)
      ;
      
      if (break_flag)
      {
         break;
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : iterator::operator ++ (post)
==============================================================================
*/

WidgetListBase::iterator   WidgetListBase::iterator::operator ++ (int)
{
   WidgetListBase::iterator tmp = *this;
   
   ++*this;
   
   return tmp;
}



/*
==============================================================================
Name : iterator::operator -- (pre)
==============================================================================
*/

WidgetListBase::iterator & WidgetListBase::iterator::operator -- ()
{
   if (_it != _it_end)
   {
      bool break_flag
         = (_it->_state == Element::State_RESIDENT)
         || (_it->_state == Element::State_ADDED)
      ;
      
      assert (break_flag);
   }
   
   while (true)
   {
      --_it;
      
      bool break_flag
         = (_it->_state == Element::State_RESIDENT)
         || (_it->_state == Element::State_ADDED)
      ;
      
      if (break_flag)
      {
         break;
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : iterator::operator -- (post)
==============================================================================
*/

WidgetListBase::iterator   WidgetListBase::iterator::operator -- (int)
{
   WidgetListBase::iterator tmp = *this;
   
   --*this;
   
   return tmp;
}



/*
==============================================================================
Name : iterator::operator ==
==============================================================================
*/

bool  WidgetListBase::iterator::operator == (const iterator & other) const
{
   return _it == other._it;
}



/*
==============================================================================
Name : iterator::operator !=
==============================================================================
*/

bool  WidgetListBase::iterator::operator != (const iterator & other) const
{
   return _it != other._it;
}



/*
==============================================================================
Name : iterator::was_added
==============================================================================
*/

bool  WidgetListBase::iterator::was_added () const
{
   return _it->_state == Element::State_ADDED;
}



/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

WidgetListBase::iterator::iterator (std::list <Element>::iterator it, std::list <Element>::iterator it_end)
:  _it (it)
,  _it_end (it_end)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetListBase::WidgetListBase ()
:  _need_sync_flag (false)
,  _element_arr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetListBase::~WidgetListBase ()
{
   try
   {
      clear ();
      synchronize ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  WidgetListBase::empty () const
{
   return size () == 0;
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   WidgetListBase::size () const
{
   size_t ret_val = 0;
   
   ElementArr::const_iterator it = _element_arr.begin ();
   const ElementArr::const_iterator it_end = _element_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      bool add_flag
         = (it->_state == Element::State_RESIDENT)
         || (it->_state == Element::State_ADDED)
      ;
      
      if (add_flag)
      {
         ++ret_val;
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : begin
==============================================================================
*/

WidgetListBase::iterator   WidgetListBase::begin ()
{
   ElementArr::iterator it = _element_arr.begin ();
   
   while (true)
   {
      if (it == _element_arr.end ())
      {
         break;
      }
      
      bool break_flag
         = (it->_state == Element::State_RESIDENT)
         || (it->_state == Element::State_ADDED)
      ;
      
      if (break_flag)
      {
         break;
      }
      
      ++it;
   }
   
   return iterator (it, _element_arr.end ());
}



/*
==============================================================================
Name : end
==============================================================================
*/

WidgetListBase::iterator   WidgetListBase::end ()
{
   return iterator (_element_arr.end (), _element_arr.end ());
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  WidgetListBase::erase (iterator it)
{
   do_erase (it);
   
   _need_sync_flag = true;

   invalidate_layout ();
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  WidgetListBase::erase (iterator it_first, iterator it_last)
{
   for (iterator it = it_first ; it != it_last ;)
   {
      iterator it_next = it;
      ++it_next;
      
      do_erase (it);
      
      it = it_next;
   }
   
   _need_sync_flag = true;
   invalidate_layout ();
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  WidgetListBase::clear ()
{
   erase (begin (), end ());
}



/*
==============================================================================
Name : synchronize
Description :
   Put ADDED to resident, Remove REMOVED
==============================================================================
*/

void  WidgetListBase::synchronize ()
{
   if (_need_sync_flag)
   {
      ElementArr::iterator it = _element_arr.begin ();
      const ElementArr::iterator it_end = _element_arr.end ();
      
      for (; it != it_end ;)
      {
         ElementArr::iterator it_next = it;
         ++it_next;
         
         Element & element = *it;
         
         if (element._state == Element::State_ADDED)
         {
            WidgetSPtr widget_sptr = element._widget_sptr;
            
            embed (*widget_sptr);
            
            element._state = Element::State_RESIDENT;
         }
         else if (element._state == Element::State_REMOVED)
         {
            WidgetSPtr widget_sptr = element._widget_sptr;
            
            unembed (*widget_sptr);
            
            _element_arr.erase (it);
         }
         
         it = it_next;
      }
      
      _need_sync_flag = false;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_erase
==============================================================================
*/

void  WidgetListBase::do_erase (iterator it)
{
   Element & element = *it._it;
   
   assert (element._state != Element::State_REMOVED);
   
   if (element._state == Element::State_ADDED)
   {
      // remove directly as it is not embedded
      
      _element_arr.erase (it._it);
   }
   else
   {
      element._state = Element::State_REMOVED;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
