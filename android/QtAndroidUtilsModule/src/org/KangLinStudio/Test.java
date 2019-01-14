package org.KangLinStudio;

import android.util.Log;
import java.util.Timer;
import java.util.TimerTask;

public class Test {
    private static final String TAG = "Test";
    private  static Timer timer = new Timer();
    private static int n = 0;

    private static TimerTask task = new TimerTask() {
           @Override
           public void run() {
               Log.d(TAG, "number: " + n++);
           }
   };

   public static void timeOut(){
       timer.schedule(task, 0,1000);
   }
}
