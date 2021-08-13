#include "jni.h"

struct SPIValueStr;
typedef SPIValueStr SPIValue;
struct SPIVariableStr;
typedef SPIVariableStr SPIVariable;

int piControlGetValue(jint fd, SPIValue * pSpiValue);
int piControlSetValue(jint fd, SPIValue * pSpiValue);
int piControlGetVariableInfo(jint fd, SPIVariable * pSpiVariable);
