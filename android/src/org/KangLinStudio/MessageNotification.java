package org.KangLinStudio;

import android.annotation.TargetApi;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.drawable.Icon;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.util.Log;
import org.qtproject.qt5.android.bindings.QtActivity;

/**
 * Helper class for showing and canceling message
 * notifications.
 * <p>
 * This class makes heavy use of the {@link NotificationCompat.Builder} helper
 * class to create notifications in a backward-compatible way.
 */
public class MessageNotification {
    /**
     * The unique identifier for this type of notification.
     */
    private static final String NOTIFICATION_TAG = "Message";
    private final static String ACTION_ON_CHICK = "org.KangLinStudio.QtAndroidUtils.OnClick";
    
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
    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id,
                              final Icon smallIcon,
                              final Bitmap largeIcon
                              ) 
    {
        final Notification.Builder builder = new Notification.Builder(context)
                          
                // Set appropriate defaults for the notification light, sound,
                // and vibration.
                .setDefaults(Notification.DEFAULT_ALL)
                          
                // Set required fields, including the small icon, the
                // notification title, and text.
                
                .setContentTitle(title)
                .setContentText(text)
                          
                // All fields below this line are optional.
                          
                // Use a default priority (recognized on devices running Android
                // 4.1 or later)
                .setPriority(Notification.PRIORITY_DEFAULT)
                          
                // Provide a large icon, shown with the notification in the
                // notification drawer on devices running Android 3.0 or later.
                //.setLargeIcon(picture)
                          
                // Set ticker text (preview) information for this notification.
                .setTicker(text)
                          
                // Show a number. This is useful when stacking notifications of
                // a single type.
                .setNumber(number)
                
                // If this notification relates to a past or upcoming event, you
                // should set the relevant time information using the setWhen
                // method below. If this call is omitted, the notification's
                // timestamp will by set to the time at which it was shown.
                // TODO: Call setWhen if this notification relates to a past or
                // upcoming event. The sole argument to this method should be
                // the notification timestamp in milliseconds.
                //.setWhen(...)
                          
                // Automatically dismiss the notification when it is touched.
                .setAutoCancel(true)
                ;
                
        // Set the pending intent to be initiated when the user touches
        // the notification.         
        builder.setContentIntent(
            PendingIntent.getActivity(
                context,
                0,
                new Intent(context, QtActivity.class),
                PendingIntent.FLAG_UPDATE_CURRENT)
                );
        if(null == largeIcon)
            Log.d(NOTIFICATION_TAG, "largeIcon is null");
        else
            builder.setLargeIcon(largeIcon);
        if(null == smallIcon)
            Log.d(NOTIFICATION_TAG, "smallIcon is null");
        else
            builder.setSmallIcon(smallIcon);
        notify(context, builder.build(), id);            
    }

    /*
     * Small and large icon default use resources: R.drawable.app
     */
    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id) {
        final Resources res = context.getResources();
        final Bitmap largeIcon;
        largeIcon = BitmapFactory.decodeResource(res, R.drawable.icon);
        final Icon smallIcon = Icon.createWithResource(context, R.drawable.icon);
        notify(context, text, title, number, id, smallIcon, largeIcon);
    }

    public static void notify(final Context context,
                              final String text,
                              final String title,
                              final int number,
                              final int id,
                              final String smallIconFile,
                              final String largeIconFile) {
        final Bitmap largeIcon;
        largeIcon = BitmapFactory.decodeFile(smallIconFile);
        final Icon smallIcon = Icon.createWithFilePath(smallIconFile);
        notify(context, text, title, number, id, smallIcon, largeIcon);
    }

    @TargetApi(Build.VERSION_CODES.ECLAIR)
    private static void notify(final Context context,
                               final Notification notification,
                               final int id) {
        final NotificationManager nm = (NotificationManager) context
                .getSystemService(Context.NOTIFICATION_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR) {
            nm.notify(NOTIFICATION_TAG, id, notification);
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
            nm.cancel(NOTIFICATION_TAG, id);
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
