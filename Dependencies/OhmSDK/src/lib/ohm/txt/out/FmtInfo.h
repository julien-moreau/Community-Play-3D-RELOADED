/*****************************************************************************

        FmtInfo.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30475

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



#if ! defined (ohm_txt_out_FmtInfo_HEADER_INCLUDED)
#define  ohm_txt_out_FmtInfo_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace txt
{
namespace out
{



class FmtInfo
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum JustifyMode
   {
      JustifyMode_LEFT = 0,
      JustifyMode_RIGHT,
      JustifyMode_CENTER,
      JustifyMode_LEFT_PAD
   };

   enum FloatMode
   {
      FloatMode_AUTO = 0,
      FloatMode_NORM,
      FloatMode_SCI
   };

   enum CaseMode
   {
      CaseMode_NONE = 0,
      CaseMode_UPPER,
      CaseMode_LOWER
   };

   enum SignMode
   {
      SignMode_NONE = 0,
      SignMode_MINUS,
      SignMode_ALWAYS
   };

                  FmtInfo ();
   virtual        ~FmtInfo () {}

   int            parse_options (const char *opt_ptr, long opt_len, const long param_ptr [], long max_param);

   long           get_max_len () const;
   bool           has_max_len () const;
   int            get_base () const;
   int            get_nbr_decimals () const;
   int            get_group_size () const;
   int            get_exp_size () const;
   JustifyMode    get_justify_mode () const;
   FloatMode      get_float_mode () const;
   CaseMode       get_case_mode () const;
   SignMode       get_sign_mode () const;
   char           get_whitespace () const;
   bool           has_zero_trail () const;
   bool           is_truncated () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   struct ParserState
   {
                     ParserState (const char *opt_ptr, long opt_len, const long param_ptr [], long max_param);

      const char * const
                     _opt_ptr;
      const long     _opt_len;
      const long * const
                     _param_ptr;
      const long     _max_param;

      int            _ret_val;
      int            _cur_param;
      long           _scan_pos;
   };

   void           parse_max_length_option (ParserState &state);
   void           parse_whitespace_option (ParserState &state);
   void           parse_nbr_decimals_option (ParserState &state);
   void           parse_group_size_option (ParserState &state);
   void           parse_fp_disp_mode_option (ParserState &state, FloatMode type);

   bool           is_parameter (const ParserState &state) const;
   long           parse_parameter (ParserState &state);
   long           parse_number (ParserState &state);

   long           _max_len;         // Default = max
   int            _base;            // Default = 10
   int            _nbr_decimals;    // Default = 6
   int            _group_size;      // Default = 0
   int            _exp_size;        // Default = 0
   JustifyMode    _justify_mode;    // Default = left
   FloatMode      _float_mode;      // Default = auto
   CaseMode       _case_mode;       // Default = none
   SignMode       _sign_mode;       // Default = none
   char           _whitespace;      // Default = ' '
   bool           _zero_trail_flag; // Default = false
   bool           _truncate_flag;   // Default = false



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FmtInfo (const FmtInfo &other);
   FmtInfo &      operator = (const FmtInfo &other);
   bool           operator == (const FmtInfo &other);
   bool           operator != (const FmtInfo &other);

}; // class FmtInfo



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/FmtInfo.hpp"



#endif   // ohm_txt_out_FmtInfo_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
