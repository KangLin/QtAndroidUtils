/* Utils
 * Author: KangLin(kl222@!26.com) 
 */

package org.KangLinStudio.QtAndroidUtils;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.content.Intent;
import android.text.Html;
import java.lang.Object;

public class Utils {
   
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

    static public void shareText(Activity activity,
                                 String title,
                                 String subject, 
                                 String content)
    {
        Intent share = new Intent(Intent.ACTION_SEND);
        share.setType("text/plain");
    
        share.putExtra(Intent.EXTRA_SUBJECT, subject);
        share.putExtra(Intent.EXTRA_TEXT, Html.fromHtml(content).toString());
        share.putExtra(Intent.EXTRA_HTML_TEXT, content);
        activity.startActivity(Intent.createChooser(share, title));
    }
}
