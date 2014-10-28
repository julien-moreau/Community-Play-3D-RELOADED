/*****************************************************************************

        Monitor.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70583

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



#if ! defined (ohm_mon_Monitor_HEADER_INCLUDED)
#define  ohm_mon_Monitor_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  NOMINMAX
   #include <windows.h>
#endif


#include "ohm/mon/MsgRingBuffer.h"

#include "ohm/archi/types.h"
#include "ohm/util/SingletonBase.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include <DbgHelp.h>
#endif


#include <string>



namespace ohm
{
namespace txt
{
namespace out
{
class PrintTubeEscaper;
}
}
namespace mon
{



class MonitorImpl;

class Monitor
:  public util::SingletonBase <Monitor>
{

   friend class MonitorImpl;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef archi::UInt64   CounterType;
   
   virtual        ~Monitor () {}
   
   static unsigned int
                  thread_self ();
   static CounterType
                  counter ();
   static double  to_time (CounterType counter);
   
   void           add_rule (const std::string & search, const std::string & replace);
   void           add_rule (const std::string & condition, const std::string & search, const std::string & replace);
   
   void           write_na (unsigned int thread, CounterType timestamp, const char * pretty_func_0, const char * msg_0);
   void           write (unsigned int thread, CounterType timestamp, const char * pretty_func_0, const char * output_0);
   
   void           suspend_threads ();
   void           resume_threads ();
   void           trace_backtrace ();
   void           trace_backtrace (const char * str_0, bool prefix_flag = true, bool offset_flag = true);
   void           trace_dump ();

   void           print_dump_xml (txt::out::PrintTubeEscaper & printer);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  Monitor ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm_util_SingletonBase_INVITE_SINGLETON (Monitor)

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   typedef DWORD (__stdcall *
                  PtrFncUnDecorateSymbolName) (
                     __in PCSTR name,
                     __out_ecount(maxStringLength) PSTR outputString,    
                     __in DWORD maxStringLength,
                     __in DWORD flags
                  );
#endif

   class Rule
   {
   public:
      std::string _condition;
      std::string _search;
      std::string _replace;
   };
   
   typedef std::vector <Rule> RuleArr;

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   void           undecorate (std::string & func);
   void           load_map_dgbhelp ();
#endif
   
   void           process_rules (std::string & output) const;
   void           process_rule (std::string & output, const Rule & rule) const;

   RuleArr        _rule_arr;
   MsgRingBuffer  _msg_ring_buffer;

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   ::HMODULE      _dbghelp_hnd;
   static PtrFncUnDecorateSymbolName
                  _fnc_UnDecorateSymbolName;
   static archi::Int64
                  _pc_freq;
#endif



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Monitor (const Monitor &other);
   Monitor &      operator = (const Monitor &other);
   bool           operator == (const Monitor &other);
   bool           operator != (const Monitor &other);

}; // class Monitor



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace mon
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/mon/Monitor.hpp"



#endif   // ohm_mon_Monitor_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
