#include <array>
#include <memory>

#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>

/***********************************************************************
  This is the only file you need to change for your assignment.  The
  other files control the UI (in case you want to make changes.)
************************************************************************/

// Helpers
namespace {

// Normalize the given kernel of |length| size.
static void NormalizeKernel(double* kernel, int length)
{
	double denom = 0.000001;
	for (int i = 0; i < length; ++i) {
		denom += kernel[i];
	}

	for (int i = 0; i < length; ++i) {
		kernel[i] /= denom;
	}
}

// Returns a pixel from the image at the given x,y coordinates. Returns black
// if out of bounds.
static QRgb PixelAt(const QImage& image, int x, int y)
{
	if (x >= 0 && y >= 0 && x < image.width() && y < image.height())
		return image.pixel(x, y);

	return 0;
}

// Convolves the given filter with the given radius over the image, then applies 
// a final color to each pixel at the end of the operation.
static void ConvolveImage(QImage* image, const double* kernel, int radius, QRgb addColor)
{
	QImage buffer = image->copy(-radius, -radius, image->width() + radius * 2, image->height() + radius * 2);
	int size = radius * 2 + 1;
	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			double rgb[3] {0, 0, 0};

			for (int rd = -radius; rd <= radius; ++rd)
			{
				for (int cd = -radius; cd <= radius; ++cd)
				{
					QRgb pixel = buffer.pixel(c + cd + radius, r + rd + radius);

					// Get the value of the kernel
					double weight = kernel[(rd + radius) * size + cd + radius];

					rgb[0] += weight * (double)qRed(pixel);
					rgb[1] += weight * (double)qGreen(pixel);
					rgb[2] += weight * (double)qBlue(pixel);
				}
			}

			rgb[0] = min(255., max(0., rgb[0] + qRed(addColor)));
			rgb[1] = min(255., max(0., rgb[1] + qGreen(addColor)));
			rgb[2] = min(255., max(0., rgb[2] + qBlue(addColor)));

			image->setPixel(c, r, qRgb((int)floor(rgb[0] + 0.5), (int)floor(rgb[1] + 0.5), (int)floor(rgb[2] + 0.5)));
		}
	}
}

// Applies the given filter at a single point in the image and returns the red component of that point.
static double ApplyFilterAt(const QImage& image, int x, int y, const double* kernel, int radius)
{
	double intensity;
	int size = 2 * radius + 1;
	for (int rd = -radius; rd <= radius; ++rd)
	{
		for (int cd = -radius; cd <= radius; ++cd)
		{
			QRgb pixel = PixelAt(image, x + cd + radius, y + rd + radius);

			// Get the value of the kernel
			double weight = kernel[(rd + radius) * size + cd + radius];

			intensity += weight * (double)qRed(pixel);
		}
	}

	return intensity;
}

// Scales the given RGB color by |scale| and returns the results as vector of doubles.
static std::vector<double> ScaleColor(QRgb color, double scale)
{
	std::vector<double> scaledColor(3);
	scaledColor[0] = (double)qRed(color) * scale;
	scaledColor[1] = (double)qGreen(color) * scale;
	scaledColor[2] = (double)qBlue(color) * scale;
	return scaledColor;
}

// Returns the magintude of the given vector.
static double Magnitude(double* vec, int dims)
{
	double sum = 0.0;
	for (int i = 0; i < dims; ++i)
	{
		sum += vec[i] * vec[i];
	}
	return sqrt(sum);
}

// Returns the distance between the two points with |dim| size.
static double Distance(const double* p1, const double* p2, int dim)
{
	double dist = 0.0;
	for (int i = 0; i < dim; ++i)
	{
		double dimDist = p1[i] - p2[i];
		dist += dimDist * dimDist;
	}
	return sqrt(dist);
}

// Tests whether the given point is sufficiently different from the rest of the seeds.
static bool SufficientlyDifferentSeed(const std::vector<double[3]>& seeds, const double* current, int cap)
{
	const double kMinDist = 100.0;
	// Check distances
	for (int i = 0; i < cap; ++i)
	{
		if (Distance(current, seeds[i], 3) < kMinDist)
			return false;
	}

	return true;
}

