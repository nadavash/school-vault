#include <unordered_set>

#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include "Matrix.h"

namespace {


// Returns a pixel from the image at the given x,y coordinates. Uses the 'fixed' method.
static QRgb PixelAt(const QImage& image, int x, int y)
{
	x = min(max(x, 0), image.width() - 1);
	y = min(max(y, 0), image.height() - 1);
	return image.pixel(x, y);

	return 0;
}


// Returns a pixel from the image at the given x,y coordinates. Uses the 'fixed' method.
static double PixelAt(const double* image, int w, int h, int x, int y)
{
	x = min(max(x, 0), w - 1);
	y = min(max(y, 0), h - 1);
	return image[x + y * w];
}

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

// Converts the buffer to a QImage.
static void BufferToImage(const double* buffer, QImage* image)
{
	int w = image->width();
	int h = image->height();
	for (int r = 0; r < h; ++r)
	{
		for (int c = 0; c < w; ++c)
		{
			int value = min(max((int)floor(buffer[c + r * w] + 0.5), 0), 255);
			image->setPixel(c, r, qRgb(value, value, value));
		}
	}
}

// Convolves the given filter with the given radius over the image, then applies 
// a final color to each pixel at the end of the operation.
static void ConvolveImage(double* image, int w, int h, const double* kernel, int radius)
{
	std::vector<double> buffer(image, image + w * h);

	int size = radius * 2 + 1;
	for (int r = 0; r < h; ++r)
	{
		for (int c = 0; c < w; ++c)
		{
			double intensity = 0.0;

			for (int rd = -radius; rd <= radius; ++rd)
			{
				for (int cd = -radius; cd <= radius; ++cd)
				{
					double pixel = PixelAt(buffer.data(), w, h, c + cd, r + rd);

					// Get the value of the kernel
					double weight = kernel[(rd + radius) * size + cd + radius];
					intensity += weight * pixel;
				}
			}

			image[c + r * w] = intensity;
		}
	}
}

// Multiplies the two given images with the provided with |w| and height |h|, and
// returns the result in the |out| buffer.
static void MultiplyImages(const double* image1, const double* image2, int w, int h, double* out)
{
	for (int i = 0; i < w * h; ++i)
	{
		out[i] = image1[i] * image2[i];
	}
}

// Computes the response over the given derivative images, width, height, window size, sigma and threshold
// and returns the results in |outImage|.
static void ComputeResponse(const double* ix2, const double* iy2, const double* ixy, int w, int h, int windowSize, double sigma, double thres, double* outImage)
{
	int radius = windowSize / 2;
	std::vector<double> kernel(windowSize * windowSize, 0);
	for (int i = 0; i < kernel.size(); ++i)
	{
		int u = i % windowSize - radius;
		int v = i / windowSize - radius;
		double numerator = pow(M_E, -(u * u + v * v) / (2 * sigma * sigma));
		double denominator = sqrt(2 * M_PI) * sigma;
		kernel[i] = numerator / denominator;
	}

	// Make sure kernel sums to 1
	NormalizeKernel(kernel.data(), windowSize * windowSize);

	std::vector<double> responseImage(w * h);
	for (int r = 0; r < h; ++r)
	{
		for (int c = 0; c < w; ++c)
		{
			double harrisMat[4]{ 0, 0, 0, 0 };

			for (int rd = -radius; rd <= radius; ++rd)
			{
				for (int cd = -radius; cd <= radius; ++cd)
				{
					double weight = kernel[cd + radius + (rd + radius) * windowSize];
					harrisMat[0] += weight * PixelAt(ix2, w, h, c + cd, r + rd);
					harrisMat[1] += weight * PixelAt(ixy, w, h, c + cd, r + rd);
					harrisMat[3] += weight * PixelAt(iy2, w, h, c + cd, r + rd);
				}
			}
			harrisMat[2] = harrisMat[1];

			// Compute response as det(H) / trace(H)
			double det = harrisMat[0] * harrisMat[3] - harrisMat[1] * harrisMat[2];
			double trace = harrisMat[0] + harrisMat[3];
			double response = det / trace;
			outImage[c + r * w] = response >= thres ? response : 0.0;
		}
	}
}

// Does a local maxima check over the given image at the given coordinates.
static double LocalMaxima(const double* image, int w, int h, int c, int r)
{
	const int kRadius = 2;
	double pixel = image[c + r * w];
	for (int rd = -kRadius; rd < kRadius; ++rd)
	{
		for (int cd = -kRadius; cd < kRadius; ++cd)
		{
			if (cd == 0 && rd == 0)
				continue;

			if (pixel <= PixelAt(image, w, h, c + cd, r + rd))
				return false;
		}
	}

	return true;
}

// Finds the interest points in the given Harris image.
static void FindInterestPoints(const double* image, int w, int h, CIntPt** points, int& numPoints)
{
	std::vector<CIntPt> interestPoints;
	for (int r = 0; r < h; ++r)
	{
		for (int c = 0; c < w; ++c)
		{
			if (LocalMaxima(image, w, h, c, r))
			{
				CIntPt point{ c, r };
				interestPoints.push_back(point);
			}
		}
	}

	numPoints = interestPoints.size();
	*points = new CIntPt[numPoints];
	std::copy(interestPoints.begin(), interestPoints.end(), *points);
}

// Calculates the L1 Norm distance between the descriptors of the two points.
static double NormDistanceL1(CIntPt point1, CIntPt point2)
{
	double sum = 0;
	for (int i = 0; i < DESC_SIZE; ++i)
	{
		sum += abs(point1.m_Desc[i] - point2.m_Desc[i]);
	}

	return sum;
}

}  // namespace

