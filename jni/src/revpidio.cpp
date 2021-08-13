#include "org_clehne_revpi_dataio_DataInOut.h"
#include "revpi.h"
#include "piControl.h"

#include "jni.h"

extern "C" {
#include <unistd.h>
#include <fcntl.h>
}

/******************************************************************************/
/*******************************  Functions  **********************************/
/******************************************************************************/

JNIEXPORT jint JNICALL Java_org_clehne_revpi_dataio_DataInOut__1openDIO(
		JNIEnv *env, jclass obj) {
	int PiControlHandle = -1;
	PiControlHandle = open(PICONTROL_DEVICE, O_RDWR);
    if (PiControlHandle < 0) {
    	throwIOExceptionMsg(env, "Could not open revpi datainout.");
    }
    return PiControlHandle;
}


JNIEXPORT void Java_org_clehne_revpi_dataio_DataInOut__1close
(JNIEnv *env, jclass obj, jint fd)
{
    /* open handle if needed */
    if (fd > 0) {
    	close(fd);
    }else{
    	throwIOExceptionMsg(env, "Could not close. Invalid handle");
    }
	return;
}


JNIEXPORT jint JNICALL Java_org_clehne_revpi_dataio_DataInOut__1setDataOut
	(JNIEnv *env, jclass obj, jint fd, jint channel, jint value) {
	char channelName[20];
	int ret;
	if(channel < 1 || channel > 16){
		return -1;
	}
	if(value < 0 || value > 1){
		return -2;
	}
	snprintf(channelName, 20, "O_%d", channel);
	ret = writeVariableValue(fd, (char*)channelName, value);
	return ret;
}

JNIEXPORT jint JNICALL Java_org_clehne_revpi_dataio_DataInOut__1getDataOut
	(JNIEnv *env, jclass obj, jint fd, jint channel) {

	char channelName[20];
	int ret;
	if(channel < 1 || channel > 16){
		return -1;
	}
	snprintf(channelName, 20, "O_%d", channel);
	ret = readVariableValue(fd, (char*)channelName);
	return ret;
}

JNIEXPORT jint JNICALL Java_org_clehne_revpi_dataio_DataInOut__1getDataIn
	(JNIEnv *env, jclass obj, jint fd, jint channel) {

	char channelName[20];
	int ret;
	if(channel < 1 || channel > 16){
		return -1;
	}
	snprintf(channelName, 20, "I_%d", channel);
	ret = readVariableValue(fd, (char*)channelName);
	return ret;
}
