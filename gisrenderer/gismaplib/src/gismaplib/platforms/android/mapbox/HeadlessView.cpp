#include <gismaplib/platforms/android/mapbox/HeadlessView.hpp>

#include <cassert>
#include <cstring>
#include <list>

#include <sys/system_properties.h>

#include <mbgl/platform/event.hpp>
#include <mbgl/platform/log.hpp>

#include <GLES2/gl2.h>


namespace mbgl
{

  void log_egl_string(
      EGLDisplay display,
      EGLint name,
      const char* label)
  {
    const char* str = eglQueryString(display, name);
    if (str == nullptr) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "eglQueryString(%d) returned error %d", name, eglGetError());
      throw std::runtime_error("eglQueryString() failed");
    } else {
      char buf[513];
      for (int len = std::strlen(str), pos = 0; len > 0; len -= 512, pos += 512) {
        strncpy(buf, str + pos, 512);
        buf[512] = 0;
        mbgl::Log::Info(mbgl::Event::OpenGL, "EGL %s: %s", label, buf);
      }
    }
  }


  void log_gl_string(
      GLenum name,
      const char* label)
  {
    const GLubyte* str = glGetString(name);
    if (str == nullptr) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "glGetString(%d) returned error %d", name, glGetError());
      throw std::runtime_error("glGetString() failed");
    } else {
      char buf[513];
      for (int len = std::strlen(reinterpret_cast<const char*>(str)), pos = 0; len > 0;
           len -= 512, pos += 512) {
        strncpy(buf, reinterpret_cast<const char*>(str) + pos, 512);
        buf[512] = 0;
        mbgl::Log::Info(mbgl::Event::OpenGL, "GL %s: %s", label, buf);
      }
    }
  }


  HeadlessView::HeadlessView(
      float pixelRatio_,
      uint16_t width,
      uint16_t height)
      : pixelRatio(pixelRatio_)
      , dimensions({{width, height}})
      , needsResize(true)
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::HeadlessView");

    initializeDisplay();
    createContext();
  }


  HeadlessView::~HeadlessView()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::~HeadlessView");

    destroyContext();
    destroySurface();
    terminateDisplay();
  }


  void HeadlessView::resize(
      const uint16_t width,
      const uint16_t height)
  {
    if (dimensions[0] == width && dimensions[1] == height) {
      return;
    }
    dimensions = {{width, height}};
    needsResize = true;
  }


  PremultipliedImage HeadlessView::readStillImage()
  {
    assert(active);

    const unsigned int w = dimensions[0] * pixelRatio;
    const unsigned int h = dimensions[1] * pixelRatio;

    PremultipliedImage image{w, h};

    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image.data.get());
    GLenum error = glGetError();
    if (GL_NO_ERROR != error) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "glViewport() returned error %d", error);
      while (glGetError() != GL_NO_ERROR) { }
      throw std::runtime_error("glViewport() failed");
    }

    const auto stride = image.stride();
    auto tmp = std::make_unique <uint8_t[]>(stride);
    uint8_t* rgba = image.data.get();
    for (int i = 0, j = h - 1; i < j; i++, j--) {
      std::memcpy(tmp.get(), rgba + i * stride, stride);
      std::memcpy(rgba + i * stride, rgba + j * stride, stride);
      std::memcpy(rgba + j * stride, tmp.get(), stride);
    }

    return image;
  }


  float HeadlessView::getPixelRatio() const
  {
    return pixelRatio;
  }


  std::array <uint16_t, 2> HeadlessView::getSize() const
  {
    return dimensions;
  }


  std::array <uint16_t, 2> HeadlessView::getFramebufferSize() const
  {
    return {
        {
            static_cast<uint16_t>(dimensions[0] * pixelRatio), static_cast<uint16_t>(dimensions[1]
            * pixelRatio)}};
  }


  void HeadlessView::activate()
  {
    assert(eglContext != EGL_NO_CONTEXT);

    active = true;
    activateContext();

    if (needsResize) {
      destroySurface();
      createSurface();
      needsResize = false;
    }
  }


  void HeadlessView::deactivate()
  {
    deactivateContext();
    active = false;
  }


  void HeadlessView::invalidate()
  {
    assert(false);
  }


  bool HeadlessView::inEmulator()
  {
    // Detect if we are in emulator
    char prop[PROP_VALUE_MAX];
    __system_property_get("ro.kernel.qemu", prop);
    return strtol(prop, nullptr, 0) == 1;
  }


  void HeadlessView::initializeDisplay()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "NativeMapView::initializeDisplay");

    assert(eglDisplay == EGL_NO_DISPLAY);
    assert(eglConfig == nullptr);
    assert(format < 0);

    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "eglGetDisplay() returned error %d", eglGetError());
      throw std::runtime_error("eglGetDisplay() failed");
    }

    EGLint major, minor;
    if (!eglInitialize(eglDisplay, &major, &minor)) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "eglInitialize() returned error %d", eglGetError());
      throw std::runtime_error("eglInitialize() failed");
    }
    if ((major <= 1) && (minor < 3)) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "EGL version is too low, need 1.3, got %d.%d", major, minor);
      throw std::runtime_error("EGL version is too low");
    }

    log_egl_string(eglDisplay, EGL_VENDOR, "Vendor");
    log_egl_string(eglDisplay, EGL_VERSION, "Version");
    log_egl_string(eglDisplay, EGL_CLIENT_APIS, "Client APIs");
    log_egl_string(eglDisplay, EGL_EXTENSIONS, "Client Extensions");

    // Detect if we are in emulator
    if (inEmulator()) {
      mbgl::Log::Warning(mbgl::Event::Android, "In emulator! Enabling hacks :-(");
    }

    // Get all configs at least RGB 565 with 16 depth and 8 stencil
    EGLint configAttribs[] = {
        EGL_CONFIG_CAVEAT,
        EGL_NONE,
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_BUFFER_SIZE,
        16,
        EGL_RED_SIZE,
        5,
        EGL_GREEN_SIZE,
        6,
        EGL_BLUE_SIZE,
        5,
        EGL_DEPTH_SIZE,
        16,
        EGL_STENCIL_SIZE,
        8,
        (inEmulator() ? EGL_NONE : EGL_CONFORMANT),
        EGL_OPENGL_ES2_BIT, // Ugly hack
        (inEmulator() ? EGL_NONE : EGL_COLOR_BUFFER_TYPE),
        EGL_RGB_BUFFER,     // Ugly hack
        EGL_NONE};
    EGLint numConfigs;
    if (!eglChooseConfig(eglDisplay, configAttribs, nullptr, 0, &numConfigs)) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "eglChooseConfig(NULL) returned error %d", eglGetError());
      throw std::runtime_error("eglChooseConfig() failed");
    }
    if (numConfigs < 1) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "eglChooseConfig() returned no configs.");
      throw std::runtime_error("eglChooseConfig() failed");
    }

    const auto configs = std::make_unique <EGLConfig[]>(numConfigs);
    if (!eglChooseConfig(eglDisplay, configAttribs, configs.get(), numConfigs, &numConfigs)) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "eglChooseConfig() returned error %d", eglGetError());
      throw std::runtime_error("eglChooseConfig() failed");
    }

    eglConfig = chooseConfig(configs.get(), numConfigs);
    if (eglConfig == nullptr) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "No config chosen");
      throw std::runtime_error("No config chosen");
    }

    if (!eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format)) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "eglGetConfigAttrib() returned error %d", eglGetError());
      throw std::runtime_error("eglGetConfigAttrib() failed");
    }
    mbgl::Log::Info(mbgl::Event::OpenGL, "Chosen window format is %d", format);
  }


  void HeadlessView::terminateDisplay()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "NativeMapView::terminateDisplay");

    if (eglDisplay != EGL_NO_DISPLAY) {
      // Destroy the surface first, if it still exists. This call needs a valid surface.
      if (eglSurface != EGL_NO_SURFACE) {
        if (!eglDestroySurface(eglDisplay, eglSurface)) {
          mbgl::Log::Error(
              mbgl::Event::OpenGL, "eglDestroySurface() returned error %d", eglGetError());
          throw std::runtime_error("eglDestroySurface() failed");
        }
        eglSurface = EGL_NO_SURFACE;
      }

      if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglMakeCurrent(EGL_NO_CONTEXT) returned error %d", eglGetError());
        throw std::runtime_error("eglMakeCurrent() failed");
      }

      if (!eglTerminate(eglDisplay)) {
        mbgl::Log::Error(mbgl::Event::OpenGL, "eglTerminate() returned error %d", eglGetError());
        throw std::runtime_error("eglTerminate() failed");
      }
    }

    eglDisplay = EGL_NO_DISPLAY;
    eglConfig = nullptr;
    format = -1;
  }


  void HeadlessView::createContext()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::initializeContext");

    assert(eglDisplay != EGL_NO_DISPLAY);
    assert(eglContext == EGL_NO_CONTEXT);
    assert(eglConfig != nullptr);

    const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttribs);
    if (eglContext == EGL_NO_CONTEXT) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "eglCreateContext() returned error %d", eglGetError());
      throw std::runtime_error("eglCreateContext() failed");
    }
  }


  void HeadlessView::destroyContext()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::terminateContext");

    if (eglDisplay != EGL_NO_DISPLAY) {

      if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglMakeCurrent(EGL_NO_CONTEXT) returned error %d", eglGetError());
        throw std::runtime_error("eglMakeCurrent() failed");
      }

      if (eglContext != EGL_NO_CONTEXT) {
        if (!eglDestroyContext(eglDisplay, eglContext)) {
          mbgl::Log::Error(
              mbgl::Event::OpenGL, "eglDestroyContext() returned error %d", eglGetError());
          throw std::runtime_error("eglDestroyContext() failed");
        }
      }
    }

    eglContext = EGL_NO_CONTEXT;
  }


  void HeadlessView::createSurface()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::createSurface");

    assert(eglDisplay != EGL_NO_DISPLAY);
    assert(eglSurface == EGL_NO_SURFACE);
    assert(eglConfig != nullptr);
    assert(format >= 0);

    const unsigned int w = dimensions[0] * pixelRatio;
    const unsigned int h = dimensions[1] * pixelRatio;

    EGLint pbufferAttribs[5];
    pbufferAttribs[0] = EGL_WIDTH;
    pbufferAttribs[1] = w;
    pbufferAttribs[2] = EGL_HEIGHT;
    pbufferAttribs[3] = h;
    pbufferAttribs[4] = EGL_NONE;

    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttribs);
    if (eglSurface == EGL_NO_SURFACE) {
      mbgl::Log::Error(
          mbgl::Event::OpenGL, "eglCreateWindowSurface() returned error %d", eglGetError());
      throw std::runtime_error("eglCreateWindowSurface() failed");
    }

    glViewport(0, 0, w, h);
    GLenum error = glGetError();
    if (GL_NO_ERROR != error) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "glViewport() returned error %d", error);
      while (glGetError() != GL_NO_ERROR) { }
      throw std::runtime_error("glViewport() failed");
    }

    if (!firstTime) {
      firstTime = true;

      if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
        mbgl::Log::Error(mbgl::Event::OpenGL, "eglMakeCurrent() returned error %d", eglGetError());
        throw std::runtime_error("eglMakeCurrent() failed");
      }

      log_gl_string(GL_VENDOR, "Vendor");
      log_gl_string(GL_RENDERER, "Renderer");
      log_gl_string(GL_VERSION, "Version");
      if (!inEmulator()) {
        log_gl_string(
            GL_SHADING_LANGUAGE_VERSION,
            "SL Version"); // In the emulator this returns NULL with error code 0?
        // https://code.google.com/p/android/issues/detail?id=78977
      }

      log_gl_string(GL_EXTENSIONS, "Extensions");
      mbgl::gl::InitializeExtensions(
          [](const char* name)
          {
            return reinterpret_cast<mbgl::gl::glProc>(eglGetProcAddress(name));
          });
    }
  }


  void HeadlessView::destroySurface()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::destroySurface");

    if (eglSurface != EGL_NO_SURFACE) {
      if (!eglDestroySurface(eglDisplay, eglSurface)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglDestroySurface() returned error %d", eglGetError());
        throw std::runtime_error("eglDestroySurface() failed");
      }
    }

    eglSurface = EGL_NO_SURFACE;
  }


  void HeadlessView::activateContext()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "HeadlessView::activateContext");

    if ((eglDisplay != EGL_NO_DISPLAY) && (eglSurface != EGL_NO_SURFACE)
        && (eglContext != EGL_NO_CONTEXT)) {
      if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglMakeCurrent() returned error %d", eglGetError());
        throw std::runtime_error("eglMakeCurrent() failed");
      }
    } else {
      mbgl::Log::Info(mbgl::Event::Android, "Not activating as we are not ready");
    }
  }


  void HeadlessView::deactivateContext()
  {
    mbgl::Log::Debug(mbgl::Event::Android, "NativeMapView::deactivate");

    if (eglDisplay != EGL_NO_DISPLAY) {
      if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglMakeCurrent(EGL_NO_CONTEXT) returned error %d", eglGetError());
        throw std::runtime_error("eglMakeCurrent() failed");
      }
    } else {
      mbgl::Log::Info(mbgl::Event::Android, "Not deactivating as we are not ready");
    }
  }


