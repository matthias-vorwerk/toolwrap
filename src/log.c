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
 *  $Id: log.c,v 1.3 2005/03/15 18:38:05 bmonthy Exp $
 */
#include "log.h"


static FILE* logfile=NULL;

static void 
logger_file(int p, const char* fmt, va_list ap)
{
  static char* P;
  time_t now;
  char t[256];
  
  if (!logfile) logfile=stderr;
  
  switch (p)
  {
    case LOG_INFO:
      P="INFO";
      break;
 
    case LOG_NOTICE:
      P="NOTICE";
      break;
    
    case LOG_DEBUG:
      P="DEBUG";
      break;
    
    case LOG_ERR:
    default:
      P="ERROR";
      break;    
  }
  
  fprintf (logfile, "[%s] ",P);
  vfprintf(logfile, fmt, ap);
  fprintf (logfile, "\n");

  fflush(logfile);  
}


static void 
logger_syslog(int p, const char* fmt, va_list ap)
{
  vsyslog( p, fmt, ap);
}


/*
 * default logger function
 */
static void (*logger)(int, const char*, va_list) = logger_file;


void
log_msg(int p, const char* fmt, ...)
{
  va_list ap;
  
  va_start(ap, fmt);
  logger(p, fmt, ap);
  va_end(ap);
  
}


void
log_perror(int p, const char* fmt, ...)
{
  va_list ap;
  char msg[1024];
 
  va_start(ap,fmt); 
  vsnprintf(msg, 1024, fmt, ap);
  va_end(ap);
  log_msg(p, "%s %s", msg, strerror(errno));
  
}



void 
log_set_logfile(const char* filename)
{
  FILE* sav;
  
  sav = logfile;
  logfile = fopen(filename, "a+");
  if (!logfile)
      logfile=sav;
  else
      logger = logger_file;
}


void 
log_set_stderr()
{
  logfile = stderr;
  logger = logger_file;
}

void 
log_set_syslog(char* ident, int opt, int facility)
{
  openlog(ident, opt, facility);
  logger = logger_syslog;
}

