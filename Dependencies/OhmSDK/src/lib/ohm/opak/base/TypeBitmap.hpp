/*****************************************************************************

        TypeBitmap.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if defined (ohm_opak_TypeBitmap_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeBitmap code header.
#endif
#define  ohm_opak_TypeBitmap_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeBitmap_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeBitmap_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"
#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/lang/fnc.h"

#if (ohm_opal_API == ohm_opal_API_OPENGL)
   #include "ohm/opal/opengl/OpenGLEngine.h"

#elif (ohm_opal_API == ohm_opal_API_DIRECTX)
   #include "ohm/opal/directx/DirectXEngine.h"
   
#else
   #error Unsupported Architecture
   
#endif

#include <cstring>



namespace ohm
{
namespace opak
{



/*
==============================================================================
                                 MODEL
   
   Internally, the proxy maintains a queue of animation with each element
   (target_val, transition_time, transition_function)
   
   Cumulative mode : a new element is pushed into the queue
   Immediate mode : the queue is flushed and the element is pushed into the
   queue.
   
   prev_proc : some primitive requires to know if the cur_val is going to
   change. Therefore if we are to change the value of cur_val, we need
   to call the callback before.
   
   stabilization : some primitive aspect depends on both cur_val & prev_val
   (that is not only cur_val). When stabilized, the callback will fire until
   prev_val == cur_val (that is we don't stop when cur_val reaches target_val)
   
   State properties : delay, function are state properties of this proxy.
   This means that they are implicitly accumulated in immediate mode, and
   changing them have no effect on the current animation.
   
   operator type () : the user except the following :
   a = b => a == b. So in command thread, the return value is the last one
   previously set. (the latest element pushed in cumulative mode)
   
   Loop : the animation can be looped. In that case, when a queue element
   has been fully executed, it is removed from front and pushed to back.
   Normally before starting a loop animation, the client would flush the
   current queue.
   
==============================================================================
*/



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeBitmap <Target>::TypeBitmap (Target & target, Proc proc)
:  _target (target)
,  _proc (proc)

,  _width (0)
,  _height (0)
,  _data_ptr (0)

,  _cur_val ()
{
   assert (&_target != 0);

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_add (this);
#endif   
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeBitmap <Target>::~TypeBitmap ()
{
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeBitmap <Target>::init (size_t width, size_t height, const opal::PixArgb * data_ptr)
{
   _width = width;
   _height = height;
   _data_ptr = data_ptr;
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeBitmap <Target>::init (const opal::Bitmap & bitmap)
{
   _width = bitmap.get_width ();
   _height = bitmap.get_height ();
   _data_ptr = &bitmap [0];
}



/*
==============================================================================
Name : signal
Description :
   Push current bitmap to video thread
==============================================================================
*/

template <class Target>
void  TypeBitmap <Target>::signal ()
{
   using namespace std;
   
   assert (_width > 0);
   assert (_height > 0);
   assert (_data_ptr != 0);
   
   std::vector <archi::UByte> atom_data;
   atom_data.resize (2 * sizeof (size_t) + _width * _height * 4);
   
   size_t pos = 0;
   memcpy (&atom_data [pos], &_width, sizeof (size_t));
   pos += sizeof (size_t);

   memcpy (&atom_data [pos], &_height, sizeof (size_t));
   pos += sizeof (size_t);

   memcpy (&atom_data [pos], _data_ptr, _width * _height * 4);
   
   TransactionMaster::use ().push (
      do_signal_proc, this, atom_data.size (), &atom_data [0]
   );
}



/*
==============================================================================
Name : do_signal
==============================================================================
*/

template <class Target>
void  TypeBitmap <Target>::do_signal_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_ptr != 0);
   
   TypeBitmap * this_ptr = reinterpret_cast <TypeBitmap *> (target_ptr);
   const archi::UByte * atom_ptr = reinterpret_cast <const archi::UByte *> (data_ptr);
   
   size_t width = *reinterpret_cast <const size_t *> (atom_ptr + 0);
   size_t height = *reinterpret_cast <const size_t *> (atom_ptr + sizeof (size_t));
   const archi::UByte * bitmap_ptr = atom_ptr + 2 * sizeof (size_t);
   
   this_ptr->do_signal (width, height, bitmap_ptr);
}



/*
==============================================================================
Name : do_signal
==============================================================================
*/

template <class Target>
void  TypeBitmap <Target>::do_signal (size_t width, size_t height, const archi::UByte * data_ptr)
{
   _cur_val.resize (width, height);
   memcpy (_cur_val.data (), data_ptr, width * height * sizeof (opal::PixArgb));
   
   call_proc ();
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
const opal::Bitmap & TypeBitmap <Target>::get_cur_value () const
{
   return _cur_val;
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target>
void  TypeBitmap <Target>::notify_parent_destroyable ()
{
   TransactionMaster::use ().debug_remove (this);
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : animate
==============================================================================
*/

template <class Target>
void  TypeBitmap <Target>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeBitmap_CODEHEADER_INCLUDED

#undef ohm_opak_TypeBitmap_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
