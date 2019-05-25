package com.wordclock.scherer.wordclock;

import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.ImageButton;
import android.widget.Toast;

import com.larswerkman.holocolorpicker.ColorPicker;
import com.larswerkman.holocolorpicker.OpacityBar;
import com.larswerkman.holocolorpicker.SVBar;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    static String SERVER_IP = "192.168.2.126";
    static int SERVER_PORT  = 80;

    ImageButton button_led_off;
    ImageButton button_led_on;

    SVBar svBar;
    ColorPicker picker;
    OpacityBar opacityBar;

    int button_var_press = 0;
    int color;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_ACTION_BAR);
        setContentView(R.layout.activity_main);
        getSupportActionBar().hide();

        svBar = (SVBar) findViewById(R.id.svbar);
        picker = (ColorPicker) findViewById(R.id.picker);
        opacityBar = (OpacityBar) findViewById(R.id.opacitybar);

        button_led_off = (ImageButton) findViewById(R.id.button_off);
        button_led_on  = (ImageButton) findViewById(R.id.button_on);

        picker.addSVBar(svBar);
        picker.addOpacityBar(opacityBar);
        picker.setShowOldCenterColor(false);

        button_led_on.setOnClickListener(new View.OnClickListener(){
            private static final long THRESHOLD_MILLIS = 1000L;
            private long lastClickMillis;

            @Override
            public void onClick(View v) {
                long now = SystemClock.elapsedRealtime();
                if (now - lastClickMillis > THRESHOLD_MILLIS) {
                    if(internetAvailable()) {
                        color = picker.getColor();
                        button_var_press = 1;
                        Toast.makeText(getApplicationContext(), "color: " + Integer.toHexString(color),  Toast.LENGTH_SHORT).show();
                        new UploaddataAsyncTask().execute();
                    }else {
                        Toast.makeText(getApplicationContext(), "Kein Internet vorhanden", Toast.LENGTH_LONG).show();
                    }
                }
                lastClickMillis = now;
            }
        });

        button_led_off.setOnClickListener(new View.OnClickListener(){
            private static final long THRESHOLD_MILLIS = 1000L;
            private long lastClickMillis;

            @Override
            public void onClick(View v) {
                long now = SystemClock.elapsedRealtime();
                if (now - lastClickMillis > THRESHOLD_MILLIS) {
                    if(internetAvailable()) {
                        color = picker.getColor();
                        button_var_press = 2;
                        Toast.makeText(getApplicationContext(), "color: " + Integer.toHexString(color),  Toast.LENGTH_SHORT).show();
                        new UploaddataAsyncTask().execute();
                    }else {
                        Toast.makeText(getApplicationContext(), "Kein Internet vorhanden", Toast.LENGTH_LONG).show();
                    }
                }
                lastClickMillis = now;
            }
        });
    }

    /* a new task for upload the photo */
    public class UploaddataAsyncTask extends AsyncTask {
        /* handles everthing in the async task */
        @Override
        protected Object doInBackground(Object[] params) {
            try {
                Log.d("Meine App", "Starte Verbindung");
                /* conncet to server */
                Socket socket_client = new Socket(SERVER_IP, SERVER_PORT);
                Log.d("Meine App", "Start connection to " + SERVER_IP + " on Port " + Integer.toString(SERVER_PORT));

                /* create input- and outputstream */
                DataOutputStream socket_output = new DataOutputStream(socket_client.getOutputStream());

                socket_output.writeInt(color);
                socket_output.write((byte) button_var_press);

                socket_output.close();
                Log.d("Meine App", "Close Socket ...");

            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
                Log.d("Meine App", "Fehler beim empfgang...");
            }
            return null;
        }
    }

    private boolean internetAvailable(){
        try{
            ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(CONNECTIVITY_SERVICE);
            NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
            return networkInfo != null && networkInfo.isConnected();
        } catch (Exception e) {
            e.printStackTrace();
            Log.d("Meine App","Fehler beim Internetcheck");
            return false;
        }
    }
}
