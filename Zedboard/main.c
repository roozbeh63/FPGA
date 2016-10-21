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


#define DEBUG
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE -1)
#define PWM_PERIOD_BASE_ADDR 0x43C00000
#define PWM_DUTY_CYCLE_BASE_ADDR 0x43C00000 + 4
#define PWM_ENABLE_BASE_ADDR 0x43C00000 + 8
#define ENCODER_BASE_ADDR 0x43C10000

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
    int *_PWM_period = getVirtualAddress(PWM_PERIOD_BASE_ADDR);
    int *_duty_cycle = getVirtualAddress(PWM_DUTY_CYCLE_BASE_ADDR);
    //int *_enable = getVirtualAddress(PWM_ENABLE_BASE_ADDR);
    *_PWM_period = 0x186A0;
    *_duty_cycle = 0xC350;
    //*_enable = 0x0;
    //*_duty_cycle = 0x5F5E100;

    int *_encoder = getVirtualAddress(ENCODER_BASE_ADDR);
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

    return 0;
}
