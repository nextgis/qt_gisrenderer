package com.nextgis.gismaplib;

import android.graphics.Bitmap;
import com.nextgis.gismaplib.messages.GetTestTextMsg;
import com.nextgis.gismaplib.messages.MapboxMsg;

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


  public Boolean putMapboxImageData(Bitmap dstBitmap)
      throws IOException
  {
    return (new MapboxMsg(dstBitmap).isImageStored());
  }


  // for test
  public String getTestText(String testPath)
      throws IOException
  {
    return (new GetTestTextMsg(testPath).getTestText());
  }
}
