#include "gismaplib/platforms/android/GlOfflineRenderer.h"

#include "gismaplib/GmLog.h"


namespace gmcore
{
  GlOfflineRenderer::GlOfflineRenderer(
      void* pImageData,
      uint32_t imageWidth,
      uint32_t imageHeight)
      : mpImageData{pImageData}
      , mImageWidth{imageWidth}
      , mImageHeight{imageHeight}
  {
    init();
  }


  void GlOfflineRenderer::init()
  {
// About OpenGL and offscreen rendering
// http://stackoverflow.com/q/28817777
// http://stackoverflow.com/a/27092070
// http://android-dls.com/wiki/index.php?title=EGL_notes
// https://www.khronos.org/registry/egl/
// https://www.khronos.org/developers/reference-cards
// https://www.opengl.org/sdk/docs/man2/

    // TODO: work with OpenGL errors
    // Step 1 - Get the default display.
    mEglDisplay = eglGetDisplay((EGLNativeDisplayType) 0);

    // Step 2 - Initialize EGL.
    eglInitialize(mEglDisplay, 0, 0);

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
    eglChooseConfig(mEglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs);
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error " << eglGetError();

    if (iConfigs != 1) {
//      throw GmCoreErrEx() << GmCoreErrInfo("Error: eglChooseConfig(): config not found.");
    }



    // Step 7 - Create a context.
    mEglContext = eglCreateContext(mEglDisplay, eglConfig, NULL, NULL);
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error " << eglGetError();

    if (mEglContext == EGL_NO_CONTEXT) {
      BOOST_LOG_SEV(GmLog::log, debug) << "context issue";
    }



    EGLint pbufferAttribs[5];
    pbufferAttribs[0] = EGL_WIDTH;
    pbufferAttribs[1] = mImageWidth;
    pbufferAttribs[2] = EGL_HEIGHT;
    pbufferAttribs[3] = mImageHeight;
    pbufferAttribs[4] = EGL_NONE;

    // Step 6 - Create a surface to draw to.
    mEglSurface = eglCreatePbufferSurface(mEglDisplay, eglConfig, pbufferAttribs);
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error " << eglGetError();

    if (mEglSurface == EGL_NO_SURFACE) {
      BOOST_LOG_SEV(GmLog::log, debug) << "surface issue";
    }
  }


  void GlOfflineRenderer::render()
  {
    // Step 8 - Bind the context to the current thread
    EGLBoolean result = eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext);

    if (!result) {
      BOOST_LOG_SEV(GmLog::log, debug) << "make current error" << eglGetError();
    }
    BOOST_LOG_SEV(GmLog::log, debug) << "egl error" << eglGetError();

    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(mEglDisplay, mEglSurface);
    glReadPixels(0, 0, mImageWidth, mImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, mpImageData);
  }
}
