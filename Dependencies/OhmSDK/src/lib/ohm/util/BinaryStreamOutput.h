/*****************************************************************************

        BinaryStreamOutput.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61015

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



#if ! defined (ohm_util_BinaryStreamOutput_HEADER_INCLUDED)
#define  ohm_util_BinaryStreamOutput_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include <vector>



namespace ohm
{
namespace util
{



class BinaryStreamOutput
:  public BinaryStreamOutputBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BinaryStreamOutput (std::vector <archi::UByte> & dst);
   virtual        ~BinaryStreamOutput ();
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // BinaryStreamOutputBase
   virtual const archi::UByte *
                  do_data () const;
   virtual size_t do_size () const;
   virtual bool   do_empty () const;
   
   virtual void   do_clear ();

   virtual archi::UByte *
                  do_resize_of (size_t diff_size);
   virtual archi::UByte *
                  do_resize_max_of (size_t pos, size_t diff_size);
   virtual void   do_resize_write_bytes (const archi::UByte * data_ptr, size_t data_size);
   virtual void   do_write_front_bytes (const archi::UByte * data_ptr, size_t data_size);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   std::vector <archi::UByte> &
                  _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BinaryStreamOutput ();
                  BinaryStreamOutput (const BinaryStreamOutput &other);
   BinaryStreamOutput &
                  operator = (const BinaryStreamOutput &other);
   bool           operator == (const BinaryStreamOutput &other);
   bool           operator != (const BinaryStreamOutput &other);

}; // class BinaryStreamOutput



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/util/BinaryStreamOutput.hpp"



#endif   // ohm_util_BinaryStreamOutput_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
