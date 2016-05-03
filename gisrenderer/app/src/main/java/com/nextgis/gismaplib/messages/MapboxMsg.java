package com.nextgis.gismaplib.messages;

import android.graphics.Bitmap;
import com.nextgis.gismaplib.capnproto.GmConst;
import com.nextgis.gismaplib.capnproto.GmMsg;
import org.capnproto.AnyPointer;

import java.io.IOException;


public class MapboxMsg
    extends GmCoreMsg
{
  protected static final String TAG = MapboxMsg.class.getName();

  protected Bitmap mDstBitmap;
  protected Boolean mIsImageStored        = null;
  protected boolean mIsBitmapPixelsLocked = false;


  public MapboxMsg(Bitmap dstBitmap)
  {
    super();
    mMsgType = GmConst.MSG_TYPE_MAPBOX_IMAGE_DATA;
    mDstBitmap = dstBitmap;
  }


  @Override
  protected void setDataQ(AnyPointer.Builder dataPtrQ)
  {
    // set the query data
    GmMsg.MapboxQ.Builder dataQ = dataPtrQ.initAs(GmMsg.MapboxQ.factory);

    long imagePointer = GmCoreMsg.lockBitmapPixels(mDstBitmap);
    dataQ.setImagePointer(imagePointer);

    if (imagePointer != 0) {
      mIsBitmapPixelsLocked = true;
      dataQ.setImageWidth(mDstBitmap.getWidth());
      dataQ.setImageHeight(mDstBitmap.getHeight());
    }
  }


  @Override
  protected AnyPointer.Reader msgWorker()
      throws IOException
  {
    // get the reply data
    AnyPointer.Reader dataPtrR = super.msgWorker();
    GmMsg.MapboxR.Reader dataR = dataPtrR.getAs(GmMsg.MapboxR.factory);

    mIsImageStored = dataR.getIsImageStored();
    if (mIsImageStored) {
      GmCoreMsg.unlockBitmapPixels(mDstBitmap);
      mIsBitmapPixelsLocked = false;
    }

    return (dataPtrR);
  }


  public Boolean isImageStored()
      throws IOException
  {
    if (null == mIsImageStored) {
      msgWorker();
    }
    return (mIsImageStored);
  }
}