// Applies the KMeans algorithm to the given image with the number of clusters and an initial set
// of seeds.
static void ApplyKMeans(QImage* image, int numCluster, std::vector<double[3]>& kMeans)
{
	const int kMaxIter = 100;

	std::vector<int> pixelClusters(image->width() * image->height());
	for (int iterCount = 1; iterCount <= kMaxIter; ++iterCount)
	{
		std::vector<int> clusterCount(numCluster, 0);
		// Pick cluster
		for (int r = 0; r < image->height(); ++r)
		{
			for (int c = 0; c < image->width(); ++c)
			{
				auto pixel = ScaleColor(PixelAt(*image, c, r), 1);

				double minDist = Distance(kMeans[0], pixel.data(), 3);
				int minIndex = 0;
				for (int i = 1; i < numCluster; ++i)
				{
					double dist = Distance(kMeans[i], pixel.data(), 3);
					if (dist < minDist)
					{
						minDist = dist;
						minIndex = i;
					}
				}

				pixelClusters[r * image->width() + c] = minIndex;
				clusterCount[minIndex]++;
			}
		}

		// Reset cluster means
		for (int i = 0; i < numCluster; ++i)
		{
			kMeans[i][0] = 0;
			kMeans[i][1] = 0;
			kMeans[i][2] = 0;
		}

		// Update the means
		for (int i = 0; i < pixelClusters.size(); ++i)
		{
			int clusterIndex = pixelClusters[i];
			int c = i % image->width();
			int r = i / image->width();
			auto pixel = ScaleColor(image->pixel(c, r), 1);
			kMeans[clusterIndex][0] += pixel[0];
			kMeans[clusterIndex][1] += pixel[1];
			kMeans[clusterIndex][2] += pixel[2];
		}

		for (int i = 0; i < numCluster; ++i)
		{
			if (clusterCount[i] != 0)
			{
				kMeans[i][0] /= clusterCount[i];
				kMeans[i][1] /= clusterCount[i];
				kMeans[i][2] /= clusterCount[i];
			}
		}
	}

	// Label image based on cluster
	// Update the means
	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			int clusterIndex = pixelClusters[r * image->width() + c];
			const double* color = kMeans[clusterIndex];
			image->setPixel(c, r, qRgb((int)floor(color[0] + 0.5), (int)floor(color[1] + 0.5), (int)floor(color[2] + 0.5)));
		}
	}
}

}
// The first four functions provide example code to help get you started

// Convert an image to grey-scale
void MainWindow::BlackWhiteImage(QImage *image)
{
    int r, c;
    QRgb pixel;

    for(r=0;r<image->height();r++)
    {
        for(c=0;c<image->width();c++)
        {
            pixel = image->pixel(c, r);
            double red = (double) qRed(pixel);
            double green = (double) qGreen(pixel);
            double blue = (double) qBlue(pixel);

            // Compute intensity from colors - these are common weights
            double intensity = 0.3*red + 0.6*green + 0.1*blue;

            image->setPixel(c, r, qRgb( (int) intensity, (int) intensity, (int) intensity));
        }
    }
}

// Add random noise to the image
void MainWindow::AddNoise(QImage *image, double mag, bool colorNoise)
{
    int r, c;
    QRgb pixel;
    int noiseMag = mag;
    noiseMag *= 2;

    for(r=0;r<image->height();r++)
    {
        for(c=0;c<image->width();c++)
        {
            pixel = image->pixel(c, r);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            // If colorNoise, add color independently to each channel
            if(colorNoise)
            {
                red += rand()%noiseMag - noiseMag/2;
                green += rand()%noiseMag - noiseMag/2;
                blue += rand()%noiseMag - noiseMag/2;
            }
            // otherwise add the same amount of noise to each channel
            else
            {
                int noise = rand()%noiseMag - noiseMag/2;

                red += noise;
                green += noise;
                blue += noise;
            }

            // Make sure we don't over or under saturate
            red = min(255, max(0, red));
            green = min(255, max(0, green));
            blue = min(255, max(0, blue));

            image->setPixel(c, r, qRgb( red, green, blue));
        }
    }
}

