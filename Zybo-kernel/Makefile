export ARCH:=arm
export CROSS_COMPILE:=/home/roozbeh/Xilinx/SDK/2016.2/gnu/arm/lin/bin-bas/arm-xilinx-linux-gnueabi-
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := kmodule.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /home/roozbeh/Xilinx/linux-kernel/linux-xlnx
	PWD  := $(shell pwd)
	BUILD := $(PWD)/build
default:
	$(MAKE) -C $(KERNELDIR) M=$(BUILD) src=$(PWD) modules
	mv build/kmodule.ko Debug/kmodule.ko
# If we run make install, copy the module to the Zynq using scp
install:
	$(MAKE) -C $(KERNELDIR) M=$(BUILD) src=$(PWD) modules
	mv build/kmodule.ko Debug/kmodule.ko
	#scp Debug/kmodule.ko root@$(BOARD):/media/card
endif
