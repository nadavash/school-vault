// File containing helper methods for Project3 assignment.
#include <math.h>

#include <qimage.h>

// Returns a pixel from the image at the given x,y coordinates. Uses the 'fixed' method.
static double FixedPixelAt(const double* image, int w, int h, int x, int y)
{
	x = std::min(std::max(x, 0), w - 1);
	y = std::min(std::max(y, 0), h - 1);
	return image[x + y * w];
}

// Returns a pixel from the image at the given x,y coordinates.
static double BlackPixelAt(const double* image, int w, int h, int x, int y)
{
	if (x < 0 || y < 0 || x >= w || y >= h)
		return 0;

	return image[x + y * w];
}


// Convert the given image to graytone.
std::vector<double> GraytoneImage(const QImage& image)
{
	std::vector<double> buffer(image.width() * image.height());
	for (int r = 0; r < image.height(); r++)
		for (int c = 0; c < image.width(); c++)
		{
			QRgb pixel = image.pixel(c, r);

			buffer[r*image.width() + c] = 0.21 * (double)qRed(pixel) + 0.72 * (double)qGreen(pixel) + 0.07 * (double)qBlue(pixel);
		}

	return buffer;
}

// Normalize the given buffer of |length| size.
static void Normalize(double* buff, int length)
{
	double denom = 0.000001;
	for (int i = 0; i < length; ++i) {
		denom += buff[i];
	}

	for (int i = 0; i < length; ++i) {
		buff[i] /= denom;
	}
}
