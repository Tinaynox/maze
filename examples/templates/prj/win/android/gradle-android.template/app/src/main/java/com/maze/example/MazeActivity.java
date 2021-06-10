package com.maze.example;

import androidx.appcompat.app.AppCompatActivity;

import android.app.NativeActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import android.content.Context;
import android.os.Build;
import android.content.pm.*;
import java.util.Locale;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.ArrayList;

import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

import java.security.MessageDigest;
import java.util.Base64;
import java.util.Base64;
//import java.util.Arrays;


public class MazeActivity extends NativeActivity
{

	private static final String LOG_TAG = "MazeEngine";
	private static Context sContext;

    private String mApkFilePath;
    private ArrayList<String> mAssetFilesList = new ArrayList<>();
    private String mAssetsHash;

    // Used to load the 'maze-native-lib' library on application startup.
    static {
		Log.v(LOG_TAG, "Loading maze-native-lib...");
		System.loadLibrary("z");
        System.loadLibrary("maze-native-lib");
		Log.v(LOG_TAG, "maze-native-lib loaded.");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        MazeActivity.sContext = getApplicationContext();

        Log.v(LOG_TAG, "onCreate");
        Log.v(LOG_TAG, "onCreate");

        super.onCreate(savedInstanceState);

        listAssetFiles("data", mAssetFilesList );

        mAssetsHash = "";
        try
        {
            PackageInfo packageInfo = getPackageManager().getPackageInfo( sContext.getPackageName(), 0 );
            mApkFilePath = packageInfo.applicationInfo.publicSourceDir;
            Log.v(LOG_TAG, "apkFilePath="+mApkFilePath);

            ZipFile zip = null;
            InputStream in = null;
            try
            {
                zip = new ZipFile( mApkFilePath );

                for( String assetFileName : mAssetFilesList )
                {
                    String assetFileNameInAPK = "assets/" + assetFileName;
                    ZipEntry manifestEntry = zip.getEntry( assetFileNameInAPK );
                    if (manifestEntry == null)
                    {
                        Log.e(LOG_TAG, "Cannot find " + assetFileNameInAPK + " in apk");
                        continue;
                    }

                    long crc32 = manifestEntry.getCrc();

                    String hex = Long.toHexString( crc32 );
                    mAssetsHash += hex;
                }
            }
            finally {

                try
                {
                    if( zip != null )
                    {
                        zip.close();
                    }
                }
                catch (IOException ignore)
                {
                    /* ignore */
                }
            }
        }
        catch ( Exception e )
        {

        }


        //setContentView(R.layout.activity_main);

        // Example of a call to a native method
        //TextView tv = findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());


        /*
        try
        {
            PackageInfo info = getPackageManager().getPackageInfo( sContext.getPackageName(), PackageManager.GET_SIGNATURES );
            for( Signature signature : info.signatures )
            {
                MessageDigest md = MessageDigest.getInstance("SHA");
                md.update(signature.toByteArray());

                String hash = android.util.Base64.encodeToString( md.digest(), android.util.Base64.DEFAULT );
                Log.v(LOG_TAG, "hash2=" + hash );
            }
        }
        catch( Exception e )
        {

        }
        */
    }
	
	@Override
    protected void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");

        super.onDestroy();
    }

    public static Locale getCurrentLocale() {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.M) {
            return sContext.getResources().getConfiguration().locale;
        } else {
            return sContext.getResources().getConfiguration().getLocales().get(0);
        }
    }

    public String getAssetsHash()
    {
        return mAssetsHash;
    }
	
	public String[] getAssetsNames()
    {

        return mAssetFilesList.toArray( new String[0] );
    }
	
	private boolean listAssetFiles( String path, ArrayList<String> results ) 
	{
        //Log.v(LOG_TAG,  "listAssetFiles: " + path);

        String[] list;
        try 
		{
            list = getAssets().list( path );
            if( list.length > 0 ) 
			{
                //Log.v(LOG_TAG,  path + " is a folder!");

                // This is a folder
                for( String file : list )
				{
                    //Log.v(LOG_TAG, "file=" + path + "/" + file);

                    if( path.length() > 0 && path != "/" )
                        listAssetFiles( path + "/" + file, results );
                    else
                        listAssetFiles( file, results );

                }
            }
            else
            {
                results.add(path);
            }
        } catch (IOException e) {
            return false;
        }

        return true;
    }

    public String getCountryCodeISO2()
    {
        Locale locale = getCurrentLocale();
        String country = locale.getCountry();

        return country;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    // public native String stringFromJNI();
}
