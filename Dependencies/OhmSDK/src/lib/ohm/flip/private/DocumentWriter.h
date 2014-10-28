/*****************************************************************************

        DocumentWriter.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63154

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



#if ! defined (ohm_flip_DocumentWriter_HEADER_INCLUDED)
#define  ohm_flip_DocumentWriter_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <map>



namespace ohm
{
namespace flip
{



class ObjectManager;
class Object;
class Bool;
class Int64;
class EnumMaster;
class Float64;
class Blob;
class Cue;
class ObjectRefMaster;
class ArrayMaster;
class CollectionMaster;
class DocumentBase;
class DataStreamOutput;

class DocumentWriter
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DocumentWriter (DocumentBase & document, bool float64x_flag = true);
   virtual        ~DocumentWriter ();
   
   void           write (DataStreamOutput & dst);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           write_dst (DataStreamOutput & dst);
   void           write (DataStreamOutput & dst, Object & object);
   
   void           write_object (DataStreamOutput & dst, Object & object);
   void           write_blob (DataStreamOutput & dst, Blob & blob);
   
   void           write_member_array (DataStreamOutput & dst, ArrayMaster & member);
   void           write_member_collection (DataStreamOutput & dst, CollectionMaster & member);
   
   DocumentBase & _document;
   ObjectManager &
                  _object_manager;
   bool           _float64x_flag;
   
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentWriter ();
                  DocumentWriter (const DocumentWriter &other);
   DocumentWriter &
                  operator = (const DocumentWriter &other);
   bool           operator == (const DocumentWriter &other);
   bool           operator != (const DocumentWriter &other);

}; // class DocumentWriter



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentWriter.hpp"



#endif   // ohm_flip_DocumentWriter_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
