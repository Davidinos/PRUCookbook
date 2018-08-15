/* 
 *
 *  pwm tester
 *	The on cycle and off cycles are stored in each PRU's Data memory
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAXCH 4

#define PRU_ADDR		0x4A300000		// Start of PRU memory Page 184 am335x TRM
#define PRU_LEN			0x80000			// Length of PRU memory
#define PRU0_DRAM		0x00000			// Offset to DRAM
#define PRU1_DRAM		0x02000
#define PRU_SHAREDMEM	0x10000			// Offset to shared memory

unsigned int	*pru0DRAM_32int_ptr;		// Points to the start of local DRAM
unsigned int	*pru1DRAM_32int_ptr;		// Points to the start of local DRAM
unsigned int	*prusharedMem_32int_ptr;	// Points to the start of the shared memory

/*******************************************************************************
* int start_pwm_count(int ch, int countOn, int countOff)
* 
* Starts a pwm pulse on for countOn and off for countOff to a single channel (ch)
*******************************************************************************/
int start_pwm_count(int ch, int countOn, int countOff) {
	unsigned int *pruDRAM_32int_ptr = pru0DRAM_32int_ptr;
	
	printf("countOn: %d, countOff: %d, count: %d\n", 
		countOn, countOff, countOn+countOff);
	// write to PRU shared memory
	pruDRAM_32int_ptr[2*(ch)+0] = countOn;	// On time
	pruDRAM_32int_ptr[2*(ch)+1] = countOff;	// Off time
	return 0;
}

int main(int argc, char *argv[])
{
	unsigned int	*pru;		// Points to start of PRU memory.
	int	fd;
	printf("Servo tester\n");
	
	fd = open ("/dev/mem", O_RDWR | O_SYNC);
	if (fd == -1) {
		printf ("ERROR: could not open /dev/mem.\n\n");
		return 1;
	}
	pru = mmap (0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
	if (pru == MAP_FAILED) {
		printf ("ERROR: could not map memory.\n\n");
		return 1;
	}
	close(fd);
	printf ("Using /dev/mem.\n");
	
	pru0DRAM_32int_ptr =     pru + PRU0_DRAM/4 + 0x200/4;	// Points to 0x200 of PRU0 memory
	pru1DRAM_32int_ptr =     pru + PRU1_DRAM/4 + 0x200/4;	// Points to 0x200 of PRU1 memory
	prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4;	// Points to start of shared memory

	// int i;
	// for(i=0; i<SERVO_CHANNELS; i++) {
	// 	start_pwm_us(i, 1000, 5*(i+1));
	// }

	// int period=1000;
	// start_pwm_us(0, 1*period, 10);
	// start_pwm_us(1, 2*period, 10);
	// start_pwm_us(2, 4*period, 10);
	// start_pwm_us(3, 8*period, 10);
	// start_pwm_us(4, 1*period, 10);
	// start_pwm_us(5, 2*period, 10);
	// start_pwm_us(6, 4*period, 10);
	// start_pwm_us(7, 8*period, 10);
	// start_pwm_us(8, 1*period, 10);
	// start_pwm_us(9, 2*period, 10);
	// start_pwm_us(10, 4*period, 10);
	// start_pwm_us(11, 8*period, 10);
	
	int i;
	for(i=0; i<MAXCH; i++) {
		start_pwm_count(i, i+1, 20-(i+1));
	}
	
	// start_pwm_count(0, 1, 1);
	// start_pwm_count(1, 2, 2);
	// start_pwm_count(2, 10, 30);
	// start_pwm_count(3, 30, 10);
	// start_pwm_count(4, 1, 1);
	// start_pwm_count(5, 10, 10);
	// start_pwm_count(6, 20, 30);
	// start_pwm_count(7, 30, 20);
	// start_pwm_count(8, 1, 3);
	// start_pwm_count(9, 2, 2);
	// start_pwm_count(10, 3, 1);
	// start_pwm_count(11, 1, 7);
	
	// start_pwm_count(12, 1, 15);
	// start_pwm_count(13, 2, 15);
	// start_pwm_count(14, 3, 15);
	// start_pwm_count(15, 4, 15);
	// start_pwm_count(16, 5, 15);
	// start_pwm_count(17, 6, 15);
	
	// for(i=0; i<24; i++) {
	// 	int mask = 1 << (i%12);
	// 	printf("Mask: %x\n", mask);
	// 	pwm_enable(mask);
	// 	usleep(500000);
	// }
	
	if(munmap(pru, PRU_LEN)) {
		printf("munmap failed\n");
	} else {
		printf("munmap succeeded\n");
	}
}

