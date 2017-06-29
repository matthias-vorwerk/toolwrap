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
 *  $Id: policy.h,v 1.3 2005/03/15 18:38:05 bmonthy Exp $
 */
#ifndef _POLICY_H
#define _POLICY_H

#include "toolwrap.h"

#include <string.h>
#include "hash.h"
#include "utils.h"


int load_policy_file(const char*);
char* resolve_pkg(const char*);

#endif /* ! _POLICY_H */
