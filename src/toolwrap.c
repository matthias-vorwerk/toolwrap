/*
 *      Copyright (C) 2004,2004  bmonthy@users.sourceforge.net
 *      Copyright (C) 2017       strassenbahn@pc1715.de
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
 *  $Id: toolwrap.c,v 1.9 2005/03/20 18:04:28 bmonthy Exp $
 */
#include "toolwrap.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <pwd.h>

#include "utils.h"
#include "policy.h"
#include "env.h"



char* g_pkgname;

/* toolwrap execution modes */

#define MODE_EXEC    1
#define MODE_INSTALL 2
#define MODE_MAN     3
#define MODE_INFO    4
#define MODE_WHICH   5



/*
 * long options
 */

struct option longopts[]=
{
	{ "debug",    0, NULL, 'd'},
	{ "install",  1, NULL, 'i'},
	{ "help",     0, NULL, 'h'},
	{ "version",  0, NULL, 'v'},
	{ "pkg",      1, NULL, 'p'},
	{ "man",      0, NULL, 'm'},
	{ "info",     0, NULL, 'I'},
	{ "which",    0, NULL, 'w'},
	{ NULL,       0, NULL,   0}
};


void
usage(char* myself)
{
	fprintf(stdout,
		"Usage: %s [--pkg <pkg>] <tool> [<arguments>]\n"
		"   or: %s [--pkg <pkg>] --man <tool>\n"
		"   or: %s [--pkg <pkg>] --info <tool>\n"
		"   or: %s [--pkg <pkg>] --which <tool>\n"
		"   or: %s --install <pkg>\n"

		"\n"
		"Install or start wrapped applications.\n"
		"\n"

		"  --pkg <pkg>       name of a package.\n"
		"  <tool>            name of a tool.\n"
		"  <arguments>       optional list of <tool>'s arguments.\n"
		"  --install <pkg>   install a package.\n"
		"  --man <tool>      start man(1).\n"
		"  --info <tool>     start info(1).\n"
		"  --which <tool>    returns the exact location of the <tool>.\n"
		"  --debug           debug mode.\n"
		"\n"
		"\n",
		myself,
		myself,
		myself,
		myself,
		myself
		);

	fprintf(stdout,
"Please see http://toolwrap.sourceforge.net for latest releases, "
"documentations & mailing lists.\n\n");

	fprintf(stdout,
		"%s, Copyright (C) 2004,2005  %s\n"
		"%s comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it "
		"under certain conditions.\n"
		"\n\n",
		PACKAGE_STRING,
		PACKAGE_BUGREPORT,
		PACKAGE_NAME);

		exit(0);
}


static void
version()
{

 fprintf(stdout, "%s.\n", PACKAGE_STRING);
 exit(0);
}

static int
load_env(const char* toolname)
{
	char* tmp;
	char* envfile ;
	char* pkgname;
	struct passwd *pwd;


    tmp = (char*) malloc (PATH_MAX);
	snprintf(tmp, PATH_MAX, "%s/etc/toolwrap-policies", g_toolwrap_root);

	if (g_flags & FL_DEBUG)
	{

		log_msg(LOG_DEBUG, "toolwrap invoked: %s.", (g_flags & FL_INVOKED_EXPLICITLY) ? "explicitly":"implicitly");
		log_msg(LOG_DEBUG, "tool name       : %s", toolname);
		log_msg(LOG_DEBUG, "toolwrap root   : %s", g_toolwrap_root);
		log_msg(LOG_DEBUG, "loading default policies file %s", tmp);
	}

	if (load_policy_file(tmp) != 0)
	{
		log_perror(LOG_DEBUG, "%s:", tmp);
		return 1;
	}

	/*
	 * if file TOOLWRAP_POLICIES is set then load this one
	 */
	if (envfile=getenv("TOOLWRAP_POLICIES"))
	{
		if ( access (envfile, R_OK)==0) {
			if (g_flags & FL_DEBUG)
				log_msg(LOG_DEBUG, "Loading TOOLWRAP_POLICIES file %s", envfile);
			load_policy_file(envfile);
		}
	}

	/*
	 * load user policy file
	 */
	pwd = getpwuid(getuid());
	if (pwd)
	{
		snprintf(tmp, PATH_MAX, "%s/.toolwrap", pwd->pw_dir);
		if ( access (tmp, R_OK)==0)
		{
			if (g_flags & FL_DEBUG)
				log_msg(LOG_DEBUG, "Trying user policy file %s", tmp);
			load_policy_file(tmp);
		}
	}
	free(tmp);

	pkgname = g_pkgname;

	if (!pkgname)
		pkgname = resolve_pkg(toolname);

	if (!pkgname)
	{
		log_msg(LOG_ERR, "%s: no wrap policy.", toolname);
		exit(1);
	}

	if ( g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG,"resolved package: %s", pkgname);


	if ( (env_load_from_package("__init__", toolname) !=0) || env_load_from_package(pkgname, toolname)!=0 )
	{
		log_msg(LOG_INFO, "Could not parse environment file for package %s.", pkgname);
		return 1;
	}


	return 0;
}

