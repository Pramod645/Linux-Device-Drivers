 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ioctl.h"

int main()
{
	int fd;
	fd = open("simple_char", O_RDWR);
	ioctl(fd, ONE, 0);
	ioctl(fd, TWO, 0);
}