// Here is an example of blurring an image using a mean or box filter with the specified radius.
// This could be implemented using separable filters to make it much more efficient, but it is not.
void MainWindow::MeanBlurImage(QImage *image, int radius)
{
    if(radius == 0)
        return;

    int r, c, rd, cd, i;
    QRgb pixel;

    // This is the size of the kernel
    int size = 2*radius + 1;

    // Create a buffer image so we're not reading and writing to the same image during filtering.
    QImage buffer;
    int w = image->width();
    int h = image->height();

    // This creates an image of size (w + 2*radius, h + 2*radius) with black borders.
    // This could be improved by filling the pixels using a different padding technique (reflected, fixed, etc.)
    buffer = image->copy(-radius, -radius, w + 2*radius, h + 2*radius);

    // Compute kernel to convolve with the image.
    double *kernel = new double [size*size];

    for(i=0;i<size*size;i++)
    {
        kernel[i] = 1.0;
    }

    // Make sure kernel sums to 1
    double denom = 0.000001;
    for(i=0;i<size*size;i++)
        denom += kernel[i];
    for(i=0;i<size*size;i++)
        kernel[i] /= denom;

    // For each pixel in the image...
    for(r=0;r<h;r++)
    {
        for(c=0;c<w;c++)
        {
            double rgb[3];

            rgb[0] = 0.0;
            rgb[1] = 0.0;
            rgb[2] = 0.0;

            // Convolve the kernel at each pixel
            for(rd=-radius;rd<=radius;rd++)
                for(cd=-radius;cd<=radius;cd++)
                {
                     // Get the pixel value
                     pixel = buffer.pixel(c + cd + radius, r + rd + radius);

                     // Get the value of the kernel
                     double weight = kernel[(rd + radius)*size + cd + radius];

                     rgb[0] += weight*(double) qRed(pixel);
                     rgb[1] += weight*(double) qGreen(pixel);
                     rgb[2] += weight*(double) qBlue(pixel);
                }

            // Store mean pixel in the image to be returned.
            image->setPixel(c, r, qRgb((int) floor(rgb[0] + 0.5), (int) floor(rgb[1] + 0.5), (int) floor(rgb[2] + 0.5)));
        }
    }

    // Clean up.
    delete [] kernel;
}

// Downsample the image by 1/2
void MainWindow::HalfImage(QImage &image)
{
    QImage buffer;
    int w = image.width();
    int h = image.height();
    int r, c;

    buffer = image.copy();

    // Reduce the image size.
    image = QImage(w/2, h/2, QImage::Format_RGB32);

    // Copy every other pixel
    for(r=0;r<h/2;r++)
        for(c=0;c<w/2;c++)
        {
             image.setPixel(c, r, buffer.pixel(c*2, r*2));
        }
}



void MainWindow::GaussianBlurImage(QImage *image, double sigma)
{
	if (sigma == 0)
	{
		return;
	}

	const int radius = (int)(3 * sigma);
	const int size = radius * 2 + 1;
	QImage buffer = image->copy(-radius, -radius, 
		image->width() + radius * 2, image->height() + radius * 2);

	std::vector<double> kernel(size * size);
	for (int i = 0; i < size * size; ++i)
	{
		int u = i % size - radius;
		int v = i / size - radius;
		double numerator = pow(M_E, -(u * u + v * v) / (2 * sigma * sigma));
		double denominator = sqrt(2 * M_PI) * sigma;
		kernel[i] = numerator / denominator;
	}

	// Make sure kernel sums to 1
	NormalizeKernel(kernel.data(), size * size);
	
	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			double rgb[3] {0, 0, 0};

			for (int rd = -radius; rd <= radius; ++rd)
			{
				for (int cd = -radius; cd <= radius; ++cd)
				{
					QRgb pixel = buffer.pixel(c + cd + radius, r + rd + radius);
					double weight = kernel[(rd + radius) * size + cd + radius];
					rgb[0] += weight * (double)qRed(pixel);
					rgb[1] += weight * (double)qGreen(pixel);
					rgb[2] += weight * (double)qBlue(pixel);
				}
			}

			image->setPixel(c, r, qRgb((int)floor(rgb[0] + 0.5), (int)floor(rgb[1] + 0.5), (int)floor(rgb[2] + 0.5)));
		}
	}
}

