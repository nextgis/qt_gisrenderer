package com.nextgis.gismaplib;

import com.nextgis.gismaplib.messages.GetTestTextMsg;

import java.io.IOException;


public final class GmCore
{
  protected static final String TAG = GmCore.class.getName();


  // native static and non-static:
  // http://stackoverflow.com/a/15254300
  private native void gmcoreInit();


  public GmCore()
  {
    gmcoreInit();
  }


  // for test
  public String getTestText(String testPath)
      throws IOException
  {
    return (new GetTestTextMsg(testPath).getTestText());
  }
}
