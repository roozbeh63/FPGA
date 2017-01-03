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
#define LED_BASE_ADDR 0x41200000


void *mapped_dev_base;
int memfd;
int mmapInit()
{
    //open file
    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd ==  -1){
        printf("can't open /dev/mem\n");
        exit(0);
    }
    return memfd;
}

//this function gets physical address of the memory and
//gives virtual address which is understandable for the OS
//through this function we can have directly have access to the memory without needing the device drive
void *getVirtualAddress(int phys_addr){
    void *mapped_base;

    off_t dev_base = phys_addr;

    int memfd = mmapInit();
    //read memory
    mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
    if(mapped_base == (void *) -1){
        printf("can't map the memory to user space.\n");
        exit(0);
    }
    mapped_dev_base =  mapped_base + (dev_base & MAP_MASK);

    return mapped_dev_base;
}

void  mmapExit()
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

    mmapInit();
    int *_led_direction_reg = getVirtualAddress(LED_BASE_ADDR + 4);
    int *_led_data_reg = getVirtualAddress(LED_BASE_ADDR);
    //put direction to output
    *(_led_direction_reg) = 0x00;
    //set the led 1 and 2 on

    *(_led_data_reg) = 0x01;
    mmapExit();
    return 0;
}
