/*
Author: Kang Lin (kl222@126.com)

See: https://sq.163yun.com/blog/article/192705627588341760
     https://blog.csdn.net/wangyiyungw/article/details/84615117
     https://blog.csdn.net/u013651026/article/details/79201962
*/

package org.KangLinStudio.QtAndroidUtils;

import android.annotation.TargetApi;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.drawable.Icon;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.util.Log;

/**
 * Helper class for showing and canceling message
 * notifications.
 */
public class MessageNotification {
    /**
     * The unique identifier for this type of notification.
     */
    private static final String TAG = "MessageNotification";
    private static final String ACTION_ON_CHICK = "org.KangLinStudio.QtAndroidUtils.MessageNotification.OnClick";
    
    private static BroadcastReceiver m_Receiver = null;
    private static native void MessageNotificationOnClickCallBack(int id);
    private class MessageNotificationBroadcastReceiver extends BroadcastReceiver{
        
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(!action.equals(ACTION_ON_CHICK)) return;
            int id = intent.getIntExtra("id", -1);
            if(-1 == id) {
                Log.e(TAG, "Get int Extra fail");
                return;
            }
            
            /* Start QtActivity
             * The following setting must be set in AndroidManifest.xml:
             * <activity android:launchMode="singleInstance" />
             */
            Intent intentActivity = new Intent(context, context.getClass());
            context.startActivity(intentActivity);
            
            // Call back native function
            MessageNotificationOnClickCallBack(id);
        }
    }

    public void init(final Context context){
        m_Receiver = new MessageNotificationBroadcastReceiver();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(ACTION_ON_CHICK);
        context.registerReceiver(m_Receiver, intentFilter);
    }

    public void clean(final Context context){
        context.unregisterReceiver(m_Receiver);
    }

    private static PendingIntent getPendingIntent(final Context context,
                                                  final int id,
                                                  final boolean callBack) {
        PendingIntent pi;
        if(callBack)
        {
            Intent i = new Intent(ACTION_ON_CHICK);
            i.putExtra("id", id);
            pi = PendingIntent.getBroadcast (
                                  context,
                                  0,
                                  i,
                                  PendingIntent.FLAG_UPDATE_CURRENT);
        } else {
            pi = PendingIntent.getActivity(
                                  context,
                                  0,
                                  new Intent(context, context.getClass()),
                                  PendingIntent.FLAG_UPDATE_CURRENT);
        }
        return pi;
    }

    /**
     * Shows the notification, or updates a previously shown notification of
     * this type, with the given parameters.
     * <p>
     * TODO: Customize this method's arguments to present relevant content in
     * the notification.
     * <p>
     * TODO: Customize the contents of this method to tweak the behavior and
     * presentation of message notifications. Make
     * sure to follow the
     * <a href="https://developer.android.com/design/patterns/notifications.html">
     * Notification design guidelines</a> when doing so.
     *
     * @see #cancel(Context)
     */
    @TargetApi(Build.VERSION_CODES.M) // version 26
    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id,
                              final Bitmap smallIcon,
                              final Bitmap largeIcon,
                              final boolean callBack
                              )
    {
        final Notification.Builder builder = new Notification.Builder(context)
                .setDefaults(Notification.DEFAULT_ALL)
                .setContentTitle(title)
                .setContentText(text)
                .setPriority(Notification.PRIORITY_DEFAULT)
                .setTicker(text)         // Show text in status bar
                .setNumber(number)
                .setAutoCancel(true)
                .setWhen(System.currentTimeMillis())
                .setOngoing(true)
                ;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (null == smallIcon)
                Log.e(TAG, "SmallIcon is null, must set setSmallIcon");
            else {
                Icon icon = Icon.createWithBitmap(smallIcon);
                if(null == icon)
                    Log.e(TAG, "icon is null");
                builder.setSmallIcon(icon);
            }
        } else {
            builder.setSmallIcon(R.drawable.icon);
        }

        if(null == largeIcon)
            Log.d(TAG, "largeIcon is null");
        else
            builder.setLargeIcon(largeIcon);

        // Set the pending intent to be initiated when the user touches
        // the notification.
        PendingIntent pi = getPendingIntent(context, id, callBack);
        builder.setContentIntent(pi);
        notify(context, builder.build(), id);            
    }

    /*
     * Small and large icon default use resources: R.drawable.app
     */
    @TargetApi(Build.VERSION_CODES.M)
    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id,
                              final boolean callBack) {
        final Resources res = context.getResources();
        final Bitmap largeIcon;
        largeIcon = BitmapFactory.decodeResource(res, R.drawable.icon);
        notify(context, text, title, number, id, largeIcon, largeIcon, callBack);
    }

    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id,
                              final int smallIconId,
                              final int largeIconId,
                              final boolean callBack) {
        final Resources res = context.getResources();
        final Bitmap smallIcon = BitmapFactory.decodeResource(res, smallIconId);
        final Bitmap largeIcon = BitmapFactory.decodeResource(res, largeIconId);
        notify(context, text, title, number, id, smallIcon, largeIcon, callBack);
    }

    @TargetApi(Build.VERSION_CODES.M)
    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id,
                              final String smallIconFile,
                              final String largeIconFile,
                              final boolean callBack) {
        final Bitmap largeIcon = BitmapFactory.decodeFile(largeIconFile);
        final Bitmap smallIcon = BitmapFactory.decodeFile(smallIconFile);
        notify(context, text, title, number, id, smallIcon, largeIcon, callBack);
    }

    @TargetApi(Build.VERSION_CODES.ECLAIR)
    private static void notify(final Context context,
                               final Notification notification,
                               final int id) {
        final NotificationManager nm = (NotificationManager) context
                .getSystemService(Context.NOTIFICATION_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR) {
            nm.notify(TAG, id, notification);
        } else {
            nm.notify(id, notification);
        }
    }

    /**
     * Cancels any notifications of this type previously shown using
     * {@link #notify(Context, String, int)}.
     */
    @TargetApi(Build.VERSION_CODES.ECLAIR)
    public static void cancel(final Context context, int id) {
        final NotificationManager nm = (NotificationManager) context
                .getSystemService(Context.NOTIFICATION_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR) {
            nm.cancel(TAG, id);
        } else {
            nm.cancel(id);
        }
    }

    /**
     * Cancels all notifications of this type previously shown using
     * {@link #notify(Context, String, int)}.
     */
    @TargetApi(Build.VERSION_CODES.ECLAIR)
    public static void cancelAll(final Context context){
        final NotificationManager nm = (NotificationManager) context
                .getSystemService(Context.NOTIFICATION_SERVICE);
        nm.cancelAll();
    }
}