static void
exec_tool(const char* toolname, int argc, char** argv)
{
	char** args;
	int i;
	int err;

	if (load_env(toolname) !=0) exit(1);

	if (!getenv("TOOLWRAP_BINARY"))
	{
		log_msg(LOG_INFO, "$TOOLWRAP_BINARY not set (environment file issue ? ).");
		exit(0);
	}


	/* create arguments list */
	args = (char**) malloc ( (argc+1)* sizeof(char*));
	for(i=1; i < argc; i++) 	args[i]=strdup(argv[i]);
	args[i]=NULL;
	args[0]=getenv("TOOLWRAP_BINARY");

	/* go !! */
	err = execv(args[0], args);

	/* hmm. something bad happened... */
	log_perror(LOG_ERR, "%s:", args[0]);
	exit(1);
}




static void
exec_man(const char* toolname)
{
	if (load_env(toolname) != 0) exit(0);

	execlp("man", "man", toolname, NULL);
	log_perror(LOG_ERR, "man: ");
}


static void
exec_info(const char* toolname)
{
	if (load_env(toolname) != 0) exit(0);

	execlp("info", "info", "-f" , toolname, NULL);
	log_perror(LOG_ERR, "info: ");
}


static void
exec_which(const char* toolname)
{
	char* res;

	if (load_env(toolname) != 0) exit(0);

	if ((res=getenv("TOOLWRAP_BINARY")) !=NULL)
	{
		if (access(res, X_OK)==0)
		{
			char* real;

			real = (char*) malloc (PATH_MAX);
			realpath(res,real);
			printf("%s\n", real);
			free(real);
			exit(0);
		}
		else
			printf("%s: not executable or not found.\n", res);
	}
	else
		printf("%s: no found.\n", toolname);

	exit(1);
}