void MainWindow::SeparableGaussianBlurImage(QImage *image, double sigma)
{
    // Add your code here.  Done right, you should be able to copy most of the code from GaussianBlurImage.
	if (sigma == 0)
	{
		return;
	}

	const int radius = (int)(3 * sigma);
	const int size = radius * 2 + 1;
	QImage buffer = image->copy(-radius, -radius,
		image->width() + radius * 2, image->height() + radius * 2);

	std::vector<double> kernel(size);
	for (int i = 0; i < size; ++i)
	{
		// Only calculate a single distance, horizontal for reference.
		int u = i - radius;
		double numerator = pow(M_E, -(u * u) / (2 * sigma * sigma));
		double denominator = sqrt(2 * M_PI) * sigma;
		kernel[i] = numerator / denominator;
	}

	// Make sure kernel sums to 1
	NormalizeKernel(kernel.data(), size);

	QImage temp = QImage(image->width() + radius * 2, image->height() + radius * 2, image->format());
	temp.fill(Qt::GlobalColor::black);

	// Horizontal pass
	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			double rgb[3] {0, 0, 0};

			for (int cd = -radius; cd <= radius; ++cd)
			{
				QRgb pixel = buffer.pixel(c + cd + radius, r + radius);
				double weight = kernel[cd + radius];
				rgb[0] += weight * (double)qRed(pixel);
				rgb[1] += weight * (double)qGreen(pixel);
				rgb[2] += weight * (double)qBlue(pixel);
			}

			temp.setPixel(c + radius, r + radius, qRgb((int)floor(rgb[0] + 0.5), (int)floor(rgb[1] + 0.5), (int)floor(rgb[2] + 0.5)));

		}
	}

	// Vertical pass
	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			double rgb[3] {0, 0, 0};

			for (int rd = -radius; rd <= radius; ++rd)
			{
				QRgb pixel = temp.pixel(c + radius, r + rd + radius);
				double weight = kernel[rd + radius];
				rgb[0] += weight * (double)qRed(pixel);
				rgb[1] += weight * (double)qGreen(pixel);
				rgb[2] += weight * (double)qBlue(pixel);
			}

			image->setPixel(c, r, qRgb((int)floor(rgb[0] + 0.5), (int)floor(rgb[1] + 0.5), (int)floor(rgb[2] + 0.5)));
		}
	}
}

void MainWindow::FirstDerivImage(QImage *image, double sigma)
{
    // Add your code here.
	if (sigma == 0)
		return;

	std::vector<double> kernel{
		0, 0, 0,
		1, 0, -1,
		0, 0, 0,
	};
	ConvolveImage(image, kernel.data(), 1, qRgb(128, 128, 128));
	SeparableGaussianBlurImage(image, sigma);
}

void MainWindow::SecondDerivImage(QImage *image, double sigma)
{
	// Add your code here.
	if (sigma == 0)
		return;

	std::vector<double> kernel{
		0, 1, 0,
		1, -4, 1,
		0, 1, 0,
	};
	ConvolveImage(image, kernel.data(), 1, qRgb(128, 128, 128));
	SeparableGaussianBlurImage(image, sigma);
}

void MainWindow::SharpenImage(QImage *image, double sigma, double alpha)
{
    // Add your code here.  It's probably easiest to call SecondDerivImage as a helper function.
	// Add your code here.
	if (sigma == 0)
		return;

	SeparableGaussianBlurImage(image, sigma);
	std::vector<double> kernel{
		0,          -1 * alpha,    0,
		-1 * alpha, 1 + 4 * alpha, -1 * alpha,
		0,          -1 * alpha,    0,
	};
	ConvolveImage(image, kernel.data(), 1, 0);
}

