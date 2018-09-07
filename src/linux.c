/*
 * Copyright (c) 2009, Giampaolo Rodola'. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Linux-specific functions.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <errno.h>
#include <stdlib.h>
#include <mntent.h>
#include <features.h>
#include <utmp.h>
#include <sched.h>
#include <linux/version.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>

#include "common.h"
#include "posix.h"

int ps__read_file(const char *path, char **buffer, size_t buffer_size) {
  int fd = -1;
  ssize_t ret;
  char *ptr;
  size_t rem_size = buffer_size;

  *buffer = 0;

  fd = open(path, O_RDONLY);
  if (fd == -1) goto error;

  ptr = *buffer = R_alloc(buffer_size, 1);
  if (!*buffer) goto error;

  do {
    if (rem_size == 0) {
      *buffer = S_realloc(*buffer, buffer_size * 2, buffer_size, 1);
      if (!*buffer) goto error;
      ptr = *buffer + buffer_size;
      rem_size = buffer_size;
      buffer_size *= 2;
    }

    ret = read(fd, ptr, rem_size);
    if (ret == -1) goto error;

    ptr += ret;
    rem_size -= ret;
  } while (ret > 0);

  close(fd);

  return buffer_size - rem_size;

 error:
  if (fd >= 0) close(fd);
  if (*buffer) free(*buffer);
  *buffer = 0;
  return -1;
}

SEXP ps__inet_ntop(SEXP raw, SEXP fam) {
  char dst[INET6_ADDRSTRLEN];
  int af = INTEGER(fam)[0];
  const char *ret = inet_ntop(af, RAW(raw), dst, INET6_ADDRSTRLEN);
  if (!ret) {
    return R_NilValue;
  } else {
    return mkString(dst);
  }
}

SEXP ps__define_tcp_statuses() {
  SEXP result, names;

  PROTECT(result = ps__build_string("01", "02", "03", "04", "05", "06",
				    "07", "08", "09", "0A", "0B", "0C", 0));
  PROTECT(names = ps__build_string("CONN_ESTABLISHED",
				   "CONN_SYN_SENT",
				   "CONN_SYN_RECV",
				   "CONN_FIN_WAIT_1",
				   "CONN_FIN_WAIT_2",
				   "CONN_TIME_WAIT",
				   "CONN_CLOSE",
				   "CONN_CLOSE_WAIT",
				   "CONN_LAST_ACK",
				   "CONN_LISTEN",
				   "CONN_CLOSING",
				   "PS__CONN_NONE", 0));

  setAttrib(result, R_NamesSymbol, names);
  UNPROTECT(2);
  return result;
}

SEXP ps__init(SEXP psenv, SEXP constenv) {

  /* Signals */
  defineVar(install("signals"), ps__define_signals(), constenv);

  /* errno values */
  defineVar(install("errno"), ps__define_errno(), constenv);

  /* Connection statuses */
  defineVar(install("tcp_statuses"), ps__define_tcp_statuses(), constenv);

  /* Socket address families */
  defineVar(install("address_families"),
	    ps__define_socket_address_families(), constenv);

  /* Socket address families */
  defineVar(install("socket_types"), ps__define_socket_types(), constenv);

  return R_NilValue;
}
