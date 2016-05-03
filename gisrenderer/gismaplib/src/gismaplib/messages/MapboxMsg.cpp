#include "gismaplib/messages/MapboxMsg.h"

#include "gismaplib/GmLog.h"

#include <GLES2/gl2.h>
#include <EGL/egl.h>


namespace gmcore
{
  void MapboxMsg::dataWorker(
      boost::shared_ptr <capnp::AnyPointer::Reader> dataPtrQ,
      boost::shared_ptr <GmMsg::Message::Builder> msgPtrR)
  {
    // get the query data
    GmMsg::MapboxQ::Reader dataQ = dataPtrQ->getAs <GmMsg::MapboxQ>();

    uintptr_t imagePointer = static_cast<uintptr_t> (dataQ.getImagePointer());

    capnp::AnyPointer::Builder dataPtrR = msgPtrR->initDataPointer();
    GmMsg::MapboxR::Builder dataR = dataPtrR.initAs <GmMsg::MapboxR>();
    if (!imagePointer) {
      dataR.setIsImageStored(false);
      return;
    }

    void* imagePtr = reinterpret_cast<void*> (imagePointer);
    uint32_t imageWidth = dataQ.getImageWidth();
    uint32_t imageHeight = dataQ.getImageHeight();

//    BOOST_LOG_SEV(GmLog::log, debug) << "testPath: " << testPath;

    // make the reply data
    // get and put image data

    // http://stackoverflow.com/q/28817777
    // http://stackoverflow.com/a/27092070
    // TODO: work with OpenGL errors
    // Step 1 - Get the default display.
    EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType) 0);

    // Step 2 - Initialize EGL.
    eglInitialize(eglDisplay, 0, 0);

    // Step 3 - Make OpenGL ES the current API.
    eglBindAPI(EGL_OPENGL_API);

    // Step 4 - Specify the required configuration attributes.
    EGLint pi32ConfigAttribs[9];
    pi32ConfigAttribs[0] = EGL_COLOR_BUFFER_TYPE;
    pi32ConfigAttribs[1] = EGL_RGB_BUFFER;
    pi32ConfigAttribs[2] = EGL_LEVEL;
    pi32ConfigAttribs[3] = 0;
    pi32ConfigAttribs[4] = EGL_RENDERABLE_TYPE;
    pi32ConfigAttribs[5] = EGL_OPENGL_ES2_BIT;
    pi32ConfigAttribs[6] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[7] = EGL_PBUFFER_BIT;
    pi32ConfigAttribs[8] = EGL_NONE;

    // Step 5 - Find a config that matches all requirements.
    int iConfigs;
    EGLConfig eglConfig;
    eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs);
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error" << eglGetError();

    if (iConfigs != 1) {
      throw GmCoreErrEx() << GmCoreErrInfo("Error: eglChooseConfig(): config not found.");
    }

    EGLint pbufferAttribs[5];
    pbufferAttribs[0] = EGL_WIDTH;
    pbufferAttribs[1] = imageWidth;
    pbufferAttribs[2] = EGL_HEIGHT;
    pbufferAttribs[3] = imageHeight;
    pbufferAttribs[4] = EGL_NONE;

    // Step 6 - Create a surface to draw to.
    EGLSurface eglSurface;
    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttribs);
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error" << eglGetError();

    if (eglSurface == EGL_NO_SURFACE) {
      BOOST_LOG_SEV(GmLog::log, debug) << "surface issue";
    }

    // Step 7 - Create a context.
    EGLContext eglContext;
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error" << eglGetError();

    if (eglContext == EGL_NO_CONTEXT) {
      BOOST_LOG_SEV(GmLog::log, debug) << "context issue";
    }

    // Step 8 - Bind the context to the current thread
    bool result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    if (!result) {
      BOOST_LOG_SEV(GmLog::log, debug) << "make current error" << eglGetError();
    }
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error" << eglGetError();

    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(eglDisplay, eglSurface);
    glReadPixels(0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, imagePtr);

    // set the reply data
    dataR.setIsImageStored(true);
  }
}  // namespace gmcore
