/* Utils
 * Author: KangLin(kl222@!26.com) 
 */

package org.KangLinStudio.QtAndroidUtils;

import android.Manifest;
import android.app.Activity;
import android.content.ComponentName;
import android.content.ContentValues;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.database.Cursor;
import android.os.Build;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.content.Intent;
import android.support.v4.content.FileProvider;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.net.Uri;
import android.util.Log;

public class Utils {
    private static final String TAG = "Utils";
    
    /**
    * Checks if the app has permission to read and write to device storage
    * If the app does not has permission then the user will be prompted to
    * grant permissions, When android > 6.0(SDK API > 23)
    *
    * @param activity
    */
    public static void verifyStoragePermissions(final Activity activity) {
        // Storage Permissions
        int REQUEST_EXTERNAL_STORAGE = 1;
        String[] PERMISSIONS_STORAGE = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};
        
        Activity a = (Activity)activity;
        int permissionRead = ActivityCompat.checkSelfPermission(a,
            Manifest.permission.READ_EXTERNAL_STORAGE);
            
        // Check if we have write permission
        int permissionWrite = ActivityCompat.checkSelfPermission(a,
            Manifest.permission.WRITE_EXTERNAL_STORAGE);
    
        if (PackageManager.PERMISSION_GRANTED != permissionRead
            || PackageManager.PERMISSION_GRANTED != permissionWrite) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(a, PERMISSIONS_STORAGE,
                REQUEST_EXTERNAL_STORAGE);
            }
    }

    /**
    * Checks if the app has permission to use camera
    * If the app does not has permission then the user will be prompted to
    * grant permissions, When android > 6.0(SDK API > 23)
    *
    * @param activity
    */
    public static void verifyCameraPermissions(final Activity activity) {
        int REQUEST_CODE = 2;
        String[] PERMISSIONS = {
            Manifest.permission.CAMERA};
        
        Activity a = (Activity)activity;
        int permissionCamera = ActivityCompat.checkSelfPermission(a,
            Manifest.permission.CAMERA);
            
       
        if (PackageManager.PERMISSION_GRANTED != permissionCamera) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(a, PERMISSIONS,
                REQUEST_CODE);
            }
    }

    //https://blog.csdn.net/l465659833/article/details/53469441
    //https://blog.csdn.net/H176Nhx7/article/details/79358290
    public static void share(Activity activity,
                             String title,
                             String subject,
                             String content,
                             String htmlContent,
                             String[] files)
    {
        Intent shareIntent = new Intent(Intent.ACTION_SEND_MULTIPLE);
        if(null == files || files.length == 0) {
            shareText(activity, title, subject, content, htmlContent);
        } else {
            sharePicture(activity, title, subject, content, htmlContent, files);
        }
    }

    //https://blog.csdn.net/wqjsir/article/details/25538841
    public static void shareText(Activity activity,
                                 String title,
                                 String subject,
                                 String content,
                                 String htmlContent) {
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        shareIntent.setType("text/plain");
        shareIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
        shareIntent.putExtra(Intent.EXTRA_TEXT, content);
        shareIntent.putExtra(Intent.EXTRA_HTML_TEXT, htmlContent);
        activity.startActivity(Intent.createChooser(shareIntent, title));
    }

    public static void sharePicture(Activity activity,
                                    String title,
                                    String subject,
                                    String content,
                                    String htmlContent,
                                    String[]files){
        if(0 == files.length || null == files)
            return;
        Intent shareIntent = new Intent(Intent.ACTION_SEND_MULTIPLE);
        shareIntent.setType("image/*");

        ArrayList<Uri> imgUris = new ArrayList<Uri>();
        for (String s : files) {
            File f = new File(s);
            Uri u;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                //u = FileProvider.getUriForFile(activity, activity.getPackageName()
                //        + ".fileprovider", new File(s));
                u = getImageContentUri(activity, new File(s));
            } else {
                u = Uri.fromFile(f);
            }
            imgUris.add(u);
        }
        if(!imgUris.isEmpty())
            shareIntent.putParcelableArrayListExtra(Intent.EXTRA_STREAM, imgUris);

        shareIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
        shareIntent.putExtra(Intent.EXTRA_TEXT, content);
        shareIntent.putExtra(Intent.EXTRA_HTML_TEXT, htmlContent);

        activity.startActivity(Intent.createChooser(shareIntent, title));
    }

    /**
     *
     * @param context
     * @param imageFile
     * @return content Uri
     */
    private static Uri getImageContentUri(Context context, File imageFile) {
        String filePath = imageFile.getAbsolutePath();
        Cursor cursor = context.getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
                new String[]{MediaStore.Images.Media._ID}, MediaStore.Images.Media.DATA + "=? ",
                new String[]{filePath}, null);
        if (cursor != null && cursor.moveToFirst()) {
            int id = cursor.getInt(cursor.getColumnIndex(MediaStore.MediaColumns._ID));
            Uri baseUri = Uri.parse("content://media/external/images/media");
            return Uri.withAppendedPath(baseUri, "" + id);
        } else {
            if (imageFile.exists()) {
                ContentValues values = new ContentValues();
                values.put(MediaStore.Images.Media.DATA, filePath);
                return context.getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
            } else {
                return null;
            }
        }
    }

    private static List<Intent> checkShareActivity(Context context, Intent intent, String[][] packageName){
        if(null == packageName)
            return null;

        List<ResolveInfo> resInfo = context.getPackageManager().queryIntentActivities(
                intent, 0);

        List<Intent> targetIntents = new ArrayList<>();
        if (!resInfo.isEmpty()) {
            for (ResolveInfo info : resInfo) {
                ActivityInfo activityInfo = info.activityInfo;
                for(int i = 0; i < packageName.length; i++){
                    if(activityInfo.packageName.contains(packageName[i][0])
                        && activityInfo.name.contains(packageName[i][1])) {
                        Intent it = new Intent(intent.getAction());
                        it.setType(intent.getType());
                        ComponentName com = new ComponentName(activityInfo.packageName,
                                activityInfo.name);
                        it.setComponent(com);
                        targetIntents.add(it);
                    }
                }
            }
        }
        return targetIntents;
    }

    private static void showShareActivity(Context context, Intent intent) {
        List<ResolveInfo> resInfo = context.getPackageManager().queryIntentActivities(
                intent, 0);

        if (!resInfo.isEmpty()) {
            for (ResolveInfo info : resInfo) {
                ActivityInfo activityInfo = info.activityInfo;
                Log.d(TAG, "PackageName:" + activityInfo.packageName);
                Log.d(TAG, "ClassName: " + activityInfo.name);
            }
        }
    }

}
