package com.nextgis.gisrenderer;

import android.app.Application;
import com.nextgis.gismaplib.GmCore;


public class MainApplication
    extends Application
{
  protected static final String TAG = MainApplication.class.getName();

  protected GmCore mGmCore;


  static {
//    System.loadLibrary("crystax");
//    System.loadLibrary("gnustl_shared");

//    System.loadLibrary("boost_system");
//    System.loadLibrary("boost_regex");
//    System.loadLibrary("boost_filesystem");
//    System.loadLibrary("boost_date_time");
//    System.loadLibrary("boost_thread");
//    System.loadLibrary("boost_chrono");
//    System.loadLibrary("boost_log"); // can be compiled only static

//    System.loadLibrary("mapbox-gl");
    System.loadLibrary("gismaplib");
  }


  @Override
  public void onCreate()
  {
    // For service debug
//    android.os.Debug.waitForDebugger();

    super.onCreate();

    mGmCore = new GmCore();
  }


  public GmCore getGmCore()
  {
    return (mGmCore);
  }
}
