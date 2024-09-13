package com.example.zabbixtrapperndk

class NativeTrapper {

    companion object {
        init {
            System.loadLibrary("zabbixtrapperndk")
        }
    }

    private var littleTrapPtr: Long = 0
    external fun createTrapper(ip: String): Long
    external fun destroyTrapper(littleTrapPtr: Long)
    external fun dataSend(littleTrapPtr: Long,
                          host: String,
                          key: String,
                          data: String): Int

    fun init(ip: String) {
        littleTrapPtr = createTrapper(ip)
    }

    fun cleanup() {
        destroyTrapper(littleTrapPtr)
    }

    fun send(host: String, key: String, data: String): Int {
        dataSend(littleTrapPtr, host, key, data)
        return 0
    }
}