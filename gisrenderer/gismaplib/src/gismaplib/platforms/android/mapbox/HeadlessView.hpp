#ifndef MBGL_COMMON_HEADLESS_VIEW
#define MBGL_COMMON_HEADLESS_VIEW

#include <mbgl/mbgl.hpp>
#include <mbgl/gl/gl.hpp>

#include <EGL/egl.h>

#include <memory>
#include <thread>


namespace mbgl
{

  class HeadlessView
      : public View
  {
  public:
    HeadlessView(
        float pixelRatio,
        uint16_t width = 256,
        uint16_t height = 256);

    ~HeadlessView();

    float getPixelRatio() const override;
    std::array <uint16_t, 2> getSize() const override;
    std::array <uint16_t, 2> getFramebufferSize() const override;

    void invalidate() override;
    void activate() override;
    void deactivate() override;

    PremultipliedImage readStillImage() override;

    void resize(
        uint16_t width,
        uint16_t height);

  private:
    // Implementation specific functions

    void initializeDisplay();
    void terminateDisplay();

    void createContext();
    void destroyContext();

    void createSurface();
    void destroySurface();

    void activateContext();
    void deactivateContext();

    EGLConfig chooseConfig(
        const EGLConfig configs[],
        EGLint numConfigs);

    bool inEmulator();

    const float pixelRatio;
    std::array <uint16_t, 2> dimensions;

    bool needsResize = false;
    bool firstTime = false;
    bool active = false;

    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLSurface eglSurface = EGL_NO_SURFACE;
    EGLContext eglContext = EGL_NO_CONTEXT;

    EGLConfig eglConfig = nullptr;
    EGLint format = -1;
  };
} // namespace mbgl

#endif
