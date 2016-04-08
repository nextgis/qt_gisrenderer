package com.nextgis.gisrenderer;

import android.app.Application;
import com.nextgis.gismaplib.GmCore;


public class MainApplication
    extends Application
{
  protected static final String TAG = MainApplication.class.getName();

  protected GmCore mGmCore;


  static {
    //System.loadLibrary("crystax"); // need for shared linking on some devices
    //System.loadLibrary("gnustl_shared"); // need for shared linking on some devices
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
