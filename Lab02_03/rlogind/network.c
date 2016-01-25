/*-
 * Copyright (c) 1983, 1988, 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>       /* for MAXHOSTNAMELEN */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>           /* for snprintf(), BUFSIZ */
#include <syslog.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "rlogind.h"

int from_af;
static int confirmed;
static int netf;


static char *
find_hostname(const struct sockaddr *fromp, socklen_t fromlen, int *hostokp)
{
	struct addrinfo hints, *res, *res0;
	char naddr[NI_MAXHOST];
	char raddr[NI_MAXHOST];
	char hbuf[NI_MAXHOST];
	char *hname;
	int hostok = 0;
	struct sockaddr_in v4;

	if (fromp->sa_family == AF_INET6) {
		const struct sockaddr_in6 *v6p = (const void *)fromp;

		if (IN6_IS_ADDR_V4MAPPED(&v6p->sin6_addr)) {
			v4.sin_family = AF_INET;
			v4.sin_addr.s_addr = v6p->sin6_addr.s6_addr32[3];
			fromp = (struct sockaddr *)&v4;
		}
	}

	if (getnameinfo(fromp, fromlen, hbuf, sizeof(hbuf), NULL, 0,
			NI_NAMEREQD)) {
		if (getnameinfo(fromp, fromlen, hbuf, sizeof(hbuf), NULL, 0,
				NI_NUMERICHOST))
			strcpy(hbuf, "???");
		hname = strdup(hbuf);
		hostok = 1;
	} 
	else {
		/*
		 * If name returned by gethostbyaddr is in our domain,
		 * attempt to verify that we haven't been fooled by someone
		 * in a remote net; look up the name and check that this
		 * address corresponds to the name.
		 */
		hname = strdup(hbuf);
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = fromp->sa_family;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_CANONNAME;
		if (getaddrinfo(hbuf, NULL, &hints, &res0) == 0) {
			if (getnameinfo(fromp, fromlen, naddr, sizeof(naddr),
					NULL, 0, NI_NUMERICHOST))
				strcpy(naddr, "???");
			for (res = res0; res; res = res->ai_next) {
				if (res->ai_family != fromp->sa_family)
					continue;
				if (getnameinfo(res->ai_addr, res->ai_addrlen,
						raddr, sizeof(raddr), NULL, 0,
						NI_NUMERICHOST) == 0
				    && strcmp(naddr, raddr) == 0) {
					free(hname);
					hname = strdup(res->ai_canonname ?
						res->ai_canonname : hbuf);
					hostok = 1;
					break;
				}
			}
			freeaddrinfo(res0);
		}
	} 

	/* 
	 * Actually it might be null if we're out of memory, but
	 * where do we go then? We'd have to bail anyhow.
	 */
	assert(hname != NULL);

	*hostokp = hostok;

	return hname;
}



char * 
network_init(int f, int *hostokp)
{
	union {
		struct sockaddr_in6 in6;
		struct sockaddr_in in;
		struct sockaddr_storage storage;
		struct sockaddr addr;
	} from;
	struct sockaddr *const fromp = &from.addr;
	socklen_t fromlen;
	int on = 1;
	char c;
	char *hname;
	int port;
	int family;

	fromlen = sizeof (from);
	if (getpeername(f, fromp, &fromlen) < 0) {
		syslog(LOG_ERR,"Can't get peer name of remote host: %m");
		fatal(STDERR_FILENO, "Can't get peer name of remote host", 1);
	}
	if (keepalive &&
	    setsockopt(f, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on)) < 0)
		syslog(LOG_WARNING, "setsockopt (SO_KEEPALIVE): %m");

	family = fromp->sa_family;

#if defined(SOL_IP) && defined(IP_TOS)
	#define IPTOS_LOWDELAY          0x10
	on = IPTOS_LOWDELAY;
	if (family == AF_INET &&
	    setsockopt(f, SOL_IP, IP_TOS, &on, sizeof(on)) < 0)
		syslog(LOG_WARNING, "setsockopt (IP_TOS): %m");
#endif
	on = 1;
	if (disable_nagle &&
	    setsockopt(f, SOL_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) 
		syslog(LOG_WARNING, "setsockopt (TCP_NODELAY): %m");

	alarm(60);
	read(f, &c, 1);

	if (c != 0)
		exit(1);

	alarm(0);

	hname = find_hostname(fromp, fromlen, hostokp);

	switch (family) {
	case AF_INET:
		port = from.in.sin_port;
		break;
	case AF_INET6:
		port = from.in6.sin6_port;
		break;
	default:
		syslog(LOG_NOTICE, "Connection with illegal family %d", family);
		fatal(f, "Permission denied", 0);
	}

	port = ntohs(port);
	if (port >= IPPORT_RESERVED || port < IPPORT_RESERVED/2) {
		syslog(LOG_NOTICE, "Connection from %s on illegal port", hname);
		fatal(f, "Permission denied", 0);
	}

#ifdef IP_OPTIONS
	if (family == AF_INET) {
	    u_char optbuf[BUFSIZ/3], *cp;
	    char lbuf[BUFSIZ];
	    int lboff;
	    socklen_t optsize = sizeof(optbuf);
	    int ipproto;
	    struct protoent *ip;

	    if ((ip = getprotobyname("ip")) != NULL)
		    ipproto = ip->p_proto;
	    else
		    ipproto = IPPROTO_IP;
	    if (getsockopt(0, ipproto, IP_OPTIONS, (char *)optbuf,
		&optsize) == 0 && optsize != 0) {
		    lboff=0;
		    for (cp = optbuf; optsize > 0; cp++, optsize--, lboff += 3)
			    snprintf(lbuf+lboff, sizeof(lbuf)-lboff, 
				     " %2.2x", *cp);
		    syslog(LOG_NOTICE,
			"Connection received using IP options (ignored):%s",
			lbuf);
		    if (setsockopt(0, ipproto, IP_OPTIONS,
				   NULL, optsize) != 0) {
			    syslog(LOG_ERR, "setsockopt IP_OPTIONS NULL: %m");
			    exit(1);
		    }
	    }
	}
#endif

	return hname;
}

void network_confirm(void) {
    assert(confirmed>=0);

    if (confirmed == 0) {		/* do_rlogin may do this */
	write(netf, "", 1);
	confirmed = 1;		/* we sent the null! */
    }
}

void network_anticonfirm(void) {
    char x='\01';		/* error indicator */

    assert(confirmed>=0);

    if (!confirmed) {
	write(netf, &x, 1);
	/* 
	 * Still not confirmed, but we shouldn't ever get here again
	 * as we should be in the process of crashing.
	 */
	confirmed = -1;
    }
}

void network_close(void) {
    shutdown(netf, 2);
}
