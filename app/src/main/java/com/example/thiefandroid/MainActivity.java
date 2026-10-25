package com.example.thiefandroid;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("thief_android");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView view = new TextView(this);
        view.setText("Thief Android\nNative runtime starting...");
        view.setTextSize(22);
        view.setGravity(android.view.Gravity.CENTER);
        setContentView(view);
    }
}
