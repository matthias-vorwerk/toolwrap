%{

#define YYSTYPE char*
#define YYERROR_VERBOSE 1

#include "toolwrap.h"
#include "log.h"
#include "env.h"

static char* env_get_var(const char*);
static char* concat(const char*, const char*);
static void  env_set(const char*, const char*);
static void  env_append(const char*, const char*);
static void  env_prepend(const char*, const char*);
static void  env_requires(const char*);
%}
 
%verbose
%defines
%token ENV_SET
%token ENV_PREPEND
%token ENV_APPEND
%token REQUIRES
%token WORD 
%debug

%pure_parser


%%

input: /* empty */
	| stmt input
	;
	
stmt: 
	 envset_stmt	;
 | requires_stmt
 ;
 	
envset_stmt: 	
	  ENV_SET     WORD expr {env_set($2,$3);} ;
	| ENV_PREPEND WORD expr {env_prepend($2,$3);}
	| ENV_APPEND  WORD expr {env_append($2,$3);}
	;
	
requires_stmt:
	REQUIRES WORD {env_requires($2);}
	;
		
	
expr: 	
     var 
   | var expr { $$ =  concat($1,$2);}
   ;

var:  
		'$' '{' expr '}'  { $$ = env_get_var($3); };	
	| WORD
	;
	
	
%%

static char* 
env_get_var(const char* var)
{
	char* ret;
	
	ret=getenv(var);
	return ret ? ret : "";
	
}	

static char* 
concat(const char* s1, const char* s2)
{
	char* ret;
	size_t s1_len, s2_len;
	
	s1_len=strlen(s1);
	s2_len=strlen(s2);
	
	ret = (char*) malloc (s1_len + s2_len + 1);
	memcpy(ret, s1, s1_len);
	memcpy(ret+ s1_len, s2, s2_len);
	*(ret + s1_len+s2_len)='\0';
	return ret;
}

static void
env_set(const char* name, const char* val)
{
	char* n;
	size_t nlen;
	
	nlen =strlen(name)+strlen(val)+1 +1;
	n=(char*) malloc(nlen);
	snprintf(n,nlen,"%s=%s", name, val);
	putenv(n);
	
	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "setenv %s", n);
	
}

static void 
env_prepend(const char* name, const char* val)
{
	char* n;
	size_t nlen;
	
	char* old;
	
	old =getenv(name);
	
	nlen=strlen(name) + 1 + strlen(val) + 	(old ? strlen(old) : 0) +1 +1;
	n = (char*) malloc (nlen);
	if (old)
		snprintf(n,nlen, "%s=%s:%s", name, val, old );
	else
		snprintf(n,nlen,"%s=%s", name, val);
	putenv(n);
	
	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "setenv %s", n);
			
	/* do not free(n): on some libc, this is now a part of our env. */
}


static void 
env_append(const char* name, const char* val)
{
	char* n;
	size_t nlen;
	
	char* old;
	
	old =getenv(name);
	
	nlen=strlen(name) + 1 + strlen(val) + 	(old ? strlen(old) : 0) +1 +1;
	n = (char*) malloc (nlen);
	if (old)
		snprintf(n,nlen, "%s=%s:%s", name, old,val);
	else
		snprintf(n,nlen,"%s=%s", name, val);
	putenv(n);
	

	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "setenv %s", n);
			
	/* do not free(n): on some libc, this is now a part of our env. */
}

static void
env_requires(const char* pkg)
{

	if (g_flags & FL_DEBUG)
		log_msg(LOG_DEBUG, "requires env. from package %s", pkg);

	env_load_from_package(pkg, NULL);
}

