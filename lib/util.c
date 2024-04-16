/* ----------------------------------------------------------------------------
   libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2023  Mark A Lindner

   This file is part of libconfig.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, see
   <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------------
*/

#include "util.h"
#include "wincompat.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------- */

void libconfig_fatal_error_handler(const char *message)
{
  if(posix_write(STDERR_FILENO, (const void *)message, strlen(message))) {}
  abort();
}

/* ------------------------------------------------------------------------- */

static void (*__libconfig_fatal_error_func)(const char *) =
  libconfig_fatal_error_handler;

static const char *__libconfig_malloc_failure_message =
  "\alibconfig: memory allocation failure\n";

/* ------------------------------------------------------------------------- */

void libconfig_set_fatal_error_func(void (*func)(const char *))
{
  __libconfig_fatal_error_func = (func ? func
                                  : libconfig_fatal_error_handler);
}

/* ------------------------------------------------------------------------- */

void libconfig_fatal_error(const char *message)
{
  __libconfig_fatal_error_func(message);
}

/* ------------------------------------------------------------------------- */

void *libconfig_malloc(size_t size)
{
  void *ptr = malloc(size);
  if(!ptr)
    libconfig_fatal_error(__libconfig_malloc_failure_message);

  return(ptr);
}

/* ------------------------------------------------------------------------- */

void *libconfig_calloc(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);
  if(!ptr)
    libconfig_fatal_error(__libconfig_malloc_failure_message);

  return(ptr);
}

/* ------------------------------------------------------------------------- */

void *libconfig_realloc(void *ptr, size_t size)
{
  ptr = realloc(ptr, size);
  if(!ptr)
    libconfig_fatal_error(__libconfig_malloc_failure_message);

  return(ptr);
}

/* ------------------------------------------------------------------------- */

long long libconfig_parse_integer(const char *s, int *ok)
{
  long long llval;
  char *endptr;
  int errsave = errno;
  errno = 0;
  llval = strtoll(s, &endptr, 0);	/* base 10 or base 8 */
  if(*endptr || errno)
  {
    errno = 0;
    *ok = 0;
    return(0);	/* parse error */
  }
  errno = errsave;

  *ok = 1;
  return(llval);
}

/* ------------------------------------------------------------------------- */

unsigned long long libconfig_parse_hex64(const char *s)
{
#ifdef __MINGW32__

  /* MinGW's strtoull() seems to be broken; it only returns the lower
   * 32 bits...
   */

  const char *p = s;
  unsigned long long val = 0;

  if(*p != '0')
    return(0);

  ++p;

  if(*p != 'x' && *p != 'X')
    return(0);

  for(++p; isxdigit(*p); ++p)
  {
    val <<= 4;
    val |= ((*p < 'A') ? (*p & 0xF) : (9 + (*p & 0x7)));
  }

  return(val);

#else /* ! __MINGW32__ */

  return(strtoull(s, NULL, 16));

#endif /* __MINGW32__ */
}

/* ------------------------------------------------------------------------- */

unsigned long long libconfig_parse_bin64(const char *s)
{
#ifdef __MINGW32__

  /* Assume that MinGW's strtoull() is broken for base 2 as well...
   */

  const char *p = s;
  unsigned long long val = 0;

  if(*p != '0')
    return(0);

  ++p;

  if(*p != 'b' && *p != 'B')
    return(0);

  for(++p; *p=='0' || *p == '1'; ++p)
  {
    val <<= 1;
    val |= *p =='1';
  }

  return(val);

#else /* ! __MINGW32__ */

  /* skip the first 2 bytes since I don't know if all implementations will ignore 0b */
  return(strtoull(s+2, NULL, 2));

#endif /* __MINGW32__ */
}

/* ------------------------------------------------------------------------- */

void libconfig_format_double(double val, int precision, int sci_ok, char *buf,
                             size_t buflen)
{
  const char *fmt = sci_ok ? "%.*g" : "%.*f";
  char *p, *q;

  snprintf(buf, buflen - 3, fmt, precision, val);

  /* Check for exponent. */
  p = strchr(buf, 'e');
  if(p) return;

  /* Check for decimal point. */
  p = strchr(buf, '.');
  if(!p)
  {
    /* No decimal point. Add trailing ".0". */
    strcat(buf, ".0");
  }
  else
  {
    /* Remove any excess trailing 0's after decimal point. */
    for(q = buf + strlen(buf) - 1; q > p + 1; --q)
    {
      if(*q == '0')
        *q = '\0';
      else
        break;
    }
  }
}

/* ------------------------------------------------------------------------- */


#ifdef __GNUC__
#define clzl(x) __builtin_clzl(x)
#else
static int clzl(int64_t val)
{
  int leading = 0;
  int64_t check = (1ll << 63);
  while (!(val & check) && check)
  {
    leading++;
    check >>= 1;
  }
  return leading;
}
#endif

void libconfig_format_bin(int64_t val, char *buf, size_t buflen)
{
  /* find number of leading 0's */
  unsigned c = clzl(val);
  unsigned i = 0;
  while ( (c < 64) && (i < buflen - 1) )
  {
    buf[i] = (val & (1ll << (63 - c))) ? '1' : '0';
    i++;
    c++;
  }
  buf[i] = 0;
}