/*******************************************************************************
    The following are helper routines with code already written.
    The routines you'll need to write for the assignment are below.
*******************************************************************************/

/*******************************************************************************
Draw detected Harris corners
    interestPts - interest points
    numInterestsPts - number of interest points
    imageDisplay - image used for drawing

    Draws a red cross on top of detected corners
*******************************************************************************/
void MainWindow::DrawInterestPoints(CIntPt *interestPts, int numInterestsPts, QImage &imageDisplay)
{
   int i;
   int r, c, rd, cd;
   int w = imageDisplay.width();
   int h = imageDisplay.height();

   for(i=0;i<numInterestsPts;i++)
   {
       c = (int) interestPts[i].m_X;
       r = (int) interestPts[i].m_Y;

       for(rd=-2;rd<=2;rd++)
           if(r+rd >= 0 && r+rd < h && c >= 0 && c < w)
               imageDisplay.setPixel(c, r + rd, qRgb(255, 0, 0));

       for(cd=-2;cd<=2;cd++)
           if(r >= 0 && r < h && c + cd >= 0 && c + cd < w)
               imageDisplay.setPixel(c + cd, r, qRgb(255, 0, 0));
   }
}

/*******************************************************************************
Compute interest point descriptors
    image - input image
    interestPts - array of interest points
    numInterestsPts - number of interest points

    If the descriptor cannot be computed, i.e. it's too close to the boundary of
    the image, its descriptor length will be set to 0.

    I've implemented a very simple 8 dimensional descriptor.  Feel free to
    improve upon this.
*******************************************************************************/
void MainWindow::ComputeDescriptors(QImage image, CIntPt *interestPts, int numInterestsPts)
{
    int r, c, cd, rd, i, j;
    int w = image.width();
    int h = image.height();
    double *buffer = new double [w*h];
    QRgb pixel;

    // Descriptor parameters
    double sigma = 2.0;
    int rad = 4;

    // Computer descriptors from green channel
    for(r=0;r<h;r++)
       for(c=0;c<w;c++)
        {
            pixel = image.pixel(c, r);
            buffer[r*w + c] = (double) qGreen(pixel);
        }

    // Blur
    SeparableGaussianBlurImage(buffer, w, h, sigma);

    // Compute the desciptor from the difference between the point sampled at its center
    // and eight points sampled around it.
    for(i=0;i<numInterestsPts;i++)
    {
        int c = (int) interestPts[i].m_X;
        int r = (int) interestPts[i].m_Y;

        if(c >= rad && c < w - rad && r >= rad && r < h - rad)
        {
            double centerValue = buffer[(r)*w + c];
            int j = 0;

            for(rd=-1;rd<=1;rd++)
                for(cd=-1;cd<=1;cd++)
                    if(rd != 0 || cd != 0)
                {
                    interestPts[i].m_Desc[j] = buffer[(r + rd*rad)*w + c + cd*rad] - centerValue;
                    j++;
                }

            interestPts[i].m_DescSize = DESC_SIZE;
        }
        else
        {
            interestPts[i].m_DescSize = 0;
        }
    }

    delete [] buffer;
}

