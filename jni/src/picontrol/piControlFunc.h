#ifndef JNI_SRC_PICONTROL_PICONTROLFUNC
#define JNI_SRC_PICONTROL_PICONTROLFUNC

#include "jni.h"

#include <cstdint>
#include <cstddef>


struct SPIValueStr;
typedef SPIValueStr SPIValue;
struct SPIVariableStr;
typedef SPIVariableStr SPIVariable;

int piControlGet(jint fd, std::size_t offSt, std::size_t len, uint8_t * pData);
int piControlGet(jint fd, SPIValue & pSpiValue);
int piControlGetRaw(jint fd, SPIValue & pSpiValue);
int piControlSet(jint fd, std::size_t offSt, std::size_t len, uint8_t const * pData);
int piControlSet(jint fd, SPIValue & pSpiValue);

int piControlGetVariableInfo(jint fd, SPIVariable * pSpiVariable);

#endif
