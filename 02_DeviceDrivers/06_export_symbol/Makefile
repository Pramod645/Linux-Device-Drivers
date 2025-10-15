obj-m := simple.o usage.o

all:
	#make -C /usr/src/arm/linux  M=`pwd` modules
	make -C /lib/modules/`uname -r`/build  M=`pwd` modules
clean:
	#make -C /usr/src/arm/linux M=`pwd` clean
	make -C /lib/modules/`uname -r`/build M=`pwd` clean



