#include "org_clehne_revpi_ledio_LedInOut.h"
#include "revpi.h"
#include "piControlFunc.h"
#include "piControl.h"

#include "jni.h"

extern "C" {
#include <unistd.h>
#include <fcntl.h>
}

#include <string>
#include <cstring>


/******************************************************************************/
/*******************************  Functions  **********************************/
/******************************************************************************/
static const std::string RevPiLedChanName = "RevPiLED";

JNIEXPORT jint JNICALL Java_org_clehne_revpi_ledio_LedInOut__1open(
		JNIEnv *env, jclass obj) {
	(void) env;
	(void) obj;
	int PiControlHandle = -1;
	PiControlHandle = open(PICONTROL_DEVICE, O_RDWR);
    if (PiControlHandle < 0) {
    	throwIOExceptionMsg(env, "Could not open revpi ledinout.");
    }
    return PiControlHandle;
}

JNIEXPORT void Java_org_clehne_revpi_ledio_LedInOut__1close
(JNIEnv *env, jclass obj, jint fd){
	(void) env;
	(void) obj;
    /* open handle if needed */
    if (fd > 0) {
    	close(fd);
    }else{
    	throwIOExceptionMsg(env, "Could not close. Invalid handle");
    }
	return;
}

JNIEXPORT jint JNICALL Java_org_clehne_revpi_ledio_LedInOut__1setValue
  (JNIEnv *env, jclass obj, jint fd, jint value){
	int ret;

	(void) env;
	(void) obj;
	ret = writeVariableValue(fd, RevPiLedChanName.c_str(), value);
	return ret;
}

JNIEXPORT jint JNICALL Java_org_clehne_revpi_ledio_LedInOut__1getValue
  (JNIEnv *env, jclass obj, jint fd){
	int ret;

	(void) env;
	(void) obj;
	ret = readVariableValue(fd, RevPiLedChanName.c_str());
	return ret;
}

JNIEXPORT jint JNICALL Java_org_clehne_revpi_ledio_LedInOut__1setBit
	(JNIEnv *env, jclass obj, jint fd, jint bit, jint value) {
	int ret;

	(void) env;
	(void) obj;
	if(bit < 0){
		return -1;
	}
	if(value < 0 || value > 1){
		return -2;
	}
	ret = writeVariableBit(fd, RevPiLedChanName.c_str(), bit, ((value > 0)?true:false));
	return ret;
}

JNIEXPORT jint JNICALL Java_org_clehne_revpi_ledio_LedInOut__1getBit
	(JNIEnv *env, jclass obj, jint fd, jint bit) {
	int ret;

	(void) env;
	(void) obj;
	if(bit < 0){
		return -1;
	}
	ret = readVariableBit(fd, RevPiLedChanName.c_str(), bit);
	return ret;
}
