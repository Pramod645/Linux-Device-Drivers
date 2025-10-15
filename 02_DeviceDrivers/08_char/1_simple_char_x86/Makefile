obj-m := char.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules
	#make -C /usr/src/linux-2.6.26 M=`pwd` modules
	#make -C /usr/src/arm/linux M=`pwd` modules
clean:
	#make -C /usr/src/arm/linux M=`pwd` clean
	make -C /lib/modules/`uname -r`/build M=`pwd` clean



