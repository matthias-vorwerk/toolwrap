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
 *  $Id: hash.c,v 1.5 2005/04/26 12:56:57 bmonthy Exp $
 */
#include "hash.h"

/* hash.c : simple functions to manage hash tables */


/* new hash table. comparaison & hash function given in parameters */

hash_t* 
hash_new( int(*cmp_func)(const void*,const void*), unsigned int(*hash_func)(const void*))
{
	hash_t* me;

	
	
	me = (hash_t*) malloc (sizeof(hash_t));
	memset(me, 0, sizeof(hash_t));
	me->cmp_func = cmp_func;
	me->hash_func = hash_func;
	
	return me;
}


/* append an element in the table */
void
hash_append(hash_t* me, const void* key, void* data)
{
	unsigned int h;
	list_t* li, *cur;
	hash_elem_t* elem;
		
	assert_ptr(me);
	assert_ptr(key);
	assert_ptr(data);
	
	h = me->hash_func(key);
	
	assert ((h <= 255) && (h>=0));
	
	li = me->idx[h];
	
	if (li)
	for (cur=li; cur; cur=cur->next)
	{
		if (me->cmp_func(((hash_elem_t*) cur->data)->key,key) == 0)
		{
				((hash_elem_t*)cur->data)->data = data;
				return;
		}
	}	
	elem = (hash_elem_t*) malloc (sizeof(hash_elem_t));
	elem->key=key;
	elem->data=data;	
	li = list_prepend(li, elem);	
	me->idx[h]=li;
}


/* searc the table for the given key. returns NULL if no match is found */

const void*
hash_lookup(hash_t* me, const void* key)
{
	list_t* li;
	unsigned int h;
	
	assert_ptr(me);
	assert_ptr(key);
	
	h = me->hash_func(key);
	
	assert ( (h<=255) && (h>=0));
	
	for(li=me->idx[h]; li; li=li->next)
	{
		if (me->cmp_func (( (hash_elem_t*) li->data)->key, key) == 0)
			return ( ((hash_elem_t*)li->data)->data);
	}
  return NULL;
}


/* destroy hash table */
void 
hash_free(hash_t* me)
{
	int h;
	list_t* li;
	
	assert_ptr(me);
	
	for (h=0; h < 256; h++)
	{

		if (! &me->idx[h]) continue;
		for (li = me->idx[h]; li; li=li->next)
			free (li->data);
		
/*		list_free(&(me->idx[h])); */
	}

}

