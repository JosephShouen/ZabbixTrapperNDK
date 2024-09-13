package com.example.zabbixtrapperndk

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.ui.Modifier
import com.example.zabbixtrapperndk.ui.theme.ZabbixTrapperNDKTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            ZabbixTrapperNDKTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    SenderView(
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}