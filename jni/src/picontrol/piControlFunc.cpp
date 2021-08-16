#include "../picontrol/piControlFunc.h"

#include "piControl.h"

extern "C" {
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
}
#include <cstdint>
#include <cstddef>


int piControlGet(jint fd,
		std::size_t offSt,
		std::size_t len,
		uint8_t * pData)
{
	int retVal;

	if (fd >= 0){
	    /* seek */
	    if (lseek(fd, offSt, SEEK_SET) >= 0) {
			/* read */
			if ( (retVal = read(fd, pData, len)) < 0) {
					retVal = -errno;
			}
			// else number of bytes read
	    }
	    else{
	    	retVal = -errno;
	    }
	}
	else{
		retVal = -ENODEV;
	}
	return retVal;
}
int piControlGet(jint fd, SPIValue & pSpiValue)
{
	pSpiValue.i16uAddress += pSpiValue.i8uBit / 8;
	pSpiValue.i8uBit %= 8;

	return piControlGetRaw(fd, pSpiValue);
}
int piControlGetRaw(jint fd, SPIValue & pSpiValue)
{
	if (fd < 0){
		return -ENODEV;
	}
	if (ioctl(fd, KB_GET_VALUE, &pSpiValue) < 0){
		return -errno;
	}
	return 0;
}

int piControlSet(jint fd,
		std::size_t offSt,
		std::size_t len,
		uint8_t const * pData)
{
	int retVal;

	if (fd >= 0){
	    /* seek */
	    if (lseek(fd, offSt, SEEK_SET) >= 0) {
			/* read */
			if ( (retVal = write(fd, pData, len)) < 0) {
					retVal = -errno;
			}
			// else number of bytes written
	    }
	    else{
	    	retVal = -errno;
	    }
	}
	else{
		  retVal = -ENODEV;
	}
	return retVal;
}
int piControlSet(jint fd, SPIValue & pSpiValue)
{
	if (fd < 0){
		return -ENODEV;
	}
	pSpiValue.i16uAddress += pSpiValue.i8uBit / 8;
	pSpiValue.i8uBit %= 8;

	if (ioctl(fd, KB_SET_VALUE, &pSpiValue) < 0){
		return -errno;
	}
	return 0;
}


int piControlGetVariableInfo(jint fd, SPIVariable * pSpiVariable)
{
	if (fd < 0){
		return -ENODEV;
	}
	if (ioctl(fd, KB_FIND_VARIABLE, pSpiVariable) < 0){
		return -errno;
	}
	return 0;
}
