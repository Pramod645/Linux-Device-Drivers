obj-m := simple.o 

all:
	make -C /lib/modules/`uname -r`/build  M=`pwd` modules
clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
install:
	make -C /usr/src/arm/linux M=`pwd` modules_install