/*******************************************************************************
Draw matches between images
    matches - matching points
    numMatches - number of matching points
    image1Display - image to draw matches
    image2Display - image to draw matches

    Draws a green line between matches
*******************************************************************************/
void MainWindow::DrawMatches(CMatches *matches, int numMatches, QImage &image1Display, QImage &image2Display)
{
    int i;
    // Show matches on image
    QPainter painter;
    painter.begin(&image1Display);
    QColor green(0, 250, 0);
    QColor red(250, 0, 0);

    for(i=0;i<numMatches;i++)
    {
        painter.setPen(green);
        painter.drawLine((int) matches[i].m_X1, (int) matches[i].m_Y1, (int) matches[i].m_X2, (int) matches[i].m_Y2);
        painter.setPen(red);
        painter.drawEllipse((int) matches[i].m_X1-1, (int) matches[i].m_Y1-1, 3, 3);
    }

    QPainter painter2;
    painter2.begin(&image2Display);
    painter2.setPen(green);

    for(i=0;i<numMatches;i++)
    {
        painter2.setPen(green);
        painter2.drawLine((int) matches[i].m_X1, (int) matches[i].m_Y1, (int) matches[i].m_X2, (int) matches[i].m_Y2);
        painter2.setPen(red);
        painter2.drawEllipse((int) matches[i].m_X2-1, (int) matches[i].m_Y2-1, 3, 3);
    }

}


/*******************************************************************************
Given a set of matches computes the "best fitting" homography
    matches - matching points
    numMatches - number of matching points
    h - returned homography
    isForward - direction of the projection (true = image1 -> image2, false = image2 -> image1)
*******************************************************************************/
bool MainWindow::ComputeHomography(CMatches *matches, int numMatches, double h[3][3], bool isForward)
{
    int error;
    int nEq=numMatches*2;

    dmat M=newdmat(0,nEq,0,7,&error);
    dmat a=newdmat(0,7,0,0,&error);
    dmat b=newdmat(0,nEq,0,0,&error);

    double x0, y0, x1, y1;

    for (int i=0;i<nEq/2;i++)
    {
        if(isForward == false)
        {
            x0 = matches[i].m_X1;
            y0 = matches[i].m_Y1;
            x1 = matches[i].m_X2;
            y1 = matches[i].m_Y2;
        }
        else
        {
            x0 = matches[i].m_X2;
            y0 = matches[i].m_Y2;
            x1 = matches[i].m_X1;
            y1 = matches[i].m_Y1;
        }


        //Eq 1 for corrpoint
        M.el[i*2][0]=x1;
        M.el[i*2][1]=y1;
        M.el[i*2][2]=1;
        M.el[i*2][3]=0;
        M.el[i*2][4]=0;
        M.el[i*2][5]=0;
        M.el[i*2][6]=(x1*x0*-1);
        M.el[i*2][7]=(y1*x0*-1);

        b.el[i*2][0]=x0;
        //Eq 2 for corrpoint
        M.el[i*2+1][0]=0;
        M.el[i*2+1][1]=0;
        M.el[i*2+1][2]=0;
        M.el[i*2+1][3]=x1;
        M.el[i*2+1][4]=y1;
        M.el[i*2+1][5]=1;
        M.el[i*2+1][6]=(x1*y0*-1);
        M.el[i*2+1][7]=(y1*y0*-1);

        b.el[i*2+1][0]=y0;

    }
    int ret=solve_system (M,a,b);
    if (ret!=0)
    {
        freemat(M);
        freemat(a);
        freemat(b);

        return false;
    }
    else
    {
        h[0][0]= a.el[0][0];
        h[0][1]= a.el[1][0];
        h[0][2]= a.el[2][0];

        h[1][0]= a.el[3][0];
        h[1][1]= a.el[4][0];
        h[1][2]= a.el[5][0];

        h[2][0]= a.el[6][0];
        h[2][1]= a.el[7][0];
        h[2][2]= 1;
    }

    freemat(M);
    freemat(a);
    freemat(b);

    return true;
}


