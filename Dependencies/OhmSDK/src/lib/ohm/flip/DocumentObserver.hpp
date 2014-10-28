/*****************************************************************************

        DocumentObserver.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45364

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



#if defined (ohm_flip_DocumentObserver_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentObserver code header.
#endif
#define  ohm_flip_DocumentObserver_CURRENT_CODEHEADER

#if ! defined (ohm_flip_DocumentObserver_CODEHEADER_INCLUDED)
#define  ohm_flip_DocumentObserver_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <algorithm>
#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
DocumentObserver <T>::DocumentObserver ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
DocumentObserver <T>::~DocumentObserver ()
{
}



/*
==============================================================================
Name : notify_document_changed
==============================================================================
*/

template <class T>
void  DocumentObserver <T>::notify_document_changed (Object & obj)
{
   Object * obj_ptr = &obj;
   
   T * root_ptr = dynamic_cast <T *> (obj_ptr);
   assert (root_ptr != 0);
   
   document_changed (*root_ptr);
}



/*
==============================================================================
Name : add
==============================================================================
*/

template <class T>
void  DocumentObserver <T>::add (DocumentObserver <T> & observer)
{
   assert (&observer != 0);
   assert (
      std::find (_observer_ptr_list.begin (), _observer_ptr_list.end (), &observer)
      == _observer_ptr_list.end ()
   );
   
   _observer_ptr_list.push_back (&observer);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

template <class T>
void  DocumentObserver <T>::remove (DocumentObserver <T> & observer)
{
   assert (&observer != 0);
   
   typename ObserverPtrList::iterator it =
      std::find (_observer_ptr_list.begin (), _observer_ptr_list.end (), &observer);
   
   assert (it != _observer_ptr_list.end ());
   
   _observer_ptr_list.erase (it);
}



/*
==============================================================================
Name : document_changed
==============================================================================
*/

template <class T>
void  DocumentObserver <T>::document_changed (T & root)
{
   typename ObserverPtrList::iterator it = _observer_ptr_list.begin ();
   typename ObserverPtrList::iterator it_end = _observer_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      DocumentObserver <T> * observer_ptr = *it;
      assert (observer_ptr != 0);
      
      observer_ptr->document_changed (root);
   }
}



/*
==============================================================================
Name : signal
==============================================================================
*/

template <class T>
void  DocumentObserver <T>::signal (Object * obj_ptr, archi::UInt32 type, const DataMaster & signal_data)
{
   typename ObserverPtrList::iterator it = _observer_ptr_list.begin ();
   typename ObserverPtrList::iterator it_end = _observer_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      DocumentObserver <T> * observer_ptr = *it;
      assert (observer_ptr != 0);
      
      observer_ptr->signal (obj_ptr, type, signal_data);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_DocumentObserver_CODEHEADER_INCLUDED

#undef ohm_flip_DocumentObserver_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
