/*****************************************************************************

        WidgetListBase.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57590

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



#if ! defined (ohm_opa_WidgetListBase_HEADER_INCLUDED)
#define  ohm_opa_WidgetListBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"

#include <list>



namespace ohm
{
namespace opa
{



class WidgetListBase
:  public virtual WidgetBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   class Element
   {
   public:
      enum State
      {
                  State_ADDED = 0,
                  State_REMOVED,
                  State_RESIDENT,
      };
      
      WidgetSPtr  _widget_sptr;
      State       _state;
   };
   
   typedef std::list <Element>   ElementArr;
   
public:
   
   class iterator
   {
   public:
                  iterator ();
                  iterator (const iterator & other);
      virtual     ~iterator ();
      
      iterator &  operator = (const iterator & other);
      WidgetBase &
                  operator * ();
      WidgetBase *
                  operator -> ();
      iterator &  operator ++ ();
      iterator    operator ++ (int);
      iterator &  operator -- ();
      iterator    operator -- (int);
      bool        operator == (const iterator & other) const;
      bool        operator != (const iterator & other) const;
      
      bool        was_added () const;
   
   private:
      friend class WidgetListBase;
                  iterator (std::list <Element>::iterator it, std::list <Element>::iterator it_end);
      std::list <Element>::iterator
                  _it;
      std::list <Element>::iterator
                  _it_end;
   };

                  WidgetListBase ();
   virtual        ~WidgetListBase ();
   
   bool           empty () const;
   size_t         size () const;
   
   iterator       begin ();
   iterator       end ();

   template <class T>
   mem::SharedPtr <T>
                  push ();
   template <class T>
   void           push (mem::SharedPtr <T> widget_sptr);
   template <class T>
   void           remove (mem::SharedPtr <T> widget_sptr);
   template <class T>
   iterator       insert (iterator it, mem::SharedPtr <T> widget_sptr);
   void           erase (iterator it);
   void           erase (iterator it_first, iterator it_last);
   void           clear ();

   template <class T>
   iterator       find (mem::SharedPtr <T> widget_sptr);
   
   template <class T>
   void           sort ();
   
   // internal
   void           synchronize ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   template <class T>
   class Comparator
   {
   public:
      bool        operator () (const Element & l_op, const Element & r_op);
   };
   
   void           do_erase (iterator it);

   bool           _need_sync_flag;
   ElementArr     _element_arr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetListBase (const WidgetListBase &other);
   WidgetListBase &
                  operator = (const WidgetListBase &other);
   bool           operator == (const WidgetListBase &other);
   bool           operator != (const WidgetListBase &other);

}; // class WidgetListBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetListBase.hpp"



#endif   // ohm_opa_WidgetListBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