/*******************************************************************************
*******************************************************************************
*******************************************************************************

    The routines you need to implement are below

*******************************************************************************
*******************************************************************************
*******************************************************************************/


/*******************************************************************************
Blur a single channel floating point image with a Gaussian.
    image - input and output image
    w - image width
    h - image height
    sigma - standard deviation of Gaussian

    This code should be very similar to the code you wrote for assignment 1.
*******************************************************************************/
void MainWindow::SeparableGaussianBlurImage(double *image, int w, int h, double sigma)
{
    // Add your code here

    // To access the pixel (c,r), use image[r*width + c].
	// Add your code here.  Done right, you should be able to copy most of the code from GaussianBlurImage.
	if (sigma == 0)
		return;

	const int radius = (int)(3 * sigma);
	const int size = radius * 2 + 1;
	std::vector<double> buffer = std::vector<double>(image, image + (w * h));

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
	NormalizeKernel(kernel.data(), kernel.size());

	std::vector<double> temp(w * h, 0.0);

	// Horizontal pass
	for (int r = 0; r < h; ++r)
	{
		for (int c = 0; c < w; ++c)
		{
			double intensity = 0.0;

			for (int cd = -radius; cd <= radius; ++cd)
			{
				double pixel = PixelAt(image, w, h, c + cd, r);
				double weight = kernel[cd + radius];
				intensity += pixel * weight;
			}

			temp[c + r * w] = intensity;

		}
	}

	// Vertical pass
	for (int r = 0; r < h; ++r)
	{
		for (int c = 0; c < w; ++c)
		{
			double intensity = 0.0;

			for (int rd = -radius; rd <= radius; ++rd)
			{
				double pixel = PixelAt(temp.data(), w, h, c, r + rd);
				double weight = kernel[rd + radius];
				intensity += pixel * weight;
			}

			image[c + r * w] = intensity;
		}
	}
}


