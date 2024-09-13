#include <jni.h>
#include <string>
#include <iostream>
#include "zabbixtrapper.h"

class LittleTrap: public ZabbixTrapper
{
public:
    LittleTrap(std::string zabbixHost, int zabbixPort, bool ready = false, int ttl = 100):
            ZabbixTrapper(zabbixHost, zabbixPort, ready, ttl)
    {
        this->zabbixHost = zabbixHost;
        this->zabbixPort = zabbixPort;
        this->ready = ready;
        this->ttl = ttl;
    }
protected:
    int zabbixPort;
    int ttl;
    std::string zabbixHost;
};

std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

extern "C" {
JNIEXPORT jlong JNICALL
Java_com_example_zabbixtrapperndk_NativeTrapper_createTrapper(
        JNIEnv *env,
        jobject  /* this */,
        jstring ip) {
    return reinterpret_cast<jlong>(new LittleTrap(
            jstring2string(env, ip),
            10051,
            true,
            150));
}

JNIEXPORT void JNICALL
Java_com_example_zabbixtrapperndk_NativeTrapper_destroyTrapper(JNIEnv *env,
                                                              jobject  /* this */,
                                                              jlong littleTrapPtr) {
    delete reinterpret_cast<LittleTrap*>(littleTrapPtr);
}

JNIEXPORT jint JNICALL
Java_com_example_zabbixtrapperndk_NativeTrapper_dataSend(
        JNIEnv *env,
        jobject  /* this */,
        jlong littleTrapPtr,
        jstring host,
        jstring key,
        jstring data) {
    LittleTrap* littleTrap = reinterpret_cast<LittleTrap*>(littleTrapPtr);
    littleTrap->setter(jstring2string(env, host), jstring2string(env, key));
    littleTrap->sendData<std::string>(jstring2string(env, data), 1);
    return 0;
}
}