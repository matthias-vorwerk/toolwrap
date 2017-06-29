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
 *  $Id: env.h,v 1.3 2005/03/18 22:20:10 bmonthy Exp $
 */
#ifndef _ENV_H
#define _ENV_H

#include "toolwrap.h"

#include "limits.h"

#include "assert.h"
#include "log.h"
#include "list.h"
#include "env_lex.h"

void yyerror(const char*);
int env_load_from_package(const char*, const char*);

#define YYPARSE_PARAM curlex
#define YYLEX_PARAM curlex

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


#if 0
#define ENV_DEBUG_PARSER
#endif


#define MAX_ENV_INCLUDE_DEPTH 10

#endif /* ! _ENV_H */
