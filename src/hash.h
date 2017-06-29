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
 *  $Id: hash.h,v 1.2 2005/03/15 18:38:05 bmonthy Exp $
 */
#ifndef _HASH_H
#define _HASH_H

#include "toolwrap.h"
#include "list.h"

struct _hash_t
{
	list_t* idx[256];
	int (*cmp_func)(const void*,const void*);
	unsigned int (*hash_func)(const void*);
} ;
typedef struct _hash_t hash_t;

struct _hash_elem_t
{
	const void* key;
  const void* data;
};
typedef struct _hash_elem_t hash_elem_t;

hash_t*     hash_new( int(*)(const void*,const void*), unsigned int(*)(const void*));
void        hash_append(hash_t*, const void*, void*);
const void* hash_lookup(hash_t*, const void*);
void        hash_free(hash_t*);




#endif /* ! _HASH_H */
