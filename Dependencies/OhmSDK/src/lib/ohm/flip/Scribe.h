/*****************************************************************************

        Scribe.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45367

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



#if ! defined (ohm_flip_Scribe_HEADER_INCLUDED)
#define  ohm_flip_Scribe_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/DataMaster.h"
#include "ohm/flip/Annotation.h"

#include <map>
#include <string>
#include <list>



namespace ohm
{
namespace flip
{



class Object;
class DocumentBase;

class Scribe
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Scribe (Object & object);
   virtual        ~Scribe ();
   
   void           annotate (const std::string & key, const std::string & value);
   void           write_history (const DataMaster & tx_data);
   void           write_history (const DataMaster & tx_data, const AnnotationSPtr & annotation_sptr);
   void           revert_history ();
   
   bool           has_undo () const;
   void           get_undo_annotation (AnnotationSPtr & annotation_sptr);
   
   bool           has_redo () const;
   void           get_redo_annotation (AnnotationSPtr & annotation_sptr);
   
   int            undo_history (AnnotationSPtr & annotation_sptr);
   int            redo_history (AnnotationSPtr & annotation_sptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class HistoryPage
   {
   public:
      bool        _normal_direction_flag;
      DataMaster  _tx_data;
      AnnotationSPtr
                  _annotation_sptr;
   };

   typedef std::list <HistoryPage> HistoryBook;

   int            process_history (AnnotationSPtr & annotation_sptr, HistoryBook & src_book, HistoryBook & dst_book);
   
   void           notify_history_stack_changed ();
   
   void           check_undo_stack_size ();
   
   Object &       _object;
   DocumentBase & _document;
   const bool     _forward_flag;
   
   AnnotationSPtr _cur_annotation_sptr;
   
   HistoryBook    _undo_book;
   HistoryBook    _redo_book;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Scribe ();
                  Scribe (const Scribe &other);
   Scribe &       operator = (const Scribe &other);
   bool           operator == (const Scribe &other);
   bool           operator != (const Scribe &other);

}; // class Scribe



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/Scribe.hpp"



#endif   // ohm_flip_Scribe_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
