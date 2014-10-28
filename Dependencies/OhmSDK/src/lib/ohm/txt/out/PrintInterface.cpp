/*****************************************************************************

        PrintInterface.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 57748

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/txt/out/ArgFloat.h"
#include "ohm/txt/out/ArgInt.h"
#include "ohm/txt/out/ArgString.h"
#include "ohm/txt/out/FmtInfo.h"
#include "ohm/txt/out/PrintInterface.h"

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
   #include <cxxabi.h>
#endif

#include <cassert>
#include <cctype>
#include <climits>
#include <cstring>



namespace std {}



namespace ohm
{
namespace txt
{
namespace out
{



PrintInterface::PrintInterface ()
:  _format_0 (0)
,  _nbr_arg_expected (0)
,  _nbr_arg_provided (0)
,  _cat_flag (false)
,  _print_beg_flag (false)
{
   // Nothing
}



void  PrintInterface::reset ()
{
   _format_0 = 0;
   _nbr_arg_expected = 0;
   _nbr_arg_provided = 0;
   _cat_flag = false;
   _print_beg_flag = false;

   do_reset ();
}



bool  PrintInterface::is_truncated () const
{
   return (do_is_truncated ());
}



/*
==============================================================================
Name: get_string_len
Description:
   Gets the theoretical string len (without any truncation done by the
   implementation)
Returns:
   The length, >= 0.
Throws: Nothing
==============================================================================
*/

long  PrintInterface::get_string_len () const
{
   const long     len = do_get_string_len ();
   assert (len >= 0);

   return (len);
}



PrintInterface &  PrintInterface::print (const char *format_0)
{
   assert (format_0 != 0);

   reset ();
   _cat_flag = false;
   print_common (format_0);

   return (*this);
}



PrintInterface &  PrintInterface::cat (const char *format_0)
{
   assert (format_0 != 0);

   _cat_flag = true;
   print_common (format_0);

   return (*this);
}



