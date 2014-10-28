/*****************************************************************************

        Debugger.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 21027

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



#if ! defined (ohm_odbg_Debugger_HEADER_INCLUDED)
#define  ohm_odbg_Debugger_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace odbg
{



class Debugger
{
public:

   // Interface

   typedef  archi::Int32 (ohm_archi_CDECL * FncPtr) (archi::Int32 command, void *data_ptr);

   // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

   // Error codes
   enum Err
   {
      Err_OK = 0,

   };
   
   enum Level
   {
      Level_VOID = 0,
      Level_PANIC = 1,
      Level_ERROR,
      Level_WARNING,
      Level_INFO,
      Level_DEBUG,
      Level_ASSERT,
   };



   // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
   // Commands from host to plugin

   class CmdPlugIn
   {
   public:

      enum
      {
         INIT = 1,
      };

      // Argument structures

      class InitParam
      {
      public:
         FncPtr      _debugger_fnc_ptr;
      };

      
   };



   // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
   // Commands from plug-in to host

   class CmdHost
   {
   public:

      enum
      {
         WRITE_LOG = 1,
      };

      // Argument structures

      class WriteLogParam
      {
      public:
         const char *   _ns_0;
         archi::Int32   _level;
         const char *   _msg_0;
         const char *   _scope_0;
         const char *   _func_0;
         archi::Int32   _line_nbr;
         archi::Int32   _fast_flag;
      };
   };
};



}  // namespace odbg
}  // namespace ohm



#endif   // ohm_odbg_Debugger_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
