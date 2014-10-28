/*****************************************************************************

        RootBase.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63180

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

#include "ohm/flip/RootBase.h"

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

RootBase::RootBase (flip::DocumentBase & document)
:  Object (document)
,  _scribe (*this)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

RootBase::~RootBase ()
{
}



/*
==============================================================================
Name : use_scribe
==============================================================================
*/

Scribe & RootBase::use_scribe ()
{
   return _scribe;
}



/*
==============================================================================
Name : set_scribe_metadata
==============================================================================
*/

void  RootBase::set_scribe_metadata (const std::string & metadata)
{
   _scribe.annotate ("main", metadata);
}



/*
==============================================================================
Name : has_undo
==============================================================================
*/

bool  RootBase::has_undo () const
{
   return _scribe.has_undo ();
}



/*
==============================================================================
Name : get_undo_annotation
==============================================================================
*/

void  RootBase::get_undo_annotation (std::string & annotation)
{
   assert (has_undo ());
   
   AnnotationSPtr annotation_sptr;
   
   _scribe.get_undo_annotation (annotation_sptr);
   
   Annotation::iterator it = annotation_sptr->find ("main");
      
   assert (it != annotation_sptr->end ());
   
   annotation = it->second;
}



/*
==============================================================================
Name : undo
==============================================================================
*/

int   RootBase::undo (std::string & annotation)
{
   AnnotationSPtr annotation_sptr;
   
   int err = _scribe.undo_history (annotation_sptr);
   
   if (err == 0)
   {
      Annotation::iterator it = annotation_sptr->find ("main");
      
      assert (it != annotation_sptr->end ());
      
      annotation = it->second;
   }
   
   return err;
}



/*
==============================================================================
Name : has_redo
==============================================================================
*/

bool  RootBase::has_redo () const
{
   return _scribe.has_redo ();
}



/*
==============================================================================
Name : get_redo_annotation
==============================================================================
*/

void  RootBase::get_redo_annotation (std::string & annotation)
{
   assert (has_redo ());
   
   AnnotationSPtr annotation_sptr;
   
   _scribe.get_redo_annotation (annotation_sptr);
   
   Annotation::iterator it = annotation_sptr->find ("main");
      
   assert (it != annotation_sptr->end ());
   
   annotation = it->second;
}



/*
==============================================================================
Name : redo
==============================================================================
*/

int   RootBase::redo (std::string & annotation)
{
   AnnotationSPtr annotation_sptr;
   
   int err = _scribe.redo_history (annotation_sptr);
   
   if (err == 0)
   {
      Annotation::iterator it = annotation_sptr->find ("main");
      
      assert (it != annotation_sptr->end ());
      
      annotation = it->second;
   }
   
   return err;
}



/*
==============================================================================
Name : push_history
==============================================================================
*/

void  RootBase::push_history (const DataMaster & tx_data, const AnnotationSPtr & annotation_sptr)
{
   _scribe.write_history (tx_data, annotation_sptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
