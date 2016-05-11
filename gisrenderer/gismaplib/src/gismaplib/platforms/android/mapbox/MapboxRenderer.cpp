#include "gismaplib/platforms/android/mapbox/MapboxRenderer.h"

#include <mbgl/map/map.hpp>
//#include <mbgl/util/image.hpp>
#include <mbgl/util/run_loop.hpp>

#include <gismaplib/platforms/android/mapbox/HeadlessView.hpp>
#include <mbgl/platform/log.hpp>
#include <mbgl/storage/default_file_source.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>


// About OpenGL and offscreen rendering
// http://stackoverflow.com/q/28817777
// http://stackoverflow.com/a/27092070
// http://android-dls.com/wiki/index.php?title=EGL_notes
// https://www.khronos.org/registry/egl/
// https://www.khronos.org/developers/reference-cards
// https://www.opengl.org/sdk/docs/man2/

namespace gmcore
{
  MapboxRenderer::MapboxRenderer(
      void* pImageData,
      uint16_t imageWidth,
      uint16_t imageHeight)
      : mpImageData(pImageData)
      , mImageWidth(imageWidth)
      , mImageHeight(imageHeight)
  { }


  void write_file(
      const std::string& filename,
      const std::string& data)
  {
    FILE* fd = fopen(filename.c_str(), "wb");
    if (fd) {
      fwrite(data.data(), sizeof(std::string::value_type), data.size(), fd);
      fclose(fd);
    } else {
      throw std::runtime_error(std::string("Failed to open file ") + filename);
    }
  }


  std::string read_file(const std::string& filename)
  {
    std::ifstream file(filename);
    if (file.good()) {
      std::stringstream data;
      data << file.rdbuf();
      return data.str();
    } else {
      throw std::runtime_error(std::string("Cannot read file ") + filename);
    }
  }


  void MapboxRenderer::render()
  {
    bool debug = false;

    std::string token_path = "/sdcard/gisrenderer/mapbox-token.txt";
    std::string style_path = "/sdcard/gisrenderer/mapbox-basic-style.json";
    std::string cache_file = "/sdcard/gisrenderer/cache.sqlite";
    std::string asset_root = ".";
//    static std::string output = "/sdcard/out.png";

//    double lat = 0, lon = 0;
//    double zoom = 0;
    double lat = 55.7522200, lon = 37.6155600; // Moscow
    double zoom = 11;
    double bearing = 0;
    double pitch = 0;

    float pixelRatio = 1.0;

    std::vector <std::string> classes;
    mbgl::util::RunLoop loop;

    std::string token = read_file(token_path);
    std::string style = read_file(style_path);

    mbgl::DefaultFileSource fileSource(cache_file, asset_root);

    // Set access token if present
    if (token.size()) {
      fileSource.setAccessToken(std::string(token));
    }

    mbgl::HeadlessView view(pixelRatio, mImageWidth, mImageHeight);
    mbgl::Map map(view, fileSource, mbgl::MapMode::Still);

    map.setStyleJSON(style, ".");
    map.setClasses(classes);

    map.setLatLngZoom({lat, lon}, zoom);
    map.setBearing(bearing);
    map.setPitch(pitch);

    if (debug) {
      map.setDebug(
          debug
              ? mbgl::MapDebugOptions::TileBorders | mbgl::MapDebugOptions::ParseStatus
              : mbgl::MapDebugOptions::NoDebug);
    }

    map.renderStill(
        [&](
            std::exception_ptr error,
            mbgl::PremultipliedImage&& image)
        {
          try {
            if (error) {
              std::rethrow_exception(error);
            }
          } catch (std::exception& e) {
            mbgl::Log::Error(mbgl::Event::Render, "Error: %s", e.what());
            exit(1);
          }

//          write_file(output, mbgl::encodePNG(image));
          std::memcpy(mpImageData, image.data.get(), image.size());
          loop.stop();
        });

    loop.run();
  }
}
