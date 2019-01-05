obj-m += CharDev.o

all:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) modules
clean:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) clean
install:
	make -C /usr/src/linux-headers-$(shell uname -r) M=$(PWD) modules_install INSTALL_MOD_PATH=/home/bekir/Eclipse_Projects/CharDevModule/build_modules
