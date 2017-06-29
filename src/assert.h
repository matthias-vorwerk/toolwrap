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
 *  $Id: assert.h,v 1.2 2005/03/15 18:38:05 bmonthy Exp $
 */
#ifndef _ASSERT_H
#define _ASSERT_H

#include "toolwrap.h"
#include "log.h"


#ifdef _DEBUG
# define assert(x) if ( ! (x)) {log_msg(LOG_CRIT, "*** assertion failed : %s ***", ""#x""); abort(); }
# define assert_ptr(x) assert ( (x) != NULL )
#else
# define assert(x) /* nothing */
# define assert_ptr(x) assert(x)
#endif 



#endif /* ! _ASSERT_H */

