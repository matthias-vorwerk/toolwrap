/*
 *      Copyright (C) 2004,2004  bmonthy@users.sourceforge.net
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 *
 *
 *  $Id: list.c,v 1.3 2005/03/15 18:38:05 bmonthy Exp $
 */
#include "list.h"

list_t*
list_prepend(list_t* me, void* elem)
{
  list_t* n;
 
  assert_ptr(elem);
   
  n = (list_t*) malloc (sizeof(list_t));
  if (!n) return NULL;
  n->next = me; 
  n->data = elem;
  return n;
}

list_t*
list_append(list_t* me, void* elem)
{
  list_t* n, *ptr; 
 
  assert_ptr(elem);
  
  if (!me) return list_prepend(me, elem);
  
  n = (list_t*) malloc (sizeof(list_t));
  n->data = elem;
  n->next = NULL;
  
  ptr = me;   
  while (ptr->next) ptr=ptr->next;
  ptr->next = n;
	
  return me;  
}

list_t*
list_unlink(list_t* me, void* elem)
{
  list_t* ptr;
  list_t* prev;
  
  assert_ptr(me);
  assert_ptr(elem);
  
  ptr=me; 
  prev=NULL;
  
  while (ptr)
  {
    if (ptr->data == elem)
    {
      if (!prev)
      {
        list_t* cur;
        cur = ptr;
        free(ptr);
        return cur->next;  
      }
      
      prev->next = ptr->next;
      free(ptr);
      return me;
    }
    
    prev=ptr;
    ptr = ptr->next;
  }
  
#ifdef _DEBUG
  log_msg(LOG_DEBUG, " ** warning: element %x not found in list %x", elem, me);  
#endif
    
  return me;
}

