/***************************************************************************
 *                                                                         *
 *      Author          : Ganesh Naik                                      *
 *      Organisation    : Levana Technologies, Mumbai, India.              *
 *      Website		: www.levanatech.com                               *
 *                                                                         *
 *  Conducting Workshops on - Embedded Linux & Device Drivers Training.    *
 *  -------------------------------------------------------------------    *
 *  Tel : 91-9869260324, Email : info@levanatech.com                       *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/


#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define MAX_PAYLOAD 1024
#define NL_EXAMPLE 19  
#define NL_GROUP 1

int read_event (int sock)
{
    int ret;
    struct nlmsghdr *nlh;

    nlh = malloc (NLMSG_SPACE (MAX_PAYLOAD));
    memset (nlh, 0, NLMSG_SPACE (MAX_PAYLOAD));
    ret = recv (sock, (void *)nlh, NLMSG_SPACE (MAX_PAYLOAD), 0);

    printf ("Message size: %d , Message: %s\n", ret, (char *)NLMSG_DATA (nlh));

    return 0;
}

int main (int argc, char *argv[])
{
    struct sockaddr_nl addr;
    int nls;
    int rc;

    nls = socket (PF_NETLINK, SOCK_RAW, NL_EXAMPLE);
    printf ("nls=%d\n", nls);

    memset ((void *)&addr, 0, sizeof (addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid ();
    addr.nl_groups = NL_GROUP;
    rc = bind (nls, (struct sockaddr *)&addr, sizeof (addr));
    printf ("errno=%d\n", errno);
    perror ("bind");
    printf ("rc from bind = %d\n", rc);

    while (1)
        read_event (nls);

    return 0;
}
