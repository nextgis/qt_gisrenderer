package com.nextgis.gismaplib.messages;

import com.nextgis.gismaplib.capnproto.GmConst;
import com.nextgis.gismaplib.capnproto.GmMsg;
import org.capnproto.AnyPointer;
import org.capnproto.ArrayInputStream;
import org.capnproto.ArrayOutputStream;
import org.capnproto.MessageBuilder;
import org.capnproto.MessageReader;
import org.capnproto.Serialize;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;


public abstract class GmCoreMsg
{
  protected static final String TAG = GmCoreMsg.class.getName();

  protected int mMsgType;


  private native static byte[] gmcoreSendMessage(ByteBuffer segmentsQuery);


  protected abstract void setDataQ(AnyPointer.Builder dataPtrQ);


  public GmCoreMsg()
  {
    mMsgType = GmConst.MSG_TYPE_UNKNOWN;
  }


  protected AnyPointer.Reader msgWorker()
      throws IOException
  {
    // create new query
    MessageBuilder cpnQuery = new MessageBuilder();
    GmMsg.Message.Builder msgQ = cpnQuery.initRoot(GmMsg.Message.factory);

    // set the query type
    msgQ.setMsgType(mMsgType);

    // set the query data
    AnyPointer.Builder dataPtrQ = msgQ.initDataPointer();
    setDataQ(dataPtrQ);

    // send query and receive reply
    MessageReader cpnReply = gmcoreSendMessage(cpnQuery);

    // get the reply
    GmMsg.Message.Reader msgR = cpnReply.getRoot(GmMsg.Message.factory);

    // check for errors
    if (msgR.getErrorFlag()) {
      throw new IOException(msgR.getMsgText().toString());
    }

    // check the reply type
    int replyType = msgR.getMsgType();
    if (mMsgType != replyType) {
      throw new IOException("Bad reply type: " + replyType + ", must be: " + mMsgType);
    }

    // get the reply data
    AnyPointer.Reader dataPtrR = msgR.getDataPointer();
    if (dataPtrR.isNull()) {
      throw new IOException("dataPtrR.isNull()");
    }

    return (dataPtrR);
  }


  protected MessageReader gmcoreSendMessage(MessageBuilder messageQuery)
      throws IOException
  {
    int sendBufferSize = (int) Serialize.computeSerializedSizeInWords(messageQuery) * 8;
    ByteBuffer buffer = ByteBuffer.allocateDirect(sendBufferSize);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    ArrayOutputStream aos = new ArrayOutputStream(buffer);
    Serialize.write(aos, messageQuery);

    ByteBuffer segmentsQuery = aos.getWriteBuffer();
    ByteBuffer segmentsReply = ByteBuffer.wrap(gmcoreSendMessage(segmentsQuery));

    ArrayInputStream ais = new ArrayInputStream(segmentsReply);
    return (Serialize.read(ais));
  }
}
