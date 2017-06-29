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
 *  $Id: utils.c,v 1.6 2005/03/20 18:04:28 bmonthy Exp $
 */
#include "utils.h"

#ifndef HAVE_STRSEP
/*
 * Get next token from string *stringp, where tokens are possibly-empty
 * strings separated by characters from delim.
 *
 * Writes NULs into the string at *stringp to end tokens.
 * delim need not remain constant from call to call.
 * On return, *stringp points past the last NUL written (if there might
 * be further tokens), or is NULL (if there are definitely no more tokens).
 *
 * If *stringp is NULL, strsep returns NULL.
 */
char *
strsep(char **stringp, const char *delim)
{
	register char *s;
	register const char *spanp;
	register int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}
#endif


#ifndef HAVE_STRNDUP
/* Taken from the GlibC implementation of strndup */
size_t
strnlen(const char *s, size_t maxlen)
{
	const char * eospos;

	eospos = memchr(s, (int)'\0', maxlen);

	return (eospos == NULL ? maxlen : (size_t)(eospos-s));
}

char *strndup(const char *str, size_t len)
{
	size_t n = strnlen(str,len);
	char *new = (char *) malloc (len+1);

	if (NULL == new) {
		return NULL;
	}

	new[n] = '\0';
	return (char *)memcpy (new, str, len);
}
#endif


#ifndef HAVE_SETENV
int setenv(const char* name, const char* value, int overwrite)
{
	char* n;
	
	if (!getenv(name) || overwrite)
	{
		n = (char*) malloc (strlen(name) + strlen(value) + 2);
		sprintf(n, "%s=%s", name, value);
	
		return putenv(n);
	}
	return 0;
}
#endif 

#ifndef HAVE_BASENAME
char *
basename (name)
     const char *name;
{
  const char *base = name;

  while (*name)
    {
      if (*name == '/')
        base = name + 1;
      ++name;
    }
  return (char *) base;
}
#endif