/*******************************************************************************
Detect Harris corners.
    image - input image
    sigma - standard deviation of Gaussian used to blur corner detector
    thres - Threshold for detecting corners
    interestPts - returned interest points
    numInterestsPts - number of interest points returned
    imageDisplay - image returned to display (for debugging)
*******************************************************************************/
void MainWindow::HarrisCornerDetector(QImage image, double sigma, double thres, CIntPt **interestPts, int &numInterestsPts, QImage &imageDisplay)
{
	const int kWindowSize = 5;
    int r, c;
    int w = image.width();
    int h = image.height();
    double *buffer = new double [w*h];
    QRgb pixel;

    numInterestsPts = 0;

    // Compute the corner response graytone image
    for(r=0;r<h;r++)
       for(c=0;c<w;c++)
        {
            pixel = image.pixel(c, r);

            buffer[r*w + c] = 0.21 * (double)qRed(pixel) + 0.72 * (double)qGreen(pixel) + 0.07 * (double)qBlue(pixel);
        }

    // Write your Harris corner detection code here.

	// Compute image derivatives
	std::vector<double> derivX(buffer, buffer + w * h);
	std::vector<double> derivY(buffer, buffer + w * h);

	std::vector<double> xKernel{
		/*-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1,*/
		-0, 0, 0,
		-1, 0, 1,
		0, 0, 0,
	};
	ConvolveImage(derivX.data(), w, h, xKernel.data(), 1);
	std::vector<double> yKernel{
	/*	-1, -2, -1,
		0, 0, 0,
		1, 2, 1,*/
		0, -1, 0,
		0, 0, 0,
		0, 1, 0,
	};
	ConvolveImage(derivY.data(), w, h, yKernel.data(), 1);

	std::vector<double> ix(derivX.begin(), derivX.end());
	std::vector<double> iy(derivY.begin(), derivY.end());
	SeparableGaussianBlurImage(ix.data(), w, h, sigma);
	SeparableGaussianBlurImage(iy.data(), w, h, sigma);

	std::vector<double> ix2(w * h);
	std::vector<double> iy2(w * h);
	std::vector<double> ixy(w * h);
	MultiplyImages(ix.data(), ix.data(), w, h, ix2.data());
	MultiplyImages(iy.data(), iy.data(), w, h, iy2.data());
	MultiplyImages(ix.data(), iy.data(), w, h, ixy.data());
	SeparableGaussianBlurImage(ixy.data(), w, h, sigma);

	std::vector<double> harrisImage(w * h, 0);
	ComputeResponse(ix2.data(), iy2.data(), ixy.data(), w, h, kWindowSize, sigma, thres, harrisImage.data());

	//BufferToImage(harrisImage.data(), &imageDisplay);
	FindInterestPoints(harrisImage.data(), w, h, interestPts, numInterestsPts);

    // Once you uknow the number of interest points allocate an array as follows:
    // *interestPts = new CIntPt [numInterestsPts];
    // Access the values using: (*interestPts)[i].m_X = 5.0;
    //
    // The position of the interest point is (m_X, m_Y)
    // The descriptor of the interest point is stored in m_Desc
    // The length of the descriptor is m_DescSize, if m_DescSize = 0, then it is not valid.

    // Once you are done finding the interest points, display them on the image
    DrawInterestPoints(*interestPts, numInterestsPts, imageDisplay);

    delete [] buffer;
}


/*******************************************************************************
Find matching interest points between images.
    image1 - first input image
    interestPts1 - interest points corresponding to image 1
    numInterestsPts1 - number of interest points in image 1
    image2 - second input image
    interestPts2 - interest points corresponding to image 2
    numInterestsPts2 - number of interest points in image 2
    matches - set of matching points to be returned
    numMatches - number of matching points returned
    image1Display - image used to display matches
    image2Display - image used to display matches
*******************************************************************************/
void MainWindow::MatchInterestPoints(QImage image1, CIntPt *interestPts1, int numInterestsPts1,
                             QImage image2, CIntPt *interestPts2, int numInterestsPts2,
                             CMatches **matches, int &numMatches, QImage &image1Display, QImage &image2Display)
{
    numMatches = 0;

    // Compute the descriptors for each interest point.
    // You can access the descriptor for each interest point using interestPts1[i].m_Desc[j].
    // If interestPts1[i].m_DescSize = 0, it was not able to compute a descriptor for that point
    ComputeDescriptors(image1, interestPts1, numInterestsPts1);
    ComputeDescriptors(image2, interestPts2, numInterestsPts2);

	std::vector<CMatches> tempMatches;
    // Add your code here for finding the best matches for each point.
	for (int i = 0; i < numInterestsPts1; ++i)
	{
		CIntPt point1 = interestPts1[i];
		if (point1.m_DescSize == 0)
			continue;

		int closestIndex = -1;
		double minDist = std::numeric_limits<double>::max();
		for (int j = 0; j < numInterestsPts2; ++j)
		{
			CIntPt point2 = interestPts2[j];
			if (point2.m_DescSize == 0)
				continue;

			double dist = NormDistanceL1(point1, point2);
			if (dist < minDist)
			{
				closestIndex = j;
				minDist = dist;
			}
		}

		if (closestIndex != -1)
		{
			// add match
			tempMatches.push_back(CMatches{
				point1.m_X, point1.m_Y,
				interestPts2[closestIndex].m_X, interestPts2[closestIndex].m_Y
			});
		}
	}

    // Once you uknow the number of matches allocate an array as follows:
    // *matches = new CMatches [numMatches];
    //
    // The position of the interest point in iamge 1 is (m_X1, m_Y1)
    // The position of the interest point in image 2 is (m_X2, m_Y2)
	numMatches = tempMatches.size();
	*matches = new CMatches[numMatches];
	std::copy(tempMatches.begin(), tempMatches.end(), *matches);

    // Draw the matches
    DrawMatches(*matches, numMatches, image1Display, image2Display);
}

