#include <iostream>
#include <unistd.h>

#include <android/log.h>

#include <jni.h>


#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "GmCore", fmt, ## __VA_ARGS__)

#include "gismaplib/GmCore.h"


// Redirect the "stdout" and "stderr" to android logcat.
// https://codelab.wordpress.com/2014/11/03/how-to-use-standard-output-streams-for-logging-in-android-apps/
// http://stackoverflow.com/a/31777050

static int logcat_pfd[2];

static pthread_t stdouts_thread;

static const char* logcat_tag;


static void* stdouts_thread_func(void*)
{
  ssize_t rdsz;
  char buf[256];

  // workaround for android logcat formatting
  buf[0] = '-';
  buf[1] = 'F';
  buf[2] = 'g';
  buf[3] = '-';
  buf[4] = ' ';

  int logPrefixSize = 5;

  while ((rdsz = read(logcat_pfd[0], buf + logPrefixSize, sizeof buf - 1 - logPrefixSize)) > 0) {
//        if(buf[rdsz + 7 - 1 ] == '\n') --rdsz;
    buf[rdsz + logPrefixSize] = 0;      /* add null-terminator */
    __android_log_write(ANDROID_LOG_DEBUG, logcat_tag, buf);
  }
  return (0);
}  // stdouts_thread_func


int redirect_stdouts_to_logcat(const char* app_name)
{
  logcat_tag = app_name;

  /* make stdout line-buffered and stderr unbuffered */
  setvbuf(stdout, 0, _IOLBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  /* create the pipe and redirect stdout and stderr */
  pipe(logcat_pfd);
  dup2(logcat_pfd[1], 1);
  dup2(logcat_pfd[1], 2);

  /* spawn the logging thread */
  if (pthread_create(&stdouts_thread, 0, stdouts_thread_func, 0) == -1) {
    return (-1);
  }

  pthread_detach(stdouts_thread);
  return (0);
}  // redirect_stdouts_to_logcat


extern "C" void Java_com_nextgis_gismaplib_GmCore_gmcoreInit(
    JNIEnv* env,
    jobject thiz)
{
  redirect_stdouts_to_logcat("GmCore");

  LOG("-Gm- GM: GmCore init %s", "starting");
  gmcore::GmCore::gmcoreInit();
  LOG("-Gm- GM: GmCore init %s", "finished");
}


extern "C" jbyteArray Java_com_nextgis_gismaplib_messages_GmCoreMsg_gmcoreSendMessage(
    JNIEnv* env,
    jclass type,
    jobject segmentsQuery)
{
  void* segmentsPtrQ = env->GetDirectBufferAddress(segmentsQuery);
  long long int segmentSizeQ = env->GetDirectBufferCapacity(segmentsQuery);

  kj::Array <capnp::word> replyWords = gmcore::GmCore::messageWorker(segmentsPtrQ, segmentSizeQ);
  kj::ArrayPtr <kj::byte> replyBytes = replyWords.asBytes();
  jbyte* segmentsPtrsR = static_cast <jbyte*>(static_cast <void*>(replyBytes.begin()));
  jsize segmentsSizesR = static_cast <jsize>(replyBytes.size());

  if (!segmentsPtrsR || !segmentsSizesR) {
    return (nullptr);
  }

  jbyteArray segmentsReply = env->NewByteArray(segmentsSizesR);
  env->SetByteArrayRegion(segmentsReply, 0, segmentsSizesR, segmentsPtrsR);
  return (segmentsReply);
}  // Java_com_nextgis_gismaplib_messages_GmCoreMsg_gmcoreSendMessage
