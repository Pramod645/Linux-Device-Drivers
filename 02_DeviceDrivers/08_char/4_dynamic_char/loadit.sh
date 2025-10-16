# ***************************************************************************
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation. No warranty is attached; we cannot take *
# *   responsibility for errors or fitness for use.                         *
# ***************************************************************************/
#
#!/bin/sh

module=dynamic_char.ko
node=/dev/simple_char
minor=0

[ "$1" != "" ] && module="$1"
[ "$2" != "" ] && node="$2"

echo loading $module
insmod $module

major=$(awk "\$2==\"simple_char\" {print \$1}" /proc/devices)
echo major number is: $major

echo creating the device node at $node with minor number=0

mknod $node c $major $minor






