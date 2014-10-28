/*****************************************************************************

        TypeFont.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45161

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



#if ! defined (ohm_opak_TypeFont_HEADER_INCLUDED)
#define  ohm_opak_TypeFont_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeKeyframable.h"
#include "ohm/opak/base/FontProxy.h"
#include "ohm/opal/SprungImage.h"

#include <list>



namespace ohm
{
namespace opak
{



typedef std::pair <std::string, float> NameSizePair;



template <class Target>
class TypeFont
:  public TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeFont (Target & target, Proc proc);
                  TypeFont (Target & target, Proc proc, Proc prev_proc);
   virtual        ~TypeFont ();
   
   TypeFont &
                  operator = (const TypeFont &other);
   TypeFont &
                  operator = (const NameSizePair & value);
   TypeFont &
                  operator = (const std::pair <const char *, float> & pair);
                  operator const NameSizePair & () const;

   TypeFont &     push (const NameSizePair & value, double transition_time);
   TypeFont &     push (const std::string & name, float size, double transition_time);
   TypeFont &     loop ();
   TypeFont &     stop ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // TypeKeyframable
   virtual void   do_convert (FontProxy & dst, const NameSizePair & src);
   virtual void   do_interpolate (NameSizePair & dst, const NameSizePair & start_val, const NameSizePair & target_val, float interp_val);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeFont ();
                  TypeFont (const TypeFont &other);
   bool           operator == (const TypeFont &other);
   bool           operator != (const TypeFont &other);

}; // class TypeFont



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeFont.hpp"



#endif   // ohm_opak_TypeFont_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
