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
 *  $Id: policy.c,v 1.3 2005/03/15 18:38:05 bmonthy Exp $
 */
#include "policy.h"


static hash_t* policies = NULL;

static unsigned int
p_mkhash(const void* _x)
{
	char* x;
	unsigned int h;

	h=0;
	x=(char*)_x;
	while (*x) h+=*x++;
	return h%256;
}

static int
p_strcmp(const void* _x, const void* _y)
{
	char* x=(char*)_x;
	char* y=(char*)_y;

	return strcasecmp(x,y);
}


int
load_policy_file(const char* filename)
{
	FILE* F;
	char buffer[8192];
	char* line;
	char *pkgname;
	char* tmp;


	if (! policies)
		policies = hash_new(p_strcmp, p_mkhash);

	F = fopen(filename, "r");
	if (!F) return 1;

	while ((line=fgets(buffer, 8192, F)))
	{
		if (line[0]=='#' || line[0]=='\r' || line[0]=='\n') continue;
		while (*line && *line==' ') line++; /* eats leading spaces */

		tmp = strchr(line,':');
  	if (!tmp) continue;
  	pkgname =strndup(line,  tmp-line);


  	line = tmp+1;
  	while (line && *line)
  	{
	  	while (*line && *line==' ') line++;
	  	if (!*line) break;
	  	tmp = strsep(&line, " \r\n");
	  	while (*tmp && *tmp==' ') tmp++;
	  	if (!*tmp) continue; /* empty tool */
	  	hash_append(policies, strdup(tmp), strdup(pkgname));
  	}
	}
	fclose(F);
	return 0;
}


char*
resolve_pkg(const char* tool)
{

	return (char*) hash_lookup(policies, (void*)tool);
}
