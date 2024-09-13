// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("zabbixtrapperndk");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("zabbixtrapperndk")
//      }
//    }

#include <jni.h>
#include <string>
//#include <boost/asio.hpp>
//#include <boost/atomic.hpp>
//#include <boost/chrono.hpp>

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

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_zabbixtrapperndk_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    LittleTrap ZT(std::string("192.168.43.42"), 10051, true, 150);
    ZT.setter(std::string("Zabbix server"), std::string("zabbix_trapper"));
    ZT.sendData<std::string>("a", 1);

    std::string hello = "C++";
//    std::atomic<bool> ready;
    return env->NewStringUTF(hello.c_str());
}