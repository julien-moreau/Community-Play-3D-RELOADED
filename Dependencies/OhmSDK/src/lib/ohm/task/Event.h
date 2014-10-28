/*****************************************************************************

        Event.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45089

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



#if ! defined (ohm_task_Event_HEADER_INCLUDED)
#define  ohm_task_Event_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"
#include "ohm/conc/AtomicInt.h"
#include "ohm/mem/SharedPtr.h"

#include <vector>

#define OHM_TASK_EVENT_SOURCE_FLAG
#define OHM_TASK_EVENT_SECURE_TYPE_FLAG

#if defined (OHM_TASK_EVENT_SOURCE_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_EVENT_SOURCE_FLAG is defined in shipping level code !
   #endif
   
   #if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
      #define ohm_task_EVENT_GET ohm::task::Event::get (__FUNCDNAME__);

   #elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
      #define ohm_task_EVENT_GET ohm::task::Event::get (__PRETTY_FUNCTION__);

   #else // ohm_archi_COMPILER_CLASS
      #error Compiler not supported

   #endif // ohm_archi_SYS
   
#else // defined (OHM_TASK_EVENT_SOURCE_FLAG)
   #define ohm_task_EVENT_GET ohm::task::Event::get (0);
   
#endif // defined (OHM_TASK_EVENT_SOURCE_FLAG)

#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_EVENT_SECURE_TYPE_FLAG is defined in shipping level code !
   #endif
#endif // defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)



namespace ohm
{
namespace task
{



class Event
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static Event & get (const char * name_0);
   
                  Event (const Event &other);
   virtual        ~Event ();

   Event &        operator = (const Event &other);

   template <class T>
   void           push (const T & val);
   template <class T>
   void           push (T & val);
   template <class T>
   T              pop ();
   template <class T>
   const T &      get ();
   template <class T>
   T &            use ();
   template <class T>
   void           skip ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

                  Event ();
   void           bind_cell (void * cell_ptr, const char * name_0);
   void           return_cell ();
   void           clean_cell ();
   void *         get_cell () const;
   const char *   get_name () const;

   template <class T>
   void           push_front (const T & val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

#if defined (OHM_TASK_EVENT_SECURE_TYPE_FLAG)
   template <class T>
   void           write_type (archi::UByte * ptr);
   template <class T>
   void           check_type (archi::UByte * ptr);
#endif   
   
   const char *   _name_0;
   
   void *         _cell_ptr;
   conc::AtomicInt <long> *
                  _count_ptr;
   
   mem::SharedPtr <std::vector <archi::UByte> >
                  _data_sptr;
   size_t         _read_pos;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Event &other) const;
   bool           operator != (const Event &other) const;

}; // class Event



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/Event.hpp"



#endif   // ohm_task_Event_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