PrintInterface &  PrintInterface::arg (const IntType &val, long param_1, long param_2, long param_3, long param_4)
{
   arg_generic (
      reinterpret_cast <const void *> (&val), sizeof (val),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const long &val, long param_1, long param_2, long param_3, long param_4)
{
   IntType        val_2 (val);

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const unsigned long &val, long param_1, long param_2, long param_3, long param_4)
{
   IntType        val_2 (val);

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const int &val, long param_1, long param_2, long param_3, long param_4)
{
   IntType        val_2 (val);

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const unsigned int &val, long param_1, long param_2, long param_3, long param_4)
{
   IntType        val_2 (val);

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const archi::UInt64 &val, long param_1, long param_2, long param_3, long param_4)
{
   ohm_lang_CHECK_CST (PtrSizetSize, (sizeof (IntType) >= sizeof (val)));

   IntType        val_2 (val);

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const void * val, long param_1, long param_2, long param_3, long param_4)
{
   ohm_lang_CHECK_CST (PtrSize, (sizeof (IntType) >= sizeof (const void *)));

   IntType        val_2 (reinterpret_cast <IntType> (val));

   // Sets the upper bits to 0, so the addresses are always positive if the
   // integer type is bigger than the address type.
   // The shift is 2<<(N-1) instead of 1<<N in order to avoid the compiler
   // complain about too big shifts.
   val_2 &= (IntType (2) << (sizeof (const void *) * CHAR_BIT - 1)) - 1;

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_INT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const FloatType &val, long param_1, long param_2, long param_3, long param_4)
{
   arg_generic (
      reinterpret_cast <const void *> (&val), sizeof (val),
      ArgInfo::Type_FLOAT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const double &val, long param_1, long param_2, long param_3, long param_4)
{
   FloatType      val_2 (val);

   arg_generic (
      reinterpret_cast <const void *> (&val_2), sizeof (val_2),
      ArgInfo::Type_FLOAT,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



PrintInterface &  PrintInterface::arg (const StringType &text_0, long param_1, long param_2, long param_3, long param_4)
{
   if (text_0 == 0)
   {
      const char * null_text_0 = "";
      
      arg_generic (
         reinterpret_cast <const void *> (&null_text_0), sizeof (null_text_0),
         ArgInfo::Type_STRING,
         param_1, param_2, param_3, param_4
      );
   }

   else
   {
      arg_generic (
         reinterpret_cast <const void *> (&text_0), sizeof (text_0),
         ArgInfo::Type_STRING,
         param_1, param_2, param_3, param_4
      );
   }

   return (*this);
}



PrintInterface &  PrintInterface::arg (const TypeInfoType &type_info, long param_1, long param_2, long param_3, long param_4)
{
   const TypeInfoType * type_info_ptr = &type_info;
   
   arg_generic (
      reinterpret_cast <const void *> (&type_info_ptr), sizeof (type_info_ptr),
      ArgInfo::Type_TYPE_INFO,
      param_1, param_2, param_3, param_4
   );

   return (*this);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  PrintInterface::arg_generic (const void *data_ptr, long data_size, ArgInfo::Type type, long param_1, long param_2, long param_3, long param_4)
{
   using namespace std;
   
   if (_nbr_arg_provided < MAX_ARG)
   {
      ArgInfo &      arg = _arg_array [_nbr_arg_provided];
      assert (data_size <= long (sizeof (arg._data)));

      arg._type = type;
      arg._param [0] = param_1;
      arg._param [1] = param_2;
      arg._param [2] = param_3;
      arg._param [3] = param_4;
      memcpy (arg._data, data_ptr, data_size);
      ++_nbr_arg_provided;

      write_if_complete ();
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  PrintInterface::print_common (const char *format_0)
{
   assert (format_0 != 0);

   _format_0 = format_0;
   _nbr_arg_provided = 0;
   const int      higher_arg = find_higher_arg_in_format_string ();
   if (   higher_arg < 0
       || higher_arg > MAX_ARG)
   {
      reset ();
   }
   else
   {
      _nbr_arg_expected = higher_arg;
   }

   write_if_complete ();
}



// Also check the basic syntax.
// Returns 0-N, or -1 is syntax error
int   PrintInterface::find_higher_arg_in_format_string ()
{
   return (scan_format_string (false));
}



int   PrintInterface::scan_format_string (bool write_flag)
{
   using namespace std;

   assert (_format_0 != 0);

   int            higher_arg = 0;
   long           index = 0;
   long           desc_index = 0;
   AnalysisState  state = AnalysisState_SCAN;
   int            cur_arg = 0;

   while (_format_0 [index] != '\0')
   {
      const int      c = _format_0 [index];

      switch (state)
      {
      case  AnalysisState_SCAN:
         if (c == '%')
         {
            state = AnalysisState_ARG_1ST_CHAR;
         }
         else if (write_flag)
         {
            print_char (c);
         }
         break;

      case  AnalysisState_ARG_1ST_CHAR:
         if (isdigit (c))
         {
            cur_arg = c - '0';
            state = AnalysisState_ARG_NBR;
         }
         else
         {
            if (write_flag)
            {
               if (c != '%')
               {
                  print_char ('%');
               }
               print_char (c);
            }
            state = AnalysisState_SCAN;
         }
         break;

      case  AnalysisState_ARG_NBR:
         if (isdigit (c))
         {
            cur_arg *= 10;
            cur_arg += c - '0';
         }
         else
         {
            higher_arg = lang::max (higher_arg, cur_arg);
            if (c == '%')
            {
               if (write_flag)
               {
                  write_arg (cur_arg - 1, index, 0);
               }
               state = AnalysisState_SCAN;
            }
            else
            {
               state = AnalysisState_ARG_DESC;
               desc_index = index;
            }
         }
         break;

      case  AnalysisState_ARG_DESC:
         if (c == '%')
         {
            if (write_flag)
            {
               write_arg (cur_arg - 1, desc_index, index - desc_index);
            }
            state = AnalysisState_SCAN;
         }        
         break;
      }

      ++ index;
   }

   // Check if last argument is complete
   if (state != AnalysisState_SCAN)
   {
      higher_arg = -1;
   }

   return (higher_arg);
}



void  PrintInterface::write_if_complete ()
{
   if (   _format_0 != 0
       && _nbr_arg_provided == _nbr_arg_expected)
   {
      if (! _print_beg_flag || ! _cat_flag)
      {
         print_begin ();
         _print_beg_flag = true;
      }
      scan_format_string (true);
      print_end ();
   }
}



void  PrintInterface::write_arg (int arg_nbr, long fmt_pos, long fmt_size)
{
   assert (arg_nbr >= 0);
   assert (arg_nbr < _nbr_arg_provided);

   const ArgInfo &   arg = _arg_array [arg_nbr];

   // Parse argument format
   FmtInfo  fmt;
   const int      ret_val = fmt.parse_options (
      _format_0 + fmt_pos,
      fmt_size,
      arg._param,
      ArgInfo::MAX_PARAM
   );

   // Write formated argument
   if (ret_val == 0)
   {
      switch (arg._type)
      {
      case  ArgInfo::Type_INT:
         {
            ArgInt   printer (*this, fmt);
            printer.print (*reinterpret_cast <const IntType *> (arg._data));
         }
         break;

      case  ArgInfo::Type_FLOAT:
         {
            ArgFloat printer (*this, fmt);
            printer.print (*reinterpret_cast <const FloatType *> (arg._data));
         }
         break;

      case  ArgInfo::Type_STRING:
         {
            ArgString   printer (*this, fmt);
            printer.print (*reinterpret_cast <const StringType *> (arg._data));
         }
         break;

      case  ArgInfo::Type_TYPE_INFO:
         {
            ArgString   printer (*this, fmt);

            const TypeInfoType * type_info_ptr
               = *reinterpret_cast <TypeInfoType * const *> (arg._data);
            
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
            const size_t buf_size = 8192;

            char buf_0 [buf_size];
            size_t length = buf_size;
            int status = 0;
            __cxxabiv1::__cxa_demangle (type_info_ptr->name (), buf_0, &length, &status);
            // assume the buffer is long enough : we cannot call assert
            
            const char * name_0 = (status == 0) ? buf_0 : type_info_ptr->name ();

#else // ohm_archi_COMPILER_CLASS
            const char * name_0 = type_info_ptr->name ();

#endif // ohm_archi_COMPILER_CLASS  

            printer.print (name_0);
         }
         break;
      }
   }
}



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
