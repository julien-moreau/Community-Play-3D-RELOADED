/*****************************************************************************

        SingletonBase.h
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 70249

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



#if ! defined (ohm_util_SingletonBase_HEADER_INCLUDED)
#define  ohm_util_SingletonBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include <memory>



#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
   #define ohm_util_SingletonBase_INVITE_SINGLETON(x)          \
   typedef ohm::util::SingletonBase <x>   ohm_util_SingletonBase_Inherited;   \
   friend ohm_util_SingletonBase_Inherited;
   
#else
   #define ohm_util_SingletonBase_INVITE_SINGLETON(x)          \
   template <class> friend class ohm::util::SingletonBase;           \
   friend class ohm::util::SingletonBase <class x>;
   
#endif



namespace ohm
{
namespace util
{



template <class T>
class SingletonBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  ObjectType;
   typedef  std::auto_ptr <ObjectType>  ObjectAPtr;

   virtual        ~SingletonBase () {}

   static ObjectType &
                  use_instance ();
   static void    force_destroy ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  SingletonBase ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static ObjectAPtr
                  _singleton_aptr;
   static bool    _singleton_init_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  SingletonBase (const SingletonBase &other);
   SingletonBase& operator = (const SingletonBase &other);
   bool           operator == (const SingletonBase &other);
   bool           operator != (const SingletonBase &other);

}; // class SingletonBase



}  // namespace util
}  // namespace ohm



#include "ohm/util/SingletonBase.hpp"



#endif   // ohm_util_SingletonBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
