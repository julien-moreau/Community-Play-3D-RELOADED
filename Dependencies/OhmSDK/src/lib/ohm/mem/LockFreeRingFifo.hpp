/*****************************************************************************

        LockFreeRingFifo.hpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 63002

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



#if defined (ohm_mem_LockFreeRingFifo_CURRENT_CODEHEADER)
   #error Recursive inclusion of LockFreeRingFifo code header.
#endif
#define  ohm_mem_LockFreeRingFifo_CURRENT_CODEHEADER

#if ! defined (ohm_mem_LockFreeRingFifo_CODEHEADER_INCLUDED)
#define  ohm_mem_LockFreeRingFifo_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/conc/AioAddMod.h"
#include "ohm/conc/AtomicIntOp.h"

#include <cassert>



namespace ohm
{
namespace mem
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*==========================================================================*/
/*      Name: Ctor                                                          */
/*      Description: Instantiates the FIFO. There can't be any concurent    */
/*                   calls until the object is totally build.               */
/*      Input parameters:                                                   */
/*        - size : Default size of the FIFO, must be > 0                    */
/*      Throws: ?                                                           */
/*==========================================================================*/

template <class T>
LockFreeRingFifo<T>::LockFreeRingFifo (long size)
:  _buf_ptr (0)
,  _write_pos (0)
,  _read_pos (0)
,  _size (size)
{
   assert (size > 0);
   _buf_ptr = new T [size];
}



/*==========================================================================*/
/*      Name: dtor                                                          */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
LockFreeRingFifo<T>::~LockFreeRingFifo ()
{
   delete [] _buf_ptr;
   _buf_ptr = 0;
}



/*==========================================================================*/
/*      Name: resize                                                        */
/*      Description: Change the FIFO size. There can't be any concurent     */
/*                   calls to member function until the function returns.   */
/*      Input parameters:                                                   */
/*        - size : New size, must be > 0.                                   */
/*      Throws: ?                                                           */
/*==========================================================================*/

template <class T>
void  LockFreeRingFifo<T>::resize (long size)
{
   assert (size > 0);

   if (size != _size)
   {
      // Allocate a new array
      T *            temp_buf_ptr = new T [size];

      // Copy meaningful data
      long           pos = 0;
      while (! is_empty ())
      {
         pos += pop_array (&temp_buf_ptr [pos], size-1 - pos);
      }
      _read_pos = 0;
      _write_pos = pos;

      // Delete old array and attach the new one
      delete [] _buf_ptr;
      _buf_ptr = temp_buf_ptr;
      _size = size;
   }
}



/*==========================================================================*/
/*      Name: clear                                                         */
/*      Description: Clear the FIFO. There can't be any concurent calls to  */
/*                   member function until the function returns.            */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
void  LockFreeRingFifo<T>::clear ()
{
   discard (get_nbr_stored_elt ());
   assert (is_empty ());
   _read_pos = 0;
   _write_pos = 0;
}



/*==========================================================================*/
/*      Name: is_full                                                       */
/*      Description: Check if the FIFO is full (no room for new data).      */
/*                   Should be called by WRITE thread only.                 */
/*      Returns: true if the FIFO is full.                                  */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::is_full () const
{
   return ((_write_pos + 1) % _size == _read_pos);
}



/*==========================================================================*/
/*      Name: get_nbr_free_elt                                              */
/*      Description: Returns the number of free slots in the FIFO.          */
/*                   Should be called by WRITE thread only.                 */
/*      Returns: Number of free slots.                                      */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
long  LockFreeRingFifo<T>::get_nbr_free_elt () const
{
   return ((_read_pos + _size - _write_pos - 1) % _size);
}



