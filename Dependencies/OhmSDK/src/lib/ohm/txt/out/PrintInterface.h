/*****************************************************************************

        PrintInterface.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 43632

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



#if ! defined (ohm_txt_out_PrintInterface_HEADER_INCLUDED)
#define  ohm_txt_out_PrintInterface_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/txt/out/DevInterface.h"

#include <typeinfo>



namespace ohm
{
namespace txt
{
namespace out
{



class PrintInterface
:  public DevInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         MAX_ARG  = 64  };

   typedef  archi::Int64   IntType;
   typedef  long double    FloatType;
   typedef  const char *   StringType;
   typedef  std::type_info TypeInfoType;

                  PrintInterface ();
   virtual        ~PrintInterface () {}

   void           reset ();
   bool           is_truncated () const;
   long           get_string_len () const;

   PrintInterface &
                  print (const char *format_0);
   PrintInterface &
                  cat (const char *format_0);

   PrintInterface &
                  arg (const IntType &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const long &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const unsigned long &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const int &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const unsigned int &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const archi::UInt64 &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const void * val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const FloatType &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const double &val, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const StringType &text_0, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);
   PrintInterface &
                  arg (const TypeInfoType &type_info, long param_1 = 0, long param_2 = 0, long param_3 = 0, long param_4 = 0);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   class ArgInfo
   {
   public:
      enum {         MAX_PARAM      = 4   };
      enum {         MAX_DATA_LEN   = 16  };
      enum Type
      {
         Type_INT = 0,
         Type_FLOAT,
         Type_STRING,
         Type_TYPE_INFO,
      };

      Type           _type;
      char           _data [MAX_DATA_LEN];
      long           _param [MAX_PARAM];
   };

   void           arg_generic (const void *data_ptr, long data_size, ArgInfo::Type type, long param_1, long param_2, long param_3, long param_4);

   virtual void   do_reset () = 0;
   virtual bool   do_is_truncated () const = 0;
   virtual long   do_get_string_len () const = 0;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum AnalysisState
   {
      AnalysisState_SCAN = 0,
      AnalysisState_ARG_1ST_CHAR,
      AnalysisState_ARG_NBR,
      AnalysisState_ARG_DESC
   };

   void           print_common (const char *format_0);
   int            find_higher_arg_in_format_string ();
   int            scan_format_string (bool write_flag);
   void           write_if_complete ();
   void           write_arg (int arg_nbr, long fmt_pos, long fmt_size);

   const char *   _format_0;           // Format string
   long           _nbr_arg_expected;   // Number of arguments seen in the format string. Actually the higher argument index number.
   long           _nbr_arg_provided;   // Current number of provided arguments
   ArgInfo        _arg_array [MAX_ARG];
   bool           _cat_flag;           // If we just add text to existing one
   bool           _print_beg_flag;     // Indicates if print_begin() has been called at least once



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   PrintInterface &
                  operator = (const PrintInterface &other);
   bool           operator == (const PrintInterface &other);
   bool           operator != (const PrintInterface &other);

}; // class PrintInterface



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/PrintInterface.hpp"



#endif   // ohm_txt_out_PrintInterface_HEADER_INCLUDED



