/*****************************************************************************

        DocumentObserver.h
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



#if ! defined (ohm_flip_DocumentObserver_HEADER_INCLUDED)
#define  ohm_flip_DocumentObserver_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <list>



namespace ohm
{
namespace flip
{



class DataMaster;
class Object;

class DocumentObserverMaster
{
public:
   virtual        ~DocumentObserverMaster () {}
   
   virtual void   notify_document_changed (Object & obj) = 0;
   virtual void   signal (Object * obj_ptr, archi::UInt32 type, const DataMaster & signal_data) = 0;
};

template <class T>
class DocumentObserver
:  public DocumentObserverMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DocumentObserver ();
   virtual        ~DocumentObserver ();
   
   void           add (DocumentObserver <T> & observer);
   void           remove (DocumentObserver <T> & observer);

   virtual void   document_changed (T & root);
   virtual void   signal (Object * obj_ptr, archi::UInt32 type, const DataMaster & signal_data);

   // DocumentObserverMaster
   virtual void   notify_document_changed (Object & obj);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::list <DocumentObserver <T> *> ObserverPtrList;
   
   ObserverPtrList
                  _observer_ptr_list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentObserver (const DocumentObserver &other);
   DocumentObserver &
                  operator = (const DocumentObserver &other);
   bool           operator == (const DocumentObserver &other);
   bool           operator != (const DocumentObserver &other);

}; // class DocumentObserver



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/DocumentObserver.hpp"



#endif   // ohm_flip_DocumentObserver_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