/*==========================================================================*/
/*      Name: push                                                          */
/*      Description: If enough room, push the object in the FIFO.           */
/*                   Should be called by WRITE thread only.                 */
/*      Input parameters:                                                   */
/*        - data : object to store into the FIFO. The object is copied.     */
/*      Returns: true if succeeded (enough room, and no error).             */
/*      Throws: ?                                                           */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::push (const T &data)
{
   const bool     can_push_flag = ! is_full ();
   if (can_push_flag)
   {
      _buf_ptr [_write_pos] = data;
      const conc::AioAddMod <PosType>  ftor_inc (1, _size);
      conc::AtomicIntOp_exec (_write_pos, ftor_inc);
   }

   return (can_push_flag);
}



/*==========================================================================*/
/*      Name: push_array                                                    */
/*      Description: Store the whole array of objects in the FIFO if there  */
/*                   is enough room to store them all. All the objects are  */
/*                   duplicated.                                            */
/*                   Should be called by WRITE thread only.                 */
/*      Input parameters:                                                   */
/*        - data_arr : Pointer on the array of objects.                     */
/*        - nbr_elt : Number of elements to store                           */
/*      Returns: true if all objects could be pushed successfully.          */
/*      Throws: ?                                                           */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::push_array (const T data_arr [], long nbr_elt)
{
   assert (data_arr != 0);
   assert (nbr_elt > 0);

   const bool     can_push_flag = (get_nbr_free_elt () >= nbr_elt);
   if (can_push_flag)
   {
      long           pos = 0;
      do
      {
         const long     work_len =
            ohm::lang::min (nbr_elt - pos, _size - _write_pos);
         const long     stop_pos = pos + work_len;
         long           write_pos = _write_pos;
         do
         {
            _buf_ptr [write_pos] = data_arr [pos];
            ++ write_pos;
            ++ pos;
         }
         while (pos < stop_pos);

         const conc::AioAddMod <PosType>  ftor_add (work_len, _size);
         conc::AtomicIntOp_exec (_write_pos, ftor_add);
      }
      while (pos < nbr_elt);
   }

   return (can_push_flag);
}



template <class T>
T *   LockFreeRingFifo<T>::use_data_block_write (long &max_nbr_elt)
{
   max_nbr_elt = get_nbr_free_elt ();
   if (_read_pos <= _write_pos)
   {
      max_nbr_elt = lang::min (max_nbr_elt, _size - _write_pos);
   }

   return (&_buf_ptr [_write_pos]);
}



template <class T>
bool  LockFreeRingFifo<T>::validate (long nbr_elt)
{
   assert (nbr_elt >= 0);
   const bool     can_validate_flag = (get_nbr_free_elt () >= nbr_elt);

   if (can_validate_flag)
   {
      const conc::AioAddMod <PosType>  ftor_add (nbr_elt, _size);
      conc::AtomicIntOp_exec (_write_pos, ftor_add);
   }

   return (can_validate_flag);
}



/*==========================================================================*/
/*      Name: is_empty                                                      */
/*      Description: Check if the FIFO is empty (no stored elements)        */
/*                   Should be called by READ thread only.                  */
/*      Returns: true if the FIFO is empty.                                 */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::is_empty () const
{
   return (_read_pos == _write_pos);
}



/*==========================================================================*/
/*      Name: get_nbr_stored_elt                                            */
/*      Description: Returns the number of objects currently stored in the  */
/*                   FIFO.                                                  */
/*                   Should be called by READ thread only.                  */
/*      Returns: The number of stored objects.                              */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
long  LockFreeRingFifo<T>::get_nbr_stored_elt () const
{
   return ((_write_pos + _size - _read_pos) % _size);
}



/*==========================================================================*/
/*      Name: pop                                                           */
/*      Description: Remove and copy the top object from the FIFO, if it is */
/*                   not empty.                                             */
/*                   Should be called by READ thread only.                  */
/*      Output parameters:                                                  */
/*        - data : The object to read. If the FIFO is empty, data is not    */
/*                 modified.                                                */
/*      Returns: true if an object had been successfully read on the FIFO.  */
/*      Throws: ?                                                           */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::pop (T &data)
{
   const bool        can_pop_flag = ! is_empty ();
   if (can_pop_flag)
   {
      data = _buf_ptr [_read_pos];
      const conc::AioAddMod <PosType>  ftor_inc (1, _size);
      conc::AtomicIntOp_exec (_read_pos, ftor_inc);
   }

   return (can_pop_flag);
}



