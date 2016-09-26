#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>



#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE -1)
#define LED_BASE_ADDR 0x43C00000
#define SWITCH_BASE_ADDR 0x41200000

void *getVirtualAddress(int phys_addr){
    void *mapped_base;
    int memfd;

    void *mapped_dev_base;
    off_t dev_base = phys_addr;

    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd ==  -1){
        printf("can't open /dev/mem\n");
        exit(0);
    }

    mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
    if(mapped_base == (void *) -1){
        printf("can't map the memory to user space.\n");
        exit(0);
    }
    mapped_dev_base =  mapped_base + (dev_base & MAP_MASK);
    return mapped_dev_base;
}


int main(){
    int *_led1 = getVirtualAddress(LED_BASE_ADDR);
    int *_led2 = getVirtualAddress(LED_BASE_ADDR + 4);
    int *_led3 = getVirtualAddress(LED_BASE_ADDR + 8);
    int *_led4 = getVirtualAddress(LED_BASE_ADDR + 12);
    int *_switch = getVirtualAddress(SWITCH_BASE_ADDR);
    int num=0;
    int i = 0;
    printf("hello zedboard\n");
    printf("led1%x\n", _led1);
    printf("led2%x\n", _led2);
    printf("led3%x\n", _led3);
    printf("led4%x\n", _led4);
    printf("switch%x\n", _switch);
    if (*_switch == 1){
        printf("num%x\n");
        *_led1 = 1;
    }
    /*while(1){
        if (num == 1024){
            num = 0;
        }
        else {
            num++;
        }
        *_led1 = num;
        *_led2 = num;
        *_led3 = num;
        *_led4 = num;
        printf("num%x\n", num);
        printf("led1%x\n", *_led1);
        sleep(0.1);
    }*/
    return 0;
}
