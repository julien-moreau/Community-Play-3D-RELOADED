/*****************************************************************************

        Event.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44889

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



#if defined (ohm_task_Event_CURRENT_CODEHEADER)
   #error Recursive inclusion of Event code header.
#endif
#define  ohm_task_Event_CURRENT_CODEHEADER

#if ! defined (ohm_task_Event_CODEHEADER_INCLUDED)
#define  ohm_task_Event_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/trace.h"

#include <typeinfo>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : push
Description :
   Push a copy of 'val' in the event.
   The push operation is sophiticated enough to support complex classes
   like mem::SharedPtr. (ie a shared ptr can travel with this class while
   keeping its cnt properties)
==============================================================================
*/

template <class T>
void  Event::push (const T & val)
{
   assert (_read_pos == 0);
   
   std::vector <archi::UByte> & data = *_data_sptr;
   
#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      size_t write_pos = data.size ();
      
      data.resize (data.size () + sizeof (void *));
      
      write_type <T> (&data [write_pos]);
   }
#endif
   
   {
      size_t type_size = sizeof (T);
      
      size_t write_pos = data.size ();
      
      data.resize (data.size () + type_size);
      
      new (&data [write_pos]) T (val);
   }
}



/*
==============================================================================
Name : push
Description :
   Push a copy of 'val' in the event, for the classes which copy ctor
   and 'operator =' have the std::auto_ptr semantic
==============================================================================
*/

template <class T>
void  Event::push (T & val)
{
   assert (_read_pos == 0);
   
   std::vector <archi::UByte> & data = *_data_sptr;
   
#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      size_t write_pos = data.size ();
      
      data.resize (data.size () + sizeof (void *));
      
      write_type <T> (&data [write_pos]);
   }
#endif
   
   {
      size_t type_size = sizeof (T);
      
      size_t write_pos = data.size ();
      
      data.resize (data.size () + type_size);
      
      new (&data [write_pos]) T (val);
   }
}



/*
==============================================================================
Name : pop
Description :
   Pop a copy of 'T' from the event.
   The internal copy is dtored.
   The pop operation is sophiticated enough to support complex classes
   like mem::SharedPtr. (ie a shared ptr can travel with this class while
   keeping its cnt properties)
==============================================================================
*/

template <class T>
T  Event::pop ()
{
   T ret_val;
   
   std::vector <archi::UByte> & data = *_data_sptr;
   
#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      assert (_read_pos + sizeof (void *) < data.size ());
      
      check_type <T> (&data [_read_pos]);

      _read_pos += sizeof (void *);
   }
#endif

   {
      assert (_read_pos + sizeof (T) <= data.size ());
      
      T * val_ptr = reinterpret_cast <T *> (&data [_read_pos]);
      _read_pos += sizeof (T);
      
      ret_val = *val_ptr;
      
      val_ptr->~T ();
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get
Description :
   get a const reference of 'T' from the event.
   The internal copy is NOT dtored.
   Client using this interface need to use skip afterward, which does the
   dtor of the internal copy.
   The get operation is sophiticated enough to support complex classes
   like mem::SharedPtr. (ie a shared ptr can travel with this class while
   keeping its cnt properties)
==============================================================================
*/

template <class T>
const T &   Event::get ()
{
   std::vector <archi::UByte> & data = *_data_sptr;
   
   size_t read_pos = _read_pos;
   
#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      assert (read_pos + sizeof (void *) < data.size ());
      
      check_type <T> (&data [_read_pos]);
      
      read_pos += sizeof (void *);
   }
#endif
   
   T * val_ptr = 0;
   
   {
      assert (read_pos + sizeof (T) <= data.size ());
      
      val_ptr = reinterpret_cast <T *> (&data [read_pos]);
   }
   
   return *val_ptr;
}



/*
==============================================================================
Name : use
Description :
   use a reference of 'T' from the event.
   The internal copy is NOT dtored.
   Client using this interface need to use skip afterward, which does the
   dtor of the internal copy.
   The use operation is sophiticated enough to support complex classes
   like mem::SharedPtr. (ie a shared ptr can travel with this class while
   keeping its cnt properties)
==============================================================================
*/

template <class T>
T &   Event::use ()
{
   std::vector <archi::UByte> & data = *_data_sptr;
   
   size_t read_pos = _read_pos;
   
#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      assert (read_pos + sizeof (void *) < data.size ());
      
      check_type <T> (&data [_read_pos]);
      
      read_pos += sizeof (void *);
   }
#endif
   
   T * val_ptr = 0;
   
   {
      assert (read_pos + sizeof (T) <= data.size ());
      
      val_ptr = reinterpret_cast <T *> (&data [read_pos]);
   }
   
   return *val_ptr;
}



/*
==============================================================================
Name : pop
Description :
   skip over a copy of 'T' from the event.
   The internal copy is dtored.
   The skip operation is sophiticated enough to support complex classes
   like mem::SharedPtr. (ie a shared ptr can travel with this class while
   keeping its cnt properties)
==============================================================================
*/

template <class T>
void  Event::skip ()
{
   std::vector <archi::UByte> & data = *_data_sptr;
   
#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      assert (_read_pos + sizeof (void *) < data.size ());
      
      check_type <T> (&data [_read_pos]);
      
      _read_pos += sizeof (void *);
   }
#endif

   {
      assert (_read_pos + sizeof (T) <= data.size ());
      
      T * val_ptr = reinterpret_cast <T *> (&data [_read_pos]);
      _read_pos += sizeof (T);
      
      val_ptr->~T ();
   }
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : push_front
Description :
   Push a copy of 'val' in front of the event. This is used to add headers
   to the event.
   The push operation is sophiticated enough to support complex classes
   like mem::SharedPtr. (ie a shared ptr can travel with this class while
   keeping its cnt properties)
==============================================================================
*/

template <class T>
void  Event::push_front (const T & val)
{
   assert (_read_pos == 0);
   
   std::vector <archi::UByte> & data = *_data_sptr;
   
   {
      data.insert (data.begin (), sizeof (T), archi::UByte (0));
      
      new (&data [0]) T (val);
   }

#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)   
   {
      data.insert (data.begin (), sizeof (void *), archi::UByte (0));
      
      write_type <T> (&data [0]);
   }
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : write_type
==============================================================================
*/

#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)
template <class T>
void  Event::write_type (archi::UByte * ptr)
{
   const std::type_info * type_info_ptr = &typeid (T);
   
   reinterpret_cast <const std::type_info * &> (*ptr) = type_info_ptr;
}
#endif   



/*
==============================================================================
Name : check_type <T>
==============================================================================
*/

#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)
template <class T>
void  Event::check_type (archi::UByte * ptr)
{
   const std::type_info * type_info_ptr
      = reinterpret_cast <const std::type_info * &> (*ptr);
   
   if (type_info_ptr != &typeid (T))
   {
      ohm_util_TRACE_ERROR2 ("Expected %1% got %2%", typeid (T), *type_info_ptr);
      assert (false);
   }
}
#endif   



}  // namespace task
}  // namespace ohm



#endif   // ohm_task_Event_CODEHEADER_INCLUDED

#undef ohm_task_Event_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
