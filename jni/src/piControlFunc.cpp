#include "piControlFunc.h"
#include "piControl.h"

extern "C" {
#include <sys/ioctl.h>
#include <errno.h>
}


int piControlGetValue(jint fd, SPIValue * pSpiValue)
{
	if (fd < 0){
		return -ENODEV;
	}
	pSpiValue->i16uAddress += pSpiValue->i8uBit / 8;
	pSpiValue->i8uBit %= 8;

	if (ioctl(fd, KB_GET_VALUE, pSpiValue) < 0){
		return -errno;
	}
	return 0;
}

int piControlSetValue(jint fd, SPIValue * pSpiValue)
{
	if (fd < 0){
		return -ENODEV;
	}
	pSpiValue->i16uAddress += pSpiValue->i8uBit / 8;
	pSpiValue->i8uBit %= 8;

	if (ioctl(fd, KB_SET_VALUE, pSpiValue) < 0){
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
