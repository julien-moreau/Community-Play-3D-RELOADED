/*****************************************************************************

        Cue.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48109

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



#if ! defined (ohm_flip_Cue_HEADER_INCLUDED)
#define  ohm_flip_Cue_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/BasicType.h"



namespace ohm
{
namespace flip
{



class DocumentBase;

class Cue
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    declare ();
   
                  Cue (DocumentBase & document);
   virtual        ~Cue ();
   
   // Object
   virtual void   ctor (ohm::flip::Args & args);
   void           ctor ();
   void           ctor (const archi::UByte * data_ptr, size_t data_size);

   void           fire ();
   void           fire (const archi::UByte * data_ptr, size_t data_size);
   
   bool           was_fired () const;
   size_t         get_elem_cnt () const;
   void           get_elem (const archi::UByte * & data_ptr, size_t & data_size, int & direction, size_t elem_nbr) const;
   
   virtual void   backup ();
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class VirtualMachine;

   void           set_cue (const archi::UByte * data_ptr, archi::UInt32 data_size, bool direction_normal_flag);
   void           make_transaction_and_set (const archi::UByte * data_ptr, size_t data_size);
   
   std::vector <archi::UByte>
                  _data;
   std::vector <size_t>
                  _pos_arr;
   std::vector <int>
                  _cnt_arr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Cue ();
                  Cue (const Cue &other);
   Cue &          operator = (const Cue &other);
   bool           operator == (const Cue &other);
   bool           operator != (const Cue &other);


}; // class Cue



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_Cue_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
