#ifndef JNI_SRC_REVPI_H_
#define JNI_SRC_REVPI_H_

#include "jni.h"

#include <string>
#include <cstdint>
#include <cstddef>


struct SPIValueStr;
typedef SPIValueStr SPIValue;
struct SPIVariableStr;
typedef SPIVariableStr SPIVariable;


void throwException(JNIEnv *, const std::string&,
		const std::string&);
inline void throwIOExceptionMsg(JNIEnv *env, const std::string& msg) {
	throwException(env, "java/io/IOException", msg);
}

void logthis(std::string);
int readVariableValue(jint fd, char const *pszVariableName);
int writeVariableValue(jint fd, char const *, uint32_t);

int readVariableBit(jint fd,
		char const * pszVariableName,
		unsigned char btPos);
int writeVariableBit(jint fd,
		char const * pszVariableName,
		unsigned char btPos,
		bool val);

#endif /* JNI_SRC_REVPI_H_ */
