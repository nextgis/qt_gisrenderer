package com.nextgis.gismaplib.messages;

import com.nextgis.gismaplib.capnproto.GmConst;
import com.nextgis.gismaplib.capnproto.GmMsg;
import org.capnproto.AnyPointer;

import java.io.IOException;


public class GetTestTextMsg
    extends GmCoreMsg
{
  protected static final String TAG = GetTestTextMsg.class.getName();

  protected String mTestPath;
  protected String mTestText = null;


  public GetTestTextMsg(String testPath)
  {
    super();
    mMsgType = GmConst.MSG_TYPE_GET_TEST_TEXT;
    mTestPath = testPath;
  }


  @Override
  protected void setDataQ(AnyPointer.Builder dataPtrQ)
  {
    // set the query data
    GmMsg.GetTestTextQ.Builder dataQ = dataPtrQ.initAs(GmMsg.GetTestTextQ.factory);
    dataQ.setTestPath(mTestPath);
  }


  @Override
  protected AnyPointer.Reader msgWorker()
      throws IOException
  {
    // get the reply data
    AnyPointer.Reader dataPtrR = super.msgWorker();
    GmMsg.GetTestTextR.Reader dataR = dataPtrR.getAs(GmMsg.GetTestTextR.factory);
    mTestText = dataR.getTestText().toString();
    return (dataPtrR);
  }


  public String getTestText()
      throws IOException
  {
    if (null == mTestText) {
      msgWorker();
    }
    return (mTestText);
  }
}
