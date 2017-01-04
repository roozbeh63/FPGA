export ARCH:=arm
export CROSS_COMPILE:=/home/roozbeh/Xilinx/SDK/2016.3/gnu/arm/lin/bin/arm-xilinx-linux-gnueabi-
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := kmodule_encoder.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /home/roozbeh/Xilinx/linux-kernel/linux-xlnx
	PWD  := $(shell pwd)
	BUILD := $(PWD)/build
default:
	$(MAKE) -C $(KERNELDIR) M=$(BUILD) src=$(PWD) modules
	mv build/kmodule_encoder.ko Debug/kmodule_encoder.ko
# If we run make install, copy the module to the Zynq using scp
install:
	$(MAKE) -C $(KERNELDIR) M=$(BUILD) src=$(PWD) modules
	mv build/kmodule_encoder.ko Debug/kmodule_encoder.ko
	scp Debug/kmodule_encoder.ko ros@192.168.1.104:/home/ros
endif
