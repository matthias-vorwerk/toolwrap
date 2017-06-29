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
 *  $Id: env_lex.h,v 1.4 2005/04/26 12:56:57 bmonthy Exp $
 */
 
 

#ifndef _ENV_LEX_H
#define _ENV_LEX_H

#include "toolwrap.h"

#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

#define YYSTYPE char*

#include "env.h"
#include "list.h"
#include "log.h"

#define MAX_WORD_LEN 512


struct _env_lex_t
{
	FILE*   in;			/* input file handle */
	int     cond;                   /* current start condition */
	char*   buffer;                 /* temporary buffer */
	size_t  buffer_len;             /* temporary buffer length */ 
	int     eof;                    /* end of file */
	char    tmp[MAX_WORD_LEN];      /* current read word */
};

typedef struct _env_lex_t env_lex_t;

env_lex_t *env_lex_new(const char*);
void      env_lex_free(env_lex_t*);


int yylex(YYSTYPE *yylval, env_lex_t*);


#endif /* ! _ENV_LEX_H */