void MainWindow::BilateralImage(QImage *image, double sigmaS, double sigmaI)
{
    // Add your code here.  Should be similar to GaussianBlurImage.
}

void MainWindow::SobelImage(QImage *image)
{
    // Add your code here.

    /***********************************************************************
      When displaying the orientation image I
      recommend the following:

    double mag; // magnitude of the gradient
    double orien; // orientation of the gradient

    double red = (sin(orien) + 1.0)/2.0;
    double green = (cos(orien) + 1.0)/2.0;
    double blue = 1.0 - red - green;

    red *= mag*4.0;
    green *= mag*4.0;
    blue *= mag*4.0;

    // Make sure the pixel values range from 0 to 255
    red = min(255.0, max(0.0, red));
    green = min(255.0, max(0.0, green));
    blue = min(255.0, max(0.0, blue));

    image->setPixel(c, r, qRgb( (int) (red), (int) (green), (int) (blue)));

    ************************************************************************/
	BlackWhiteImage(image);
	QImage buffer = image->copy(0, 0, image->width(), image->height());
	
	std::vector<double> sobelHorizontalMag{
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1,
	};

	std::vector<double> sobelVerticalMag{
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			double xMagnitude = ApplyFilterAt(buffer, c, r, sobelHorizontalMag.data(), 1);
			double yMagnitude = ApplyFilterAt(buffer, c, r, sobelVerticalMag.data(), 1);

			double magnitude = sqrt(xMagnitude * xMagnitude + yMagnitude * yMagnitude);
			double orientation = atan2(yMagnitude, xMagnitude);

			double red = (sin(orientation) + 1.0) / 2.0;
			double green = (cos(orientation) + 1.0) / 2.0;
			double blue = 1.0 - red - green;

			red *= magnitude * 4.0;
			green *= magnitude * 4.0;
			blue *= magnitude * 4.0;

			red = min(255.0, max(0.0, red));
			green = min(255.0, max(0.0, green));
			blue = min(255.0, max(0.0, blue));
			image->setPixel(c, r, qRgb((int)floor(red + 0.5), (int)floor(green + 0.5), (int)floor(blue + 0.5)));
		}
	}
}


void MainWindow::BilinearInterpolation(QImage *image, double x, double y, double rgb[3])
{
    // Add your code here.  Return the RGB values for the pixel at location (x,y) in double rgb[3].
	int x1 = (int)x;
	int y1 = (int)y;
	int x2 = x1 + 1;
	int y2 = y1 + 1;

	QRgb q11 = PixelAt(*image, x1, y1);
	QRgb q12 = PixelAt(*image, x1, y2);
	QRgb q21 = PixelAt(*image, x2, y1);
	QRgb q22 = PixelAt(*image, x2, y2);

	auto r1 = ScaleColor(q11, (x2 - x) / (x2 - x1));
	auto r12 = ScaleColor(q21, ((x - x1) / (x2 - x1)));
	r1[0] += r12[0];
	r1[1] += r12[1];
	r1[2] += r12[2];

	auto r2 = ScaleColor(q12, (x2 - x) / (x2 - x1));
	auto r22 = ScaleColor(q22, (x - x1) / (x2 - x1));
	r2[0] += r22[0];
	r2[1] += r22[1];
	r2[2] += r22[2];

	double scale1 = (y2 - y) / (y2 - y1);
	double scale2 = (y - y1) / (y2 - y1);

	r1[0] *= scale1;
	r1[1] *= scale1;
	r1[2] *= scale1;
		  
	r2[0] *= scale2;
	r2[1] *= scale2;
	r2[2] *= scale2;

	rgb[0] = r1[0] + r2[0];
	rgb[1] = r1[1] + r2[1];
	rgb[2] = r1[2] + r2[2];

}

// Here is some sample code for rotating an image.  I assume orien is in degrees.

