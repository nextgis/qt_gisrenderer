#ifndef GMCORE_GLOFFLINERENDERER_H
#define GMCORE_GLOFFLINERENDERER_H

#include <cinttypes>

#include <GLES2/gl2.h>
#include <EGL/egl.h>


namespace gmcore
{
  class GlOfflineRenderer
  {
  public:

    explicit GlOfflineRenderer(
            void* pImageData,
            uint32_t imageWidth,
            uint32_t imageHeight);

    void render();

  private:
    void init();

    void* mpImageData;
    uint32_t mImageWidth;
    uint32_t mImageHeight;

    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLContext mEglContext;
  };
}

#endif  // GMCORE_GLOFFLINERENDERER_H
