#include "volt/gfx/Image.hpp"
#include "volt/gfx/extern/stb/stb_image.h"
#include "volt/gfx/extern/stb/stb_image_write.h"

#include <stdexcept>

using namespace volt::gfx;

Image::Image(std::string const &path)
{
    // Ensure that stb_image uses images using OpenGL's coordinate system
    // TODO: Look into stbi_set_flip_vertically_on_load_thread()
    stbi_set_flip_vertically_on_load(true);
    stbi_flip_vertically_on_write(true);

    // Ensure that the resulting image always has four color channels (RGBA)
    int      colorChannelsCount = 0;
    stbi_uc *imageData = stbi_load(path.c_str(), &this->width, &this->height,
                                   &colorChannelsCount, 0);

    this->imageData = std::vector<std::uint8_t>();
    this->imageData.resize(this->width * this->height * 4);

    // This loops over every pixel
    for (std::size_t i = 0; i < this->width * this->height; i++)
    {
        // Ensure that this is copied over properly, filling in with apprpriate
        // bytes for any color channel that the source image does not support
        for (std::size_t j = 0; j < 4; j++)
        {
            std::size_t fillIndex = (4 * i) + j;
            if (j < colorChannelsCount) // Source image has this color channel
                this->imageData[fillIndex] =
                    imageData[(colorChannelsCount * i) + j];
            else if (j == 3) // This is the alpha channel, fill with 255
                this->imageData[fillIndex] = 127;
            else // This is a different channel, fill with 0
                this->imageData[fillIndex] = 0;
        }
    }
    stbi_image_free(imageData);
}

Image::Image(std::vector<std::uint8_t> const &data, GLsizei w, GLsizei h)
{
    this->width     = w;
    this->height    = h;
    this->imageData = data;
}

Image::Image(std::vector<std::uint8_t> &&data, GLsizei w, GLsizei h)
{
    this->width     = w;
    this->height    = h;
    this->imageData = std::move(data);
}

Image::Image(const Image &other)
    : imageData(other.imageData), width(other.width), height(other.height)
{
}

Image &Image::operator=(const Image &other)
{
    this->imageData = other.imageData;
    this->width     = other.width;
    this->height    = other.height;
    return *this;
}

Image::Image(Image &&other)
    : imageData(std::move(other.imageData)), width(other.width),
      height(other.height)
{
}

Image &Image::operator=(Image &&other)
{
    this->imageData = other.imageData;
    this->width     = other.width;
    this->height    = other.height;
    return *this;
}

Image::~Image() {}

void Image::Save(std::string const &filePath)
{
    if (stbi_write_png(filePath.c_str(), this->width, this->height, 4,
                       this->imageData.data(), 0) == 0)
        throw std::runtime_error("Could not save image: '" + filePath + "\'");
}
