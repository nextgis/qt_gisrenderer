#ifndef GMCORE_MAPBOXRENDER_H
#define GMCORE_MAPBOXRENDER_H

#include <cinttypes>


namespace gmcore
{
  class MapboxRenderer
  {
  public:
    explicit MapboxRenderer(
        void* pImageData,
        uint16_t imageWidth,
        uint16_t imageHeight);

    void render();

  private:
    void* mpImageData;
    uint16_t mImageWidth;
    uint16_t mImageHeight;
  };
}

#endif  // GMCORE_MAPBOXRENDER_H
