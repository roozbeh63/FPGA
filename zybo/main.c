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


//#define DEBUG
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE -1)
#define PWM_PERIOD_BASE_ADDR 0x43C10000
#define PWM_DUTY_CYCLE_BASE_ADDR 0x43C10000 + 4
#define PWM_ENABLE_BASE_ADDR 0x43C10000 + 8
#define ENCODER_BASE_ADDR 0x43C00000

void *mapped_dev_base;

int mmapInit()
{
   int memfd;
   //open file
   memfd = open("/dev/mem", O_RDWR | O_SYNC);
   if (memfd ==  -1){
       printf("can't open /dev/mem\n");
       exit(0);
   }
}

//this function gets physical address of the memory and
//gives virtual address which is understandable for the OS
//through this function we can have directly have access to the memory without needing the device drive
void *getVirtualAddress(int phys_addr, int memfd){
    void *mapped_base;

    off_t dev_base = phys_addr;

    //read memory
    mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
    if(mapped_base == (void *) -1){
        printf("can't map the memory to user space.\n");
        exit(0);
    }
    mapped_dev_base =  mapped_base + (dev_base & MAP_MASK);

    return mapped_dev_base;
}

void  mmapExit(int memfd)
{
    //close the file
    if (close (memfd) == -1)
    {
        printf("can't close /dev/mem\n");
        exit(0);
    }
    //free the memory
    if (munmap (mapped_dev_base, MAP_SIZE) == -1) {
        printf("can't unmap the memory from user space.\n");
        exit(0);
    }

}


int main(){
    int fd = mmapInit();
    int *_PWM_period = getVirtualAddress(PWM_PERIOD_BASE_ADDR, fd);
    int *_duty_cycle = getVirtualAddress(PWM_DUTY_CYCLE_BASE_ADDR, fd);
    //int *_enable = getVirtualAddress(PWM_ENABLE_BASE_ADDR);
    *_PWM_period = 0x186A0;
    *_duty_cycle = 0xC350;
    //*_enable = 0x0;
    //*_duty_cycle = 0x5F5E100;

    int *_encoder = getVirtualAddress(ENCODER_BASE_ADDR, fd);
#ifdef DEBUG
    printf("hello zedboard\n");
    printf("pwm%p\n", _PWM_period);
    printf("duty cycle%p\n", _duty_cycle);
    printf("encoder%p\n", _encoder);
#endif

    while(1){
        //printf ("enable value%d\n", *_enable);
        int data = *_encoder;
        printf("encoder value%d\n", data & 0x7FFFFFFF);
        printf("encoder direction%d\n", (data & 0x80000000)>>31);
        sleep(0.8);
    }
    /*
    if (*_switch == 1){
        printf("num%x\n");
        *_led1 = 1;
    }
    while(1){
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
    mmapExit(fd);
    return 0;
}
