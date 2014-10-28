/*****************************************************************************

        SessionManagerMemory.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45386

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

#include "ohm/flip/SessionManagerMemory.h"
#include "ohm/flip/private/DocumentKeywords.h"

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

SessionManagerMemory::SessionManagerMemory ()
:  _document_revision ()
,  _revision_rev_pos (0)
,  _document_head ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SessionManagerMemory::~SessionManagerMemory ()
{
}



/*
==============================================================================
Name : has_revision
Description :
   Returns true if we have at least one version of the document
==============================================================================
*/

bool  SessionManagerMemory::has_revision () const
{
   return (_document_revision != "");
}



/*
==============================================================================
Name : has_offline_work
Description :
   Returns true if :
   - head exists, and
   - head is different from revision (except revision number)
==============================================================================
*/

bool  SessionManagerMemory::has_offline_work () const
{
   return (_document_head != "");
}



/*
==============================================================================
Name : read_revision
==============================================================================
*/

const std::string &  SessionManagerMemory::read_revision ()
{
   assert (has_revision ());
   
   return _document_revision;
}



/*
==============================================================================
Name : read_head
==============================================================================
*/

const std::string &  SessionManagerMemory::read_head ()
{
   assert (has_offline_work ());
   
   return _document_head;
}



/*
==============================================================================
Name : write_revision
==============================================================================
*/

void  SessionManagerMemory::write_revision (const DataSPtr & data_sptr)
{
   /*_document_revision = document;
   _document_head = "";
   
   _revision_rev_pos = _document_revision.rfind (DocumentKeywords::_revision_0);*/
}



/*
==============================================================================
Name : write_head
==============================================================================
*/

void  SessionManagerMemory::write_head (const DataSPtr & data_sptr)
{
   // check if head and revision are different
   
   /*size_t head_rev_pos = document.rfind (const DataSPtr & data_sptr);
   
   bool changed_flag = false;
   
   if (_revision_rev_pos != head_rev_pos)
   {
      changed_flag = true;
   }
   else
   {
      int cmp_val = _document_revision.compare (0, _revision_rev_pos, _document_head);
      
      changed_flag = cmp_val != 0;
   }
   
   if (changed_flag)
   {
      _document_head = document;
   }*/
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