// Speed
/*
typedef enum {
    Format16Bit = 0,
    Format32BitNoAlpha = 1,
    Format32BitAlpha = 2,
    Format24Bit = 3,
    Unknown = 4
} BufferFormat;

typedef enum {
    Format16Depth8Stencil = 0,
    Format24Depth8Stencil = 1,
} DepthStencilFormat;
*/

// Quality
  typedef enum
  {
    Format16Bit = 3,
    Format32BitNoAlpha = 1,
    Format32BitAlpha = 2,
    Format24Bit = 0,
    Unknown = 4
  } BufferFormat;

  typedef enum
  {
    Format16Depth8Stencil = 1,
    Format24Depth8Stencil = 0,
  } DepthStencilFormat;

// Tuple is <buffer_format, depth_stencil_format, is_not_conformant, is_caveat, config_num,
// config_id>
  typedef std::tuple <BufferFormat, DepthStencilFormat, bool, bool, int, EGLConfig>
      ConfigProperties;


  EGLConfig HeadlessView::chooseConfig(
      const EGLConfig configs[],
      EGLint numConfigs)
  {
    mbgl::Log::Info(mbgl::Event::OpenGL, "Found %d configs", numConfigs);

    // Create a list of configs that pass our filters
    std::list <ConfigProperties> configList;
    for (int i = 0; i < numConfigs; i++) {
      mbgl::Log::Info(mbgl::Event::OpenGL, "Config %d:", i);

      EGLint caveat, conformant, bits, red, green, blue, alpha, alphaMask, depth, stencil,
          sampleBuffers, samples;

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_CONFIG_CAVEAT, &caveat)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_CONFIG_CAVEAT) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_CONFORMANT, &conformant)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_CONFORMANT) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_BUFFER_SIZE, &bits)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_BUFFER_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_RED_SIZE, &red)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_RED_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_GREEN_SIZE, &green)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_GREEN_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_BLUE_SIZE, &blue)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_BLUE_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_ALPHA_SIZE, &alpha)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_ALPHA_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_ALPHA_MASK_SIZE, &alphaMask)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_ALPHA_MASK_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_DEPTH_SIZE, &depth)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_DEPTH_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_STENCIL_SIZE, &stencil)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_STENCIL_SIZE) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_SAMPLE_BUFFERS, &sampleBuffers)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_SAMPLE_BUFFERS) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      if (!eglGetConfigAttrib(eglDisplay, configs[i], EGL_SAMPLES, &samples)) {
        mbgl::Log::Error(
            mbgl::Event::OpenGL, "eglGetConfigAttrib(EGL_SAMPLES) returned error %d",
            eglGetError());
        throw std::runtime_error("eglGetConfigAttrib() failed");
      }

      mbgl::Log::Info(mbgl::Event::OpenGL, "...Caveat: %d", caveat);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Conformant: %d", conformant);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Color: %d", bits);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Red: %d", red);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Green: %d", green);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Blue: %d", blue);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Alpha: %d", alpha);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Alpha mask: %d", alphaMask);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Depth: %d", depth);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Stencil: %d", stencil);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Sample buffers: %d", sampleBuffers);
      mbgl::Log::Info(mbgl::Event::OpenGL, "...Samples: %d", samples);

      bool configOk = true;
      configOk &= (depth == 24) || (depth == 16);
      configOk &= stencil == 8;
      configOk &= sampleBuffers == 0;
      configOk &= samples == 0;

      // Filter our configs first for depth, stencil and anti-aliasing
      if (configOk) {
        // Work out the config's buffer format
        BufferFormat bufferFormat;
        if ((bits == 16) && (red == 5) && (green == 6) && (blue == 5) && (alpha == 0)) {
          bufferFormat = Format16Bit;
        } else if ((bits == 32) && (red == 8) && (green == 8) && (blue == 8) && (alpha == 0)) {
          bufferFormat = Format32BitNoAlpha;
        } else if ((bits == 32) && (red == 8) && (green == 8) && (blue == 8) && (alpha == 8)) {
          bufferFormat = Format32BitAlpha;
        } else if ((bits == 24) && (red == 8) && (green == 8) && (blue == 8) && (alpha == 0)) {
          bufferFormat = Format24Bit;
        } else {
          bufferFormat = Unknown;
        }

        // Work out the config's depth stencil format
        DepthStencilFormat depthStencilFormat;
        if ((depth == 16) && (stencil == 8)) {
          depthStencilFormat = Format16Depth8Stencil;
        } else {
          depthStencilFormat = Format24Depth8Stencil;
        }

        bool isNotConformant = (conformant & EGL_OPENGL_ES2_BIT) != EGL_OPENGL_ES2_BIT;
        bool isCaveat = caveat != EGL_NONE;
        EGLConfig configId = configs[i];

        // Ignore formats we don't recognise
        if (bufferFormat != Unknown) {
          configList.push_back(
              std::make_tuple(
                  bufferFormat, depthStencilFormat, isNotConformant, isCaveat, i, configId));
        }
      }
    }

    if (configList.empty()) {
      mbgl::Log::Error(mbgl::Event::OpenGL, "Config list was empty.");
    }

    // Sort the configs to find the best one
    configList.sort();
    bool isConformant = !std::get <2>(configList.front());
    bool isCaveat = std::get <3>(configList.front());
    int configNum = std::get <4>(configList.front());
    EGLConfig configId = std::get <5>(configList.front());

    mbgl::Log::Info(mbgl::Event::OpenGL, "Chosen config is %d", configNum);

    if (isCaveat) {
      mbgl::Log::Warning(mbgl::Event::OpenGL, "Chosen config has a caveat.");
    }
    if (!isConformant) {
      mbgl::Log::Warning(mbgl::Event::OpenGL, "Chosen config is not conformant.");
    }

    return configId;
  }
} // namespace mbgl