void MainWindow::RotateImage(QImage *image, double orien)
{
    int r, c;
    QRgb pixel;
    QImage buffer;
    int w = image->width();
    int h = image->height();
    double radians = -2.0*3.141*orien/360.0;

    buffer = image->copy();

    pixel = qRgb(0, 0, 0);
    image->fill(pixel);

    for(r=0;r<h;r++)
    {
        for(c=0;c<w;c++)
        {
            double rgb[3];
            double x0, y0;
            double x1, y1;

            // Rotate around the center of the image.
            x0 = (double) (c - w/2);
            y0 = (double) (r - h/2);

            // Rotate using rotation matrix
            x1 = x0*cos(radians) - y0*sin(radians);
            y1 = x0*sin(radians) + y0*cos(radians);

            x1 += (double) (w/2);
            y1 += (double) (h/2);

            BilinearInterpolation(&buffer, x1, y1, rgb);

            image->setPixel(c, r, qRgb((int) floor(rgb[0] + 0.5), (int) floor(rgb[1] + 0.5), (int) floor(rgb[2] + 0.5)));
        }
    }

}

void MainWindow::FindPeaksImage(QImage *image, double thres)
{
    // Add your code here.
	QImage sobelImage = image->copy(0, 0, image->width(), image->height());
	SobelImage(&sobelImage);


	for (int r = 0; r < image->height(); ++r)
	{
		for (int c = 0; c < image->width(); ++c)
		{
			QRgb pixel = sobelImage.pixel(c, r);
			double gradient[3] { qRed(pixel), qGreen(pixel), qBlue(pixel) };
			double gradientNormal[2] { gradient[0], gradient[1] };
			double normalMag = Magnitude(gradientNormal, 2);
			gradientNormal[0] /= normalMag;
			gradientNormal[1] /= normalMag;

			double e0Vec[3];
			BilinearInterpolation(image, c + gradientNormal[0], r + gradientNormal[1], e0Vec);
			double e1Vec[3];
			BilinearInterpolation(image, c - gradientNormal[0], r - gradientNormal[1], e1Vec);

			double e0 = Magnitude(e0Vec, 2);
			double e1 = Magnitude(e1Vec, 2);
			double pixelMag = Magnitude(gradient, 2);

			if (pixelMag > e0 && pixelMag > e1 && pixelMag > thres)
				image->setPixel(c, r, qRgb(255, 255, 255));
			else
				image->setPixel(c, r, 0);
		}
	}

}


void MainWindow::MedianImage(QImage *image, int radius)
{
    // Add your code here
}

void MainWindow::HoughImage(QImage *image)
{
    // Add your code here
}

void MainWindow::CrazyImage(QImage *image)
{
    // Add your code here
}

void MainWindow::RandomSeedImage(QImage *image, int num_clusters)
{
     // Add your code here
	const int kMaxIter = 100;
	const double kMinDist = 100;

	std::vector<double[3]> kMeans(num_clusters);
	// Randomly select initial clusters centroids
	for (int k = 0; k < num_clusters; ++k)
	{
		do
		{
			kMeans[k][0] = (double)rand() / RAND_MAX * 255.0;
			kMeans[k][1] = (double)rand() / RAND_MAX * 255.0;
			kMeans[k][2] = (double)rand() / RAND_MAX * 255.0;

		} while (!SufficientlyDifferentSeed(kMeans, kMeans[k], k - 1));
	}

	ApplyKMeans(image, num_clusters, kMeans);
}

void MainWindow::PixelSeedImage(QImage *image, int num_clusters)
{
	std::vector<double[3]> kMeans(num_clusters);
	// Randomly select initial clusters centroids
	for (int k = 0; k < num_clusters; ++k)
	{
		do
		{
			int x = rand() % image->width();
			int y = rand() % image->height();
			QRgb pixel = image->pixel(x, y);
			kMeans[k][0] = qRed(pixel);
			kMeans[k][1] = qGreen(pixel);
			kMeans[k][2] = qBlue(pixel);

		} while (!SufficientlyDifferentSeed(kMeans, kMeans[k], k - 1));
	}

	ApplyKMeans(image, num_clusters, kMeans);
}

void MainWindow::HistogramSeedImage(QImage *image, int num_clusters)
{
    // Add your code here
}
