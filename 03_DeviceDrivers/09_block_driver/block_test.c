 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 2122
#define CHK  250

int main ()
{
    int j, length, fd, rc;
    int vector[SIZE];
    off_t offset;

    length = sizeof (int) * SIZE;
    offset = sizeof (int) * CHK;

    fd = open ("/dev/mybdrv", O_RDWR);

    for (j = 0; j < SIZE; j++) {
        vector[j] = j;
    }

    rc = write (fd, vector, length);
    printf ("\n **** return code from write = %d\n", rc);

    rc = lseek (fd, offset, SEEK_SET);
    printf ("\n **** retrun code from lseek(%d) = %d \n", (int)offset, rc);

    rc = read (fd, vector + CHK, sizeof (int));
    printf ("\n **** retrun code from read vec[%d] = %d, vec[%d] = %d \n",
            CHK, rc, CHK, vector[CHK]);

    close (fd);

    exit (0);

}
