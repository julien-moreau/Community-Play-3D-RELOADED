/*****************************************************************************

        FloatMultiPrecPrint.hpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 42523

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



#if ! defined (ohm_math_FloatMultiPrecPrint_CODEHEADER_INCLUDED)
#define  ohm_math_FloatMultiPrecPrint_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/math/bit.h"
#include "ohm/math/BitMask.h"
#include "ohm/math/fnc.h"

#include <cassert>
#include <cfloat>



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
FloatMultiPrecPrint <NW>::FloatMultiPrecPrint ()
:  _v ()
,  _high ()
,  _low ()
,  _qi ()
,  _base_k ()
,  _recomposed ()
,  _recomp_div ()
,  _req_resol (0)
,  _base (10)
,  _k (0)
,  _cur_dig (-1)
,  _nbr_dig (-1)
,  _stop_next_flag (false)
{
   // Nothing
}



template <int NW>
void  FloatMultiPrecPrint <NW>::set_nbr (const FloatType &val, int base, int req_resol)
{
   assert (&val != 0);
   assert (val > 0);
   assert (base > 0);
   assert (req_resol > 0);

   _v = val;
   _req_resol = req_resol;
   _base = base;

   prepare ();
}



template <int NW>
void  FloatMultiPrecPrint <NW>::set_nbr (double val, int base)
{
   assert (val > 0);
   assert (base > 0);

   _v.from_dbl (val);
   _req_resol = DBL_MANT_DIG;
   _base = base;

   const archi::UInt64  val_int =
      *reinterpret_cast <const archi::UInt64 *> (&val);
   const int      exp = int ((val_int >> (DBL_MANT_DIG - 1)) & 0x7FF);
   assert (exp != 0x7FF);
   if (exp == 0)
   {
      const archi::UInt64  mask =
         BitMask <archi::UInt64>::make (DBL_MANT_DIG - 1);
      const archi::UInt64  mant = val_int & mask;
      _req_resol = get_prev_pow2 (mant) + 1;
   }

   prepare ();
}



template <int NW>
void  FloatMultiPrecPrint <NW>::set_nbr (float val, int base)
{
   assert (val > 0);
   assert (base > 0);

   _v.from_dbl (val);
   _req_resol = FLT_MANT_DIG;
   _base = base;

   const archi::UInt32  val_int =
      *reinterpret_cast <const archi::UInt32 *> (&val);
   const int      exp = (val_int >> (FLT_MANT_DIG - 1)) & 0xFF;
   assert (exp != 0xFF);
   if (exp == 0)
   {
      const archi::UInt32  mask =
         BitMask <archi::UInt32>::make (FLT_MANT_DIG - 1);
      const archi::UInt32  mant = val_int & mask;
      _req_resol = get_prev_pow2 (mant) + 1;
   }

   prepare ();
}



// Number = [d0 . d1 d2 d3...] * (base ^ exp)
template <int NW>
int   FloatMultiPrecPrint <NW>::get_exp () const
{
   return (_k - 1);
}



template <int NW>
void  FloatMultiPrecPrint <NW>::start ()
{
   _cur_dig = 0;
   _nbr_dig = 0;
   _qi = _v / _base_k;
   _recomposed.clear ();
   _recomp_div = 1;
   _stop_next_flag = false;

   iterate_internal ();
}



template <int NW>
bool  FloatMultiPrecPrint <NW>::is_rem_elt () const
{
   return (_nbr_dig >= 0);
}



template <int NW>
void  FloatMultiPrecPrint <NW>::iterate ()
{
   assert (is_rem_elt ());

   if (_stop_next_flag)
   {
      _nbr_dig = -1;
   }

   // Iteration
   else
   {
      iterate_internal ();
   }
}



template <int NW>
int   FloatMultiPrecPrint <NW>::get_digit () const
{
   assert (is_rem_elt ());

   return (_cur_dig);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
void  FloatMultiPrecPrint <NW>::prepare ()
{
   // Computes _high and _low.
   FloatType      v_m (_v);
   FloatType      v_p (_v);

   const int      v_l2 = _v.compute_log2_int ();
   FloatType      tmp;
   tmp.exp2_int (v_l2);

   const int      e = v_l2 - _req_resol + 1;
   FloatType      be;
   be.exp2_int (e);

   if (_req_resol == 1 && _v == tmp)
   {
      FloatType      bem1 (be);
      bem1.scale2_int (-1);
      v_m -= bem1;
   }
   else
   {
      v_m -= be;
   }

   v_p += be;

   _low = v_m + _v;
   _low.scale2_int (-1);

   _high = _v + v_p;
   _high.scale2_int (-1);

   assert (_low < _v);
   assert (_v < _high);

   // Computes k
   FloatType      k_flt (_high);
   k_flt.log2 ();
   const FloatType   base_flt (_base);
   FloatType      base_l2 (base_flt);
   base_l2.log2 ();
   k_flt /= base_l2;
   k_flt.ceil ();

   _k = round_int (k_flt.to_dbl ());
   _base_k = ipow (base_flt, _k);
   assert (_high <= _base_k);
}



template <int NW>
void  FloatMultiPrecPrint <NW>::iterate_internal ()
{
   assert (! _stop_next_flag);

   ++ _nbr_dig;

   _qi *= _base;
   FloatType   qi_base_floor (_qi);
   qi_base_floor.floor ();
   _cur_dig = round_int (qi_base_floor.to_dbl ());

   // Inserts the new digit into the recomposed number
   const FloatType   cur_dig_hp (_cur_dig);
   _recomp_div *= _base;
   _recomposed *= _base;
   _recomposed += cur_dig_hp;
   const FloatType   recomp_mul (_base_k / _recomp_div);
   const FloatType   recomp_dn (_recomposed * recomp_mul);
   FloatType         recomp_up (recomp_dn + recomp_mul); // digit + 1

   // test_2_flag may wrongly success because of rounding errors, when _high
   // has an exact representation in the display base. Therefore, we try to
   // round the recomp_up mantissa to be sure. Note that this rounding makes
   // sense, because _high has a much more rounded mantissa. Indeed, it comes
   // from the mantissa of the original number to be displayed, which has
   // a much smaller precision than the floating point used for these
   // calculations.
   recomp_up.round_first_mantissa_bits (_req_resol + 8);

   // Check if this is the last one.
   const bool     test_1_flag = (recomp_dn > _low);
   const bool     test_2_flag = (recomp_up < _high);

   if (test_2_flag)
   {
      // Last digit may require to be rounded up.
      bool           up_flag = true;

      if (test_1_flag)
      {
         // lang:: required because otherwise std::abs is used and the number
         // is converted to double.
         const FloatType   d_lo (lang::abs (recomp_dn - _v));
         const FloatType   d_hi (lang::abs (recomp_up - _v));
         if (d_lo < d_hi)
         {
            up_flag = false;
         }
      }

      if (up_flag)
      {
         ++ _cur_dig;
         assert (_cur_dig < _base);
      }

      _stop_next_flag = true;
   }

   else if (test_1_flag)
   {
      _stop_next_flag = true;
   }

   else
   {
      _qi -= cur_dig_hp;
   }
}



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_FloatMultiPrecPrint_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
