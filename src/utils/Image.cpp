#include "Image.hpp"

#include <cstring>
#include <iostream>

#include <lodepng/lodepng.h>

const uint Image::m_colorComponents = 3; // Red, blue, green

//---------------------------------------------------------------------------------------
/*
 * Default constructor creates an empty image.
 */
Image::Image()
    : m_width(0),
      m_height(0),
      m_data(nullptr)
{}

//---------------------------------------------------------------------------------------
/*
 * Custom constructor creates a black image of the given width and height.
 */
Image::Image(
    const uint width,
    const uint height
)
    : m_width(width),
      m_height(height)
{
    const size_t numElements = m_width * m_height * m_colorComponents;
    m_data = new double[numElements];
    memset(m_data, 0, numElements * sizeof(double));
}

//---------------------------------------------------------------------------------------
/*
 * Copy constructor.
 */
Image::Image(const Image& other)
    : m_width(other.m_width),
      m_height(other.m_height),
      m_data(other.m_data ? new double[m_width * m_height * m_colorComponents] : nullptr)
{
    std::memcpy(m_data, other.m_data, m_width * m_height * m_colorComponents * sizeof(double));
}

//---------------------------------------------------------------------------------------
/*
 * Destructor.
 */
Image::~Image()
{
    delete [] m_data;
}

//---------------------------------------------------------------------------------------
/*
 * Copy assignment operator.
 */
Image& Image::operator=(const Image& other)
{
    if (this == &other)
    {
        return *this;
    }

    delete [] m_data;

    m_width = other.m_width;
    m_height = other.m_height;
    m_data = (other.m_data ? new double[m_width * m_height * m_colorComponents] : nullptr);

    if (m_data)
    {
        std::memcpy(m_data,
                    other.m_data,
                    m_width * m_height * m_colorComponents * sizeof(double)
        );
    }

    return *this;
}

//---------------------------------------------------------------------------------------
/*
 * width returns the width of the image.
 */
uint Image::width() const
{
    return m_width;
}

//---------------------------------------------------------------------------------------
/*
 * height returns the height of the image.
 */
uint Image::height() const
{
    return m_height;
}

//---------------------------------------------------------------------------------------
/*
 * operator() retrieves the color at pixel (x,y).
 */
Color Image::operator()(uint x, uint y) const
{
    return {
        m_data[m_colorComponents * (m_width * y + x)],
        m_data[m_colorComponents * (m_width * y + x) + 1],
        m_data[m_colorComponents * (m_width * y + x) + 2]
    };
}

//---------------------------------------------------------------------------------------
/*
 * operator() retrieves the i'th color component at pixel (x,y) as a const.
 */
double Image::operator()(uint x, uint y, uint i) const
{
    return m_data[m_colorComponents * (m_width * y + x) + i];
}

//---------------------------------------------------------------------------------------
/*
 * operator() retrieves the i'th color component at pixel (x,y) as a reference.
 */
double& Image::operator()(uint x, uint y, uint i)
{
    return m_data[m_colorComponents * (m_width * y + x) + i];
}

//---------------------------------------------------------------------------------------
/*
 * clampNormalize clamps x to the range [0.0, 1.0].
 */
static double clampNormalize(const double x)
{
    return x < 0.0 ? 0.0 : (x > 1.0 ? 1.0 : x);
}

//---------------------------------------------------------------------------------------
/*
 * readPng reads a PNG file from disk given a filename into the image.
 */
bool Image::readPng(const std::string& filename)
{
    std::vector<unsigned char> image;

    // Decode the image into image vector
    if (const unsigned error = lodepng::decode(image, m_width, m_height, filename, LCT_RGB))
    {
        std::cerr << "decoder error " << error << ": " << lodepng_error_text(error)
                << std::endl;
    }

    // Set the size of m_data
    size_t numElements = m_width * m_height * m_colorComponents;
    m_data = new double[numElements];
    memset(m_data, 0, numElements * sizeof(double));

    // Convert each color to a double between 0 and 1 and save to m_data
    for (uint y(0); y < m_height; y++)
    {
        for (uint x(0); x < m_width; x++)
        {
            for (uint i(0); i < m_colorComponents; ++i)
            {
                double color = static_cast<double>(image[m_colorComponents * (m_width * y + x) + i]) / 255;
                color = clampNormalize(color);
                m_data[m_colorComponents * (m_width * y + x) + i] = color;
            }
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------
/*
 * savePng writes the image to a PNG file given a filename.
 */
bool Image::savePng(const std::string& filename) const
{
    std::vector<unsigned char> image;

    image.resize(m_width * m_height * m_colorComponents);

    for (uint y(0); y < m_height; y++)
    {
        for (uint x(0); x < m_width; x++)
        {
            for (uint i(0); i < m_colorComponents; ++i)
            {
                double color = m_data[m_colorComponents * (m_width * y + x) + i];
                color = clampNormalize(color);
                image[m_colorComponents * (m_width * y + x) + i] = static_cast<unsigned char>(255 * color);
            }
        }
    }

    // Encode the image
    if (const unsigned error = lodepng::encode(filename, image, m_width, m_height, LCT_RGB))
    {
        std::cerr << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }

    return true;
}

//---------------------------------------------------------------------------------------
/*
 * data returns a pointer to the image data as a const.
 */
const double* Image::data() const
{
    return m_data;
}

//---------------------------------------------------------------------------------------
/*
 * data returns a pointer to the image data.
 */
double* Image::data()
{
    return m_data;
}
