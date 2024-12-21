package com.example.wordclockcolor

import android.net.ConnectivityManager
import android.os.AsyncTask
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import yuku.ambilwarna.AmbilWarnaDialog
import java.io.DataOutputStream
import java.io.IOException
import java.net.Socket


class MainActivity : AppCompatActivity() {

    private var mDefaultColor = 0
    private var ForgroundColor = 0
    private var BackgroundColor = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
        val button_forground: Button = findViewById(R.id.button_forground)
        val button_background: Button = findViewById(R.id.button_background)
        val textView_forground: TextView = findViewById(R.id.textView_forground)
        val textView_background: TextView = findViewById(R.id.textView_background)

        button_forground.setOnClickListener (){
                    openColorPickerDialogue(1, textView_forground)
        }

        button_background.setOnClickListener (){
            openColorPickerDialogue(2, textView_background)
        }
    }

    // the dialog functionality is handled separately
    // using openColorPickerDialog this is triggered as
    // soon as the user clicks on the Pick Color button And
    // the AmbilWarnaDialog has 2 methods to be overridden
    // those are onCancel and onOk which handle the "Cancel"
    // and "OK" button of color picker dialog
    fun openColorPickerDialogue(variant:Int, textView_color: TextView) {

        // the AmbilWarnaDialog callback needs 3 parameters
        // one is the context, second is default color,
        val colorPickerDialogue = AmbilWarnaDialog(this, mDefaultColor,
            object : AmbilWarnaDialog.OnAmbilWarnaListener {
                override fun onCancel(dialog: AmbilWarnaDialog?) {
                    // leave this function body as
                    // blank, as the dialog
                    // automatically closes when
                    // clicked on cancel button
                }

                override fun onOk(dialog: AmbilWarnaDialog?, color: Int) {
                    // change the mDefaultColor to
                    // change the GFG text color as
                    // it is returned when the OK
                    // button is clicked from the
                    // color picker dialog
                    if(variant == 1){
                        ForgroundColor = color;
                        textView_color.setTextColor(ForgroundColor)
                        Log.d("meineAPP_WordClolckColor", "ForgroundColor: $ForgroundColor")
                    }else{
                        BackgroundColor = color;
                        textView_color.setTextColor(BackgroundColor)
                        Log.d("meineAPP_WordClolckColor", "BackgroundColor: $BackgroundColor")
                    }
                    if (internetAvailable()) {
                        Toast.makeText(applicationContext, "color: " + Integer.toHexString(color), Toast.LENGTH_SHORT).show()
                        UploaddataAsyncTask(variant,color).execute()
                    } else {
                        Toast.makeText(applicationContext, "Kein Internet vorhanden", Toast.LENGTH_LONG).show()
                    }

                    // now change the picked color
                    // preview box to mDefaultColor
                }
            })
        colorPickerDialogue.show()
    }

    private fun internetAvailable(): Boolean {
        try {
            val connectivityManager = getSystemService(CONNECTIVITY_SERVICE) as ConnectivityManager
            val networkInfo = connectivityManager.activeNetworkInfo
            return networkInfo != null && networkInfo.isConnected
        } catch (e: Exception) {
            e.printStackTrace()
            Log.d("meineAPP_WordClolckColor", "Fehler beim Internetcheck")
            return false
        }
    }

    /* a new task for upload the photo */
    class UploaddataAsyncTask(private var variant: Int, private var color: Int) : AsyncTask<Any?, Any?, Any?>() {
        var SERVER_IP: String = "word-clock" // configured in fritzbox
        var SERVER_PORT: Int = 80
        /* handles everthing in the async task */
        override fun doInBackground(params: Array<Any?>): Any? {
            try {
                Log.d("meineAPP_WordClolckColor", "Starte Verbindung")
                /* conncet to server */
                val socketClient: Socket = Socket(SERVER_IP, SERVER_PORT)
                Log.d("meineAPP_WordClolckColor", ("Start connection to $SERVER_IP").toString() + " on Port " + SERVER_PORT.toString())

                /* create input- and outputstream */
                val socket_output = DataOutputStream(socketClient.getOutputStream())

                socket_output.writeInt(color)
                socket_output.write((variant.toByte()).toInt())

                socket_output.close()
                Log.d("meineAPP_WordClolckColor", "Close Socket ...")
            } catch (e: IOException) {
                // TODO Auto-generated catch block
                e.printStackTrace()
                Log.d("meineAPP_WordClolckColor", "Fehler beim empfgang...$e")
            }
            return null
        }
    }
}
