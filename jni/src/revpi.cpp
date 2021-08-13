#include "revpi.h"
#include "piControlFunc.h"
#include "piControl.h"

#include "jni.h"
#include <syslog.h>

#include <sstream>
#include <string>
#include <cstring>


// forward declaration
static int readVariable(jint, SPIVariable const &, std::size_t);
static int writeVariable(jint fd,
		SPIVariable const & sPiVariable,
		std::size_t bts,
		uint32_t val);

void throwException(JNIEnv *env, const std::string& exception_name,
		const std::string& msg) {
	const jclass exception = env->FindClass(exception_name.c_str());
	if (exception == NULL) {
		return;
	}
	env->ThrowNew(exception, msg.c_str());
}

void logthis(std::string msg) {
	openlog("librevpi_dio_java native library ", LOG_CONS, LOG_USER);
	std::stringstream strs;
	strs << msg;
	std::string temp_str = strs.str();
	char* char_type = (char*) temp_str.c_str();
	syslog(LOG_INFO, char_type);

}


int readVariableValue(jint fd, char const * pszVariableName)
{
	int rc;
	SPIVariable sPiVariable;

	// kernel code accepts not zero-terminated string. actually,
	// it drops the last char anyways, but who are we to blame then
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
	strncpy(sPiVariable.strVarName,
			pszVariableName,
			sizeof(sPiVariable.strVarName));
#pragma GCC diagnostic pop
	rc = piControlGetVariableInfo(fd, &sPiVariable);
	if (rc < 0) {
		return rc;
	}
	else{
		return readVariable(fd, sPiVariable, sPiVariable.i8uBit);
	}
}

int writeVariableValue(jint fd, const char * pszVariableName, uint32_t i32uValue)
{
	int rc;
	SPIVariable sPiVariable;

	// kernel code accepts not zero-terminated string. actually,
	// it drops the last char anyways, but who are we to blame then
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
	strncpy(sPiVariable.strVarName,
			pszVariableName,
			sizeof(sPiVariable.strVarName));
#pragma GCC diagnostic pop
	rc = piControlGetVariableInfo(fd, &sPiVariable);
	if (rc < 0) {
		return -1;
	}
	else{
		return writeVariable(fd, sPiVariable, sPiVariable.i8uBit, i32uValue);
	}
}

int readVariableBit(jint fd,
		char const * pszVariableName,
		unsigned char btPos)
{
	int rc;
	SPIVariable sPiVariable;
	// kernel code accepts not zero-terminated string. actually,
	// it drops the last char anyways, but who are we to blame then
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
	strncpy(sPiVariable.strVarName,
			pszVariableName,
			sizeof(sPiVariable.strVarName));
#pragma GCC diagnostic pop
	rc = piControlGetVariableInfo(fd, &sPiVariable);
	if (rc < 0) {
		return rc;
	}
	else{
		return readVariable(fd, sPiVariable, btPos);
	}
}

int writeVariableBit(jint fd,
		char const * pszVariableName,
		unsigned char btPos,
		bool val)
{
	int rc;
	SPIVariable sPiVariable;
	// kernel code accepts not zero-terminated string. actually,
	// it drops the last char anyways, but who are we to blame then
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
	strncpy(sPiVariable.strVarName,
			pszVariableName,
			sizeof(sPiVariable.strVarName));
#pragma GCC diagnostic pop
	rc = piControlGetVariableInfo(fd, &sPiVariable);
	if (rc < 0) {
		return rc;
	}
	else{
		return writeVariable(fd, sPiVariable, btPos, (val?1:0));
	}
}

int readVariable(jint fd, SPIVariable const & sPiVariable, std::size_t bts)
{
	int rc;
	SPIValue sPIValue;

	if (sPiVariable.i16uLength == 1) {
		sPIValue.i16uAddress = sPiVariable.i16uAddress;
		sPIValue.i8uBit = bts;

		rc = piControlGetValue(fd, &sPIValue);
		if (rc < 0){
			return rc;
		}else {
			return sPIValue.i8uValue;
		}
	}
	return -99;
}
int writeVariable(jint fd,
		SPIVariable const & sPiVariable,
		std::size_t bts,
		uint32_t val)
{
	int rc;
	SPIValue sPIValue;

	if (sPiVariable.i16uLength == 1) {
		sPIValue.i16uAddress = sPiVariable.i16uAddress;
		sPIValue.i8uBit = bts;
		//TODO: it is an error to assign >8 bits to 8 bits only
		sPIValue.i8uValue = val;
		rc = piControlSetValue(fd, &sPIValue);
		if (rc < 0)
			return -2;
		else{
			return 0;
		}
	}
	return -3;
}
