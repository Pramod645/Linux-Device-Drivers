obj-m += char.o

all:
	#make -C /usr/src/linux M=$(shell pwd) modules
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	#make -C /usr/src/linux M=$(shell pwd) clean
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
