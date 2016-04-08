package com.nextgis.gismaplib.util;

import android.util.Log;

import static android.util.Log.DEBUG;
import static android.util.Log.INFO;


public class GmLog
{
  private static final String mLogPrefix = "-Gm- app: ";


  public static int d(
      String tag,
      String msg)
  {
    return (Log.println(DEBUG, tag, mLogPrefix + msg));
  }


  public static int i(
      String tag,
      String msg)
  {
    return (Log.println(INFO, tag, mLogPrefix + msg));
  }
}
