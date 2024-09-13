package com.example.zabbixtrapperndk

import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier

@Composable
fun SenderView(modifier: Modifier = Modifier) {
    val trapper = NativeTrapper()
    trapper.init("192.168.43.42")
    trapper.send("Zabbix server", "zabbix_trapper", "Hello Native Android!")
    Text(
        text = "Hello Zabbix",
        modifier = modifier
    )
}