/*******************************************************************************
Project a point (x1, y1) using the homography transformation h
    (x1, y1) - input point
    (x2, y2) - returned point
    h - input homography used to project point
*******************************************************************************/
void MainWindow::Project(double x1, double y1, double &x2, double &y2, double h[3][3])
{
    // Add your code here.
	x2 = h[0][0] * x1 + h[0][1] * y1 + h[0][2];
	y2 = h[1][0] * x1 + h[1][1] * y1 + h[1][2];
	double w = h[2][0] * x1 + h[2][1] * y1 + h[2][2];
	x2 /= w;
	y2 /= w;
}

/*******************************************************************************
Count the number of inliers given a homography.  This is a helper function for RANSAC.
    h - input homography used to project points (image1 -> image2
    matches - array of matching points
    numMatches - number of matchs in the array
    inlierThreshold - maximum distance between points that are considered to be inliers

    Returns the total number of inliers.
*******************************************************************************/
int MainWindow::ComputeInlierCount(double h[3][3], CMatches *matches, int numMatches, double inlierThreshold)
{
	int inlierCount = 0;
    // Add your code here.
	for (int i = 0; i < numMatches; ++i)
	{
		CMatches match = matches[i];

		double projectedX, projectedY;
		Project(match.m_X1, match.m_Y1, projectedX, projectedY, h);

		double xDist = projectedX - match.m_X2;
		double yDist = projectedY - match.m_Y2;
		double dist = sqrt(xDist * xDist + yDist * yDist);

		if (dist < inlierThreshold)
			++inlierCount;
	}

    return inlierCount;
}


/*******************************************************************************
Compute homography transformation between images using RANSAC.
    matches - set of matching points between images
    numMatches - number of matching points
    numIterations - number of iterations to run RANSAC
    inlierThreshold - maximum distance between points that are considered to be inliers
    hom - returned homography transformation (image1 -> image2)
    homInv - returned inverse homography transformation (image2 -> image1)
    image1Display - image used to display matches
    image2Display - image used to display matches
*******************************************************************************/
void MainWindow::RANSAC(CMatches *matches, int numMatches, int numIterations, double inlierThreshold,
                        double hom[3][3], double homInv[3][3], QImage &image1Display, QImage &image2Display)
{
	double bestHom[3][3];
	int maxInlierCount = INT_MIN;

    // Add your code here.
	for (int i = 0; i < numIterations; ++i)
	{
		std::unordered_set<int> chosenIndices;
		CMatches randomMatches[4];
		while (chosenIndices.size() < 4)
		{
			int randIndex = qrand() % numMatches;
			if (chosenIndices.find(randIndex) == chosenIndices.end())
			{
				randomMatches[chosenIndices.size()] = matches[randIndex];
				chosenIndices.insert(randIndex);
			}
		}

		double tempHom[3][3];
		ComputeHomography(randomMatches, 4, tempHom, true);
		int inlierCount = ComputeInlierCount(tempHom, matches, numMatches, inlierThreshold);

		if (inlierCount > maxInlierCount)
		{
			memcpy(bestHom, tempHom, sizeof(double) * 9);
			maxInlierCount = inlierCount;
		}
	}

	std::vector<CMatches> inliers;
	// Add your code here.
	for (int i = 0; i < numMatches; ++i)
	{
		CMatches match = matches[i];

		double projectedX, projectedY;
		Project(match.m_X1, match.m_Y1, projectedX, projectedY, bestHom);

		double xDist = projectedX - match.m_X2;
		double yDist = projectedY - match.m_Y2;
		double dist = sqrt(xDist * xDist + yDist * yDist);

		if (dist < inlierThreshold)
			inliers.push_back(match);
	}

	ComputeHomography(inliers.data(), inliers.size(), hom, true);
	ComputeHomography(inliers.data(), inliers.size(), homInv, false);

    // After you're done computing the inliers, display the corresponding matches.
    DrawMatches(inliers.data(), inliers.size(), image1Display, image2Display);

}

