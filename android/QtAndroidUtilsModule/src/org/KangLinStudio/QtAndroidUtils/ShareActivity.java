package org.KangLinStudio.QtAndroidUtils;

import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.util.ArrayList;

public class ShareActivity extends AppCompatActivity {
    private static final String TAG = "ShareActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_share);

        Intent intent = getIntent();
        String action = intent.getAction();
        String type = intent.getType();
        if(Intent.ACTION_SEND.equals(action)) {
            if(type.equals("text/plain")){
                handleSendText(intent);
            }
            if(type.startsWith("image/")) {
                handleSendImage(intent);
            }
            if(type.startsWith("video/")){
                handleSendVideo(intent);
            }

            if(type.startsWith("audio")) {
                handleSendAudio(intent);
            }
        }
        if(Intent.ACTION_SEND_MULTIPLE.equals(action)){
            //Don't check type, judging by file type
            handleSendMultiple(intent);
        }
    }

    private void handleSendText(Intent intent){
        String subject = intent.getStringExtra(Intent.EXTRA_SUBJECT);
        String text = intent.getStringExtra(Intent.EXTRA_TEXT);
        String htmlText = intent.getStringExtra(Intent.EXTRA_HTML_TEXT);

        Log.d(TAG, "Subject:" + subject + ";text: " + text + ";htmlText: " + htmlText);
    }

    private void handleSendImage(Intent intent){
        Uri imageUri = intent.getParcelableExtra(Intent.EXTRA_STREAM);
        if(null == imageUri)
            return;

        String subject = intent.getStringExtra(Intent.EXTRA_SUBJECT);
        String text = intent.getStringExtra(Intent.EXTRA_TEXT);
        String htmlText = intent.getStringExtra(Intent.EXTRA_HTML_TEXT);
        Log.d(TAG, "Subject:" + subject + ";text: " + text + ";htmlText: " + htmlText);
    }

    private void handleSendVideo(Intent intent){

    }

    private void handleSendAudio(Intent intent){

    }

    private void handleSendMultiple(Intent intent){
        ArrayList<Uri> imageUris = intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM);
        if(null == imageUris)
            return;

        String subject = intent.getStringExtra(Intent.EXTRA_SUBJECT);
        String text = intent.getStringExtra(Intent.EXTRA_TEXT);
        String htmlText = intent.getStringExtra(Intent.EXTRA_HTML_TEXT);
        Log.d(TAG, "Subject:" + subject + ";text: " + text + ";htmlText: " + htmlText);
    }
}
