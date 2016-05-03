package com.nextgis.gismaplib.messages;

import com.nextgis.gismaplib.capnproto.GmConst;
import com.nextgis.gismaplib.capnproto.GmMsg;
import org.capnproto.AnyPointer;

import java.io.IOException;


public class MapboxMsg
    extends GmCoreMsg
{
  protected static final String TAG = MapboxMsg.class.getName();

  protected String mTestPath;
  protected byte[] mImageData = null;


  public MapboxMsg(String testPath)
  {
    super();
    mMsgType = GmConst.MSG_TYPE_MAPBOX_IMAGE_DATA;
    mTestPath = testPath;
  }


  @Override
  protected void setDataQ(AnyPointer.Builder dataPtrQ)
  {
    // set the query data
    GmMsg.MapboxQ.Builder dataQ = dataPtrQ.initAs(GmMsg.MapboxQ.factory);
    dataQ.setTestPath(mTestPath);
  }


  @Override
  protected AnyPointer.Reader msgWorker()
      throws IOException
  {
    // get the reply data
    AnyPointer.Reader dataPtrR = super.msgWorker();
    GmMsg.MapboxR.Reader dataR = dataPtrR.getAs(GmMsg.MapboxR.factory);
    mImageData = dataR.getImageData().toArray();
    return (dataPtrR);
  }


  public byte[] getImageData()
      throws IOException
  {
    if (null == mImageData) {
      msgWorker();
    }
    return (mImageData);
  }
}
