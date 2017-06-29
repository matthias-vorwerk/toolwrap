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
 *  $Id: log.h,v 1.3 2005/03/15 18:38:05 bmonthy Exp $
 */
#ifndef _LOG_H
#define _LOG_H


#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


void log_msg(int, const char*, ...);
void log_perror(int, const char*, ...);
void log_set_logfile(const char* filename);
void log_set_stderr();
void log_set_syslog_msg(char* ident, int opt, int facility);



#endif /* _LOG_H */

