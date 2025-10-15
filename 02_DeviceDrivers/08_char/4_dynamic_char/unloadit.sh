#/***************************************************************************
# *                                                                         *
# *      Author          : Ganesh Naik                                      *
# *      Organisation    : Levana Technologies, Mumbai, India.              *
# *      Website	: www.levanatech.com                                *
# *                                                                         *
# *  Conducting Workshops on - Embedded Linux & Device Drivers Training.    *
# *  -------------------------------------------------------------------    *
# *  Tel : 91-9869260324, Email : info@levanatech.com                       *
# *                                                                         *
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


major=$(awk "\$2==\"simple_char\" {print \$1}" /proc/devices)
echo major number is: $major

echo unloading $module
rmmod $module

echo removing the device node at $node with minor number=0

rm $node