/*******************************************************************************
Bilinearly interpolate image (helper function for Stitch)
    image - input image
    (x, y) - location to interpolate
    rgb - returned color values

    You can just copy code from previous assignment.
*******************************************************************************/
bool MainWindow::BilinearInterpolation(QImage *image, double x, double y, double rgb[3])
{
    // Add your code here.
	int h = image->height();
	int w = image->width();

	int x1 = (int)floor(x);
	int y1 = (int)floor(y);
	int x2 = (int)ceil(x + 0.00001);
	int y2 = (int)ceil(y + 0.00001);

	QRgb p11 = PixelAt(*image, x1, y1);
	QRgb p12 = PixelAt(*image, x1, y2);
	QRgb p21 = PixelAt(*image, x2, y1);
	QRgb p22 = PixelAt(*image, x2, y2);

	for (int i = 0; i < 3; ++i)
	{
		int(*color)(QRgb) = (i == 0 ? qRed : (i == 1 ? qGreen : qBlue));
		rgb[i] = (1 / ((x2 - x1) * (y2 - y1))) *
			(((*color)(p11)* (x2 - x) * (y2 - y)) +
			((*color)(p21)* (x - x1) * (y2 - y)) +
			((*color)(p12)* (x2 - x) * (y - y1)) +
			((*color)(p22)* (x - x1) * (y - y1)));
	}

    return true;
}


/*******************************************************************************
Stitch together two images using the homography transformation
    image1 - first input image
    image2 - second input image
    hom - homography transformation (image1 -> image2)
    homInv - inverse homography transformation (image2 -> image1)
    stitchedImage - returned stitched image
*******************************************************************************/
void MainWindow::Stitch(QImage image1, QImage image2, double hom[3][3], double homInv[3][3], QImage &stitchedImage)
{
    // Width and height of stitchedImage
    int ws = 0;
    int hs = 0;

    // Add your code to compute ws and hs here.
	double topLeft[2];
	double topRight[2];
	double bottomLeft[2];
	double bottomRight[2];
	Project(0, 0, topLeft[0], topLeft[1], homInv);
	Project(image2.width(), 0, topRight[0], topRight[1], homInv);
	Project(0, image2.height(), bottomLeft[0], bottomLeft[1], homInv);
	Project(image2.width(), image2.height(), bottomRight[0], bottomRight[1], homInv);
	
	double left = min(min(min(min(topLeft[0], bottomLeft[0]), topRight[0]), bottomRight[0]), 0.0);
	double right = max(max(max(max(topRight[0], bottomRight[0]), bottomLeft[0]), bottomRight[0]), (double)image1.width());
	double top = min(min(min(min(topLeft[1], topRight[1]), bottomRight[1]), bottomLeft[1]), 0.);
	double bottom = max(max(max(max(bottomLeft[1], bottomRight[1]), topRight[1]), bottomRight[1]), (double)image1.height());
	
	ws = ceil(right - left);
	hs = ceil(bottom - top);

    stitchedImage = QImage(ws, hs, QImage::Format_RGB32);
    stitchedImage.fill(qRgb(0,0,0));
	
	for (int r = 0; r < image1.height(); ++r)
	{
		for (int c = 0; c < image1.width(); ++c)
		{
			stitchedImage.setPixel(c - left, r - top, image1.pixel(c, r));
		}
	}

	for (int r = 0; r < hs; ++r)
	{
		for (int c = 0; c < ws; ++c)
		{
			double pX = 0;
			double pY = 0;
			Project(c + left, r + top, pX, pY, hom);

			if (pX >= 0 && pY >= 0 && pX < image2.width() && pY < image2.height())
			{
				double rgb[3];
				BilinearInterpolation(&image2, pX, pY, rgb);
				stitchedImage.setPixel(c, r, qRgb((int)floor(rgb[0] + 0.5), (int)floor(rgb[1] + 0.5), (int)floor(rgb[2] + 0.5)));
			}
		}
	}

    // Add you code to warp image1 and image2 to stitchedImage here.
}

