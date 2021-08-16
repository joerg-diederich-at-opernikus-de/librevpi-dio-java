#include "revpi.h"
#include "piControl.h"

#include "jni.h"
#include <syslog.h>

#include <sstream>
#include <string>
#include <cstring>

#include "picontrol/piControlFunc.h"


// forward declarations
static long readVariable(jint, SPIVariable const &);
static int readVariable(jint fd,
		SPIVariable const & sPiVariable,
		unsigned char bt);
static int writeVariable(jint fd,
		SPIVariable const & sPiVariable,
		std::size_t len,
		uint32_t val);
static int writeVariable(jint fd,
		SPIVariable const & sPiVariable,
		unsigned char bt,
		bool val);

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


long readVariableWord(jint fd, char const * pszVariableName)
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
		return readVariable(fd, sPiVariable);
	}
}
int readVariableBit(jint fd,
		char const * pszVariableName)
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
int readVariableBit(jint fd,
		char const * pszVariableName,
		unsigned char bt)
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
		return readVariable(fd, sPiVariable, bt);
	}
}

int writeVariableWord(jint fd,
		const char * pszVariableName,
		std::size_t len,
		uint32_t i32uValue)
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
		return writeVariable(fd, sPiVariable, len, i32uValue);
	}
}
int writeVariableBit(jint fd,
		char const * pszVariableName,
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
		return writeVariable(fd, sPiVariable, sPiVariable.i8uBit, val);
	}
}
int writeVariableBit(jint fd,
		char const * pszVariableName,
		unsigned char bt,
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
		return writeVariable(fd, sPiVariable, bt, val);
	}
}



long readVariable(jint fd, SPIVariable const & sPiVariable)
{
	int rc;
	unsigned int val;	// on only platform working (raspi), int always is
						// 32bit at least

	// length of the variable in bits. Possible values are 1, 8, 16 and 32
	if ( sPiVariable.i16uLength < 16 ){
		// shortcut in order to avoid seek & read: a single ioctl is
		// supposed to do the trick as well
		SPIValue sPiValue;
		sPiValue.i16uAddress = sPiVariable.i16uAddress;
		sPiValue.i8uBit = sPiVariable.i16uLength;
		rc = piControlGetRaw(fd, sPiValue);
		val = sPiValue.i8uValue;
	}
	else{
		// Note: the comment "// 0-7 bit position, >= 8 whole byte" lets expect
		// reading more than one byte is not possible via ioctl. As type within
		// SPIValue i8uValue declares too. For that, the split is performed
		rc = piControlGet(fd,
				sPiVariable.i16uAddress,
				sPiVariable.i8uBit / 8,	// number of bytes, coded in
										// bits before (1,8,16,32)
				(uint8_t*)&val);
	}
	if (rc < 0){
		return rc;
	}else {
		return val;
	}
}
int readVariable(jint fd,
		SPIVariable const & sPiVariable,
		unsigned char bt)
{
	int rc;

	SPIValue sPIValue;
	sPIValue.i16uAddress = sPiVariable.i16uAddress;
	sPIValue.i8uBit = bt;
	rc = piControlGet(fd, sPIValue);
	if (rc < 0){
		return rc;
	}else {
		return sPIValue.i8uValue;
	}
}

int writeVariable(jint fd,
		SPIVariable const & sPiVariable,
		std::size_t len,
		uint32_t val)
{
	int rc;

	// only chosen length are permitted
	if (len != 1 && len != 2 && len != 4) {
		return -3;
	}
	rc = piControlSet(fd,
			sPiVariable.i16uAddress,
			len,
			(uint8_t const *) & val);
	if (rc < 0) {
		return -2;
	}
	else {
		return 0;
	}
}
int writeVariable(jint fd,
		SPIVariable const & sPiVariable,
		unsigned char bt,
		bool val)
{
	int rc;
	SPIValue sPIValue;

	sPIValue.i16uAddress = sPiVariable.i16uAddress;
	sPIValue.i8uBit = bt;
	sPIValue.i8uValue = (val?1:0);
	rc = piControlSet(fd, sPIValue);
	if (rc < 0)
		return -2;
	else{
		return 0;
	}
}
