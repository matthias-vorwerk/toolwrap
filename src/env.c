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
 *  $Id: env.c,v 1.5 2005/04/26 12:56:57 bmonthy Exp $
 */


/* env.c : load toolwrap's environment files */

#include "env.h"


/* parser callback for error reporting */
void yyerror(const char* msg)
{

	log_msg(LOG_INFO, "Parser error: %s", msg);
}


#ifdef ENV_DEBUG_PARSER
extern int yydebug;
#endif

/* loads the given environment file; returns 0 if OK */
int
env_load_file(const char* file)
{
	env_lex_t* lex;
	int ret;

	lex = env_lex_new(file);
	if (!lex)
	{
		log_perror(LOG_ERR, "%s: ", file);
		return 1;
	}



#ifdef ENV_DEBUG_PARSER
	yydebug=1;
#endif
	ret = yyparse(lex);

	env_lex_free(lex);

	return ret;
}

/* loads the environment for the specified package & tool */

int
env_load_from_package(const char* pkgname, const char* toolname)
{
	char *tmp;
	int ret;
	char* cur_PKG=NULL;
	char* cur_TOOL=NULL;
	char* cur_PKGROOT=NULL;
	int   env_backup;

	tmp=(char*) malloc (PATH_MAX);

	/*
	 * save current state, as env. files may be recursive
	 */

/*	if (strcasecmp(pkgname, "__default__") != 0)
	{
*/		env_backup=1;
		cur_PKG = getenv("TOOLWRAP_PKG");
		cur_TOOL = getenv("TOOLWRAP_TOOLNAME");
		cur_PKGROOT = getenv("TOOLWRAP_PKGROOT");

		setenv("TOOLWRAP_PKG", pkgname, 1);
		if (toolname)
			setenv("TOOLWRAP_TOOL", toolname, 1);
		snprintf(tmp, PATH_MAX, "%s/pkgs/%s", g_toolwrap_root, pkgname);
		setenv("TOOLWRAP_PKGROOT", tmp, 1);
/*	}
	else
		env_backup=0;
*/

	snprintf(tmp, PATH_MAX, "%s/env/%s", g_toolwrap_root, pkgname);

	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "trying to load env file %s", tmp);

	if (access(tmp, R_OK) !=0)
	{
		if (g_flags & FL_DEBUG)
			log_msg(LOG_DEBUG, "skipping env. file %s: not found", tmp);

		snprintf(tmp, PATH_MAX, "%s/env/%s", g_toolwrap_root, "__default__");
		if (g_flags & FL_DEBUG)
			log_msg(LOG_DEBUG, "trying to load env file %s", tmp);

		if (access(tmp, R_OK) !=0)
		{
			if (g_flags & FL_DEBUG)
				log_msg(LOG_DEBUG, "skipping env. file %s: not found", tmp);
			free(tmp);
			return 1;
		}

	}

	ret = env_load_file(tmp);
	free(tmp);

	if (env_backup)
	{
		if (cur_PKG) setenv("TOOLWRAP_PKG", cur_PKG, 1);
		if (cur_TOOL) setenv("TOOLWRAP_TOOL", cur_TOOL, 1);
		if (cur_PKGROOT) setenv("TOOLWRAP_PKGROOT", cur_PKGROOT, 1);
	}

	return ret;
}