static void
install_package(const char* pkgname)
{
	char *pkgbindir;
	DIR* dir;
	struct dirent* di;
  struct stat st;
  char* pkgbintool;
  list_t* binaries, *iter;
  FILE *F;


	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "Installing package %s", pkgname);


	pkgbindir = (char*) malloc (PATH_MAX);
	snprintf(pkgbindir, PATH_MAX, "%s/pkgs/%s/bin", g_toolwrap_root, pkgname);

	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "Creating symlinks from %s...", pkgbindir);


	dir = opendir(pkgbindir);
	if (!dir)
	{
		log_perror(LOG_ERR, "%s: ", pkgbindir);
		return;
	}

	pkgbintool = (char*) malloc (PATH_MAX);

	binaries=NULL;
	while ( (di=readdir(dir)))
	{
		if (di->d_name[0]=='.') continue;

		snprintf(pkgbintool, PATH_MAX, "%s/%s", pkgbindir, di->d_name);

		if ( stat(pkgbintool, &st) != 0)
		{
			log_perror(LOG_NOTICE, "Skipping %s:", pkgbintool);
			continue;
		}

		if ( ! S_ISREG(st.st_mode))
		{
			log_msg(LOG_NOTICE, "Skipping %s: not a regular file", pkgbintool);
			continue;
		}

		if (access(pkgbintool, X_OK)!=0)
		{
			log_msg(LOG_NOTICE, "Skipping %s: not an executable", pkgbintool);
			continue;
		}

		binaries = list_append(binaries, strdup(di->d_name));

		snprintf(pkgbintool, PATH_MAX, "%s/bin/%s", g_toolwrap_root, di->d_name);

		if (access (pkgbintool, F_OK) ==0)
			log_msg(LOG_NOTICE, "%s: Symlink already exiting.", pkgbintool);
		else
		{
		log_msg(LOG_DEBUG, "Creating symlink 	%s -> toolwrap", pkgbintool);

		if (symlink("toolwrap", pkgbintool) != 0)
			log_perror(LOG_ERR, "%s: ", pkgbintool);
		}
	}

	/*
	 * create sample policy file
	 */

	snprintf(pkgbintool, PATH_MAX, "%s/etc/toolwrap-policies.%s", g_toolwrap_root, pkgname);

	log_msg(LOG_INFO, "Creating sample policy file %s", pkgbintool);
	F = fopen(pkgbintool, "w+");
	if (!F)
		log_perror(LOG_ERR, "%s: ", pkgbintool);
	else
	{
		fprintf(F,
				"#\n"
				"# toolwrap policy file for package %s\n"
				"\n"
				"%s: ",
				pkgname,
				pkgname);

		for(iter=binaries; iter; iter=iter->next)
		{
			fprintf(F, "%s ", (char*)iter->data);
		}
		fprintf(F, "\n");
		fclose(F);
	}


	free(pkgbintool);
	free(pkgbindir);
}

int main(int argc, char** argv)
{
	char* tmp;
	char* whoami;
	char* toolname;

	int   toolarg_pos;

	g_flags = 0;
	g_mode = MODE_EXEC;
	g_pkgname = NULL;


	if ((tmp=getenv("TOOLWRAP_ROOT")))
		g_toolwrap_root = strdup(tmp);
	else
		g_toolwrap_root=TOOLWRAP_ROOT;

	if (getenv("TOOLWRAP_DEBUG"))
		g_flags |= FL_DEBUG;

	tmp = strdup(argv[0]);
	whoami = basename(tmp);


	if (strcmp(whoami, TOOLWRAP_EXPLICIT_NAME)==0)
	{
		int ch;
		g_flags |= FL_INVOKED_EXPLICITLY;

		while ( (ch=getopt_long(argc,argv,"+dhi:p:mIw",longopts, NULL)) !=EOF)
		{
			switch (ch)
			{
				case 'd':
					g_flags |= FL_DEBUG;
					break;

				case 'i':
					g_mode  = MODE_INSTALL;
					g_pkgname = strdup(optarg);
					break;

				case 'p':
					g_pkgname = strdup(optarg);
					break;

				case 'm':
					g_mode = MODE_MAN;
					break;

				case 'I':
					g_mode = MODE_INFO;
					break;

				case 'w':
					g_mode = MODE_WHICH;
					break;

				case 'v':
					version();
					break;

				default:
					usage(argv[0]);
			}
		}

		toolarg_pos = optind;

		if ((g_mode != MODE_INSTALL) && (optind == argc))
		{
			usage(argv[0]);
		}

		toolname = argv[optind];
	}

	else
	{
		/* toolwrap invoked implicitly */
		toolname = strdup(whoami);
		g_mode = MODE_EXEC;
		toolarg_pos=0;
	}
	free(tmp);


	switch (g_mode)
	{
		case MODE_EXEC:
			exec_tool(toolname, argc - toolarg_pos, argv+toolarg_pos);
			break;

		case MODE_INSTALL:
			install_package(g_pkgname);
			break;

		case MODE_MAN:
			exec_man(toolname);
			break;

		case MODE_INFO:
			exec_info(toolname);
			break;

		case MODE_WHICH:
			exec_which(toolname);
			break;
	}
	exit(0);
}

