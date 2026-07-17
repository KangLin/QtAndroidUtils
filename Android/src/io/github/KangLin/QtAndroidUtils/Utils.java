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
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.content.Intent;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.net.Uri;
import android.support.v4.content.FileProvider;
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

    /**
     * Install apk
     * See: https://www.cnblogs.com/newjeremy/p/7294519.html
     *      https://blog.csdn.net/hxy_sakura/article/details/80090933
     * @param context
     * @param filePath: apk file
     */
    public static void install(Context context, String filePath) {
        Log.i(TAG, "install: " + filePath);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            boolean b = context.getPackageManager().canRequestPackageInstalls();
            if(!b){
                Intent intent = new Intent(Settings.ACTION_MANAGE_UNKNOWN_APP_SOURCES);
                context.startActivity(intent);
            }
        }

        File apkFile = new File(filePath);
        String type = "application/vnd.android.package-archive";
        
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            Log.w(TAG, "> N ，use fileProvider to install");
            intent.setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            Uri contentUri =/*getImageContentUri(context, apkFile);*/
                FileProvider.getUriForFile(
                    context
                    ,  context.getPackageName() + ".FileProvider"
                    , apkFile);
            intent.setDataAndType(contentUri, type);
        } else {
            intent.setDataAndType(Uri.fromFile(apkFile), type);
        }
        context.startActivity(intent);
    }

    public static void uninstall(Context context, String pacageName){
        Intent intent=new Intent(Intent.ACTION_DELETE);
        intent.setData(Uri.parse("package:" + pacageName));
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
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
        } else if(files.length == 1) {
            showOnePicture(activity, title, subject, content, htmlContent, files[0]);
        } else {
            sharePicture(activity, title, subject, content, htmlContent, files);
        }
    }

    private static void setShareIntent(Intent intent,
                                       String subject,
                                       String content,
                                       String htmlContent){
        intent.putExtra(Intent.EXTRA_SUBJECT, subject);
        intent.putExtra(Intent.EXTRA_TEXT, content);
        intent.putExtra(Intent.EXTRA_HTML_TEXT, htmlContent);
    }

    //https://blog.csdn.net/wqjsir/article/details/25538841
    public static void shareText(Activity activity,
                                 String title,
                                 String subject,
                                 String content,
                                 String htmlContent) {
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        shareIntent.setType("text/plain");
        setShareIntent(shareIntent, subject, content, htmlContent);
        if(shareIntent.resolveActivity(activity.getPackageManager()) != null)
            activity.startActivity(Intent.createChooser(shareIntent, title));
    }

    public static void showOnePicture(Activity activity,
                                      String title,
                                      String subject,
                                      String content,
                                      String htmlContent,
                                      String file){
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        shareIntent.setType("image/*");
        setShareIntent(shareIntent, subject, content, htmlContent);
        File f = new File(file);
        Uri u = Uri.fromFile(f);
        shareIntent.putExtra(Intent.EXTRA_STREAM, u);
        if(shareIntent.resolveActivity(activity.getPackageManager()) != null)
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
        if (!imgUris.isEmpty())
            shareIntent.putParcelableArrayListExtra(Intent.EXTRA_STREAM, imgUris);

        setShareIntent(shareIntent, subject, content, htmlContent);
        if(shareIntent.resolveActivity(activity.getPackageManager()) != null)
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