/*==========================================================================*/
/*      Name: pop_array                                                     */
/*      Description: Read a bunch of object on the FIFO and remove them     */
/*                   from it. The operation is done only if the FIFO        */
/*                   contains at least the requested number of elements.    */
/*                   Should be called by READ thread only.                  */
/*      Output parameters:                                                  */
/*        - data_arr : Pointer on the array receiving the objects. The      */
/*                     array is not altered if the operation can be done.   */
/*      Input parameters:                                                   */
/*        - nbr_elt : Number of elements to remove from the FIFO. > 0       */
/*      Returns: true if the elements had been all copied and removed       */
/*               successfully.                                              */
/*      Throws: ?                                                           */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::pop_array (T data_arr [], long nbr_elt)
{
   assert (nbr_elt > 0);

   const bool  can_pop_flag = (get_nbr_stored_elt () >= nbr_elt);
   if (can_pop_flag)
   {
      long           pos = 0;
      do
      {
         const long     work_len =
            ohm::lang::min (nbr_elt - pos, _size - _read_pos);
         const long     stop_pos = pos + work_len;
         long           read_pos = _read_pos;
         do
         {
            data_arr [pos] = _buf_ptr [read_pos];
            ++ read_pos;
            ++ pos;
         }
         while (pos < stop_pos);

         const conc::AioAddMod <PosType>  ftor_add (work_len, _size);
         conc::AtomicIntOp_exec (_read_pos, ftor_add);
      }
      while (pos < nbr_elt);
   }

   return (can_pop_flag);
}



/*==========================================================================*/
/*      Name: use_data_block_read                                           */
/*      Description: Provide an access to the top elements of the FIFO, if  */
/*                   not empty. Not all the stored elements can be accessed */
/*                   by calling this function, because of the ring          */
/*                   structure. If you want to access all the FIFO, you     */
/*                   need to call use_data_block () + discard () until      */
/*                   is_empty ().                                           */
/*                   Should be called by READ thread only.                  */
/*      Output parameters:                                                  */
/*        - nbr_elt : number of valid elements on the array. Can be 0 if    */
/*                    the FIFO is empty.                                    */
/*      Returns: a pointer to the array of elements.                        */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
const T *   LockFreeRingFifo<T>::use_data_block_read (long &nbr_elt) const
{
   if (_read_pos <= _write_pos)
   {
      nbr_elt = _write_pos - _read_pos;
   }
   else
   {
      nbr_elt = _size - _read_pos;
   }

   return (&_buf_ptr [_read_pos]);
}



/*==========================================================================*/
/*      Name: discard                                                       */
/*      Description: Delete the top elements on the FIFO. If the FIFO       */
/*                   contains less than the number of requested elements,   */
/*                   no deletion is done. Equivalent to pop_array without   */
/*                   copying the elements.                                  */
/*                   Should be called by READ thread only.                  */
/*      Input parameters:                                                   */
/*        - nbr_elt : Number of elements to delete.                         */
/*      Returns: true if all the requested elements have been deleted.      */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

template <class T>
bool  LockFreeRingFifo<T>::discard (long nbr_elt)
{
   assert (nbr_elt >= 0);
   const bool  can_discard_flag = (get_nbr_stored_elt () >= nbr_elt);

   if (can_discard_flag)
   {
      const conc::AioAddMod <PosType>  ftor_add (nbr_elt, _size);
      conc::AtomicIntOp_exec (_read_pos, ftor_add);
   }

   return (can_discard_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_LockFreeRingFifo_CODEHEADER_INCLUDED

#undef ohm_mem_LockFreeRingFifo_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
