package org.rxlab.launcher;

import java.util.ArrayList;
import java.util.List;

import android.content.Intent;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.content.ActivityNotFoundException;
import android.widget.Toast;
import android.net.Uri;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;

public class AppManager {

    private static native void appInstalled(AppInfo app);
    private static native void appUninstalled(AppInfo app);

    public static String getAppName(Context context, String packageName) {
        final PackageManager pm = context.getPackageManager();
        ApplicationInfo ai;
        try {
            ai = pm.getApplicationInfo( packageName, 0);
        } catch (Exception e) {
            ai = null;
        }
        return (String) (ai != null ? pm.getApplicationLabel(ai) : "");
    }

    public static ArrayList<AppInfo> getAllApps(Context context) {

        final PackageManager pm = context.getPackageManager();
        List<ApplicationInfo> apps = pm.getInstalledApplications(PackageManager.GET_META_DATA);

        ArrayList<AppInfo> appsList = new ArrayList<AppInfo>();

        for( ApplicationInfo info : apps) {
            AppInfo app = new AppInfo();
            app.label = info.loadLabel(pm).toString();
            app.packageName = info.packageName.toString();
            if ((info.flags & ApplicationInfo.FLAG_SYSTEM) != 0) app.system = true;
            else app.system = false;
            appsList.add(app);
        }
        return appsList;
    }

    public static void launchApp(String packageName, Context context) {

        final PackageManager pm = context.getPackageManager();
        Intent intent = pm.getLaunchIntentForPackage(packageName);
        if (intent != null ) {
            try {
                context.startActivity(intent);
            } catch (ActivityNotFoundException err) {
                Toast t = Toast.makeText(context.getApplicationContext(),
                    "App not found.", Toast.LENGTH_SHORT);
                t.show();
            }
        }
    }

    public static void deleteApp(String packageName, Context context) {
        Intent intent = new Intent(Intent.ACTION_DELETE);
        intent.setData(Uri.parse("package:" + packageName));
        try {
            context.startActivity(intent);
        } catch (Exception err) {
            Toast t = Toast.makeText(context.getApplicationContext(),
                "Cannot uninstall this package.", Toast.LENGTH_SHORT);
            t.show();
        }
    }

    public static void initAppListener(Context c) {
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(Intent.ACTION_PACKAGE_ADDED);
        intentFilter.addAction(Intent.ACTION_PACKAGE_REMOVED);
        intentFilter.addDataScheme("package");

        c.registerReceiver(appsBroadcast, intentFilter);
    }

    public static BroadcastReceiver appsBroadcast = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            AppInfo app = new AppInfo();
            String data = intent.getData().getSchemeSpecificPart();

            app.label = getAppName(context, data);
            app.packageName = data;

            if (action.equals(Intent.ACTION_PACKAGE_ADDED)) {
                appInstalled(app);
            }
            else if (action.equals(Intent.ACTION_PACKAGE_REMOVED)){
                appUninstalled(app);
            }
        }
    };

}
