// 
// impressionistDoc.cpp
//
// It basically maintain the Image for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>
#include <cmath>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "lineBrush.h"
#include "circleBrush.h"
#include "scatteredPointsBrush.h"
#include "scatteredLinesBrush.h"
#include "scatteredCirclesBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucImage		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointsBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLinesBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCirclesBrush( this, "Scattered Circles" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the line width of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getLineWidth()
{
	return m_pUI->getLineWidth();
}

//---------------------------------------------------------
// Returns the line angle of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}


//---------------------------------------------------------
// Returns the alpha value of the brush.
//---------------------------------------------------------
double ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(const char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=load(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load image file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	delete [] m_ucImage;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;

	m_ucImage		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(const char *iname, const char * type, int quality) 
{

	save(iname, m_ucPainting, m_nPaintWidth, m_nPaintHeight, type, quality);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resultingknlRow
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	// Loop through every coordinate on the buffer
	for (int imgRow = 0; imgRow < srcBufferHeight; imgRow++) {
		for (int imgCol = 0; imgCol < srcBufferWidth; imgCol++) {
			// Modifying the pixel at sourceBuffer[3*(row*srcBufferWidth+col) + (0/1/2)]
			int R, G, B;
			R = G = B = 0;

			for (int knlRow = 0; knlRow < knlHeight; knlRow++) {
				for (int knlCol = 0; knlCol < knlWidth; knlCol++) {
					int row = imgRow + (knlRow - knlHeight/2);
					int col = imgCol + (knlCol - knlWidth/2);
					double factor = filterKernel[knlRow * knlWidth + knlCol];
					// If the pixel is off the boundary, set it to be the closest pixel on the boundary.
					if (row < 0) row = 0;
					if (col < 0) col = 0;
					if (row >= srcBufferHeight) row = srcBufferHeight - 1;
					if (col >= srcBufferWidth) col = srcBufferWidth - 1;
					// Add the RGB values of the pixel
					R += sourceBuffer[3 * (row*srcBufferWidth + col) + 0] * factor;
					G += sourceBuffer[3 * (row*srcBufferWidth + col) + 1] * factor;
					B += sourceBuffer[3 * (row*srcBufferWidth + col) + 2] * factor;
				}
			}
			// Set the pixel at (imgRow, imgCol) to (summed_value / divisior) + offset
			destBuffer[3 * (imgRow*srcBufferWidth + imgCol) + 0] = adjustColor((R / divisor) + offset);
			destBuffer[3 * (imgRow*srcBufferWidth + imgCol) + 1] = adjustColor((G / divisor) + offset);
			destBuffer[3 * (imgRow*srcBufferWidth + imgCol) + 2] = adjustColor((B / divisor) + offset);
		}
	}
}

//------------------------------------------------------------------
// Adjust the color value so it lies within (0, 255)
//------------------------------------------------------------------
int ImpressionistDoc::adjustColor(int value) {
	if (value < 0) return 0;
	if (value > 255) return 255;
	return value;
}


//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucImage + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const ImpBrush::Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

//----------------------------------------------------------------
// Get the angle in degrees of the gradient at the given point p.
//----------------------------------------------------------------
GLint ImpressionistDoc::GetGradientAngle(const ImpBrush::Point p)
{
	const double sobelX[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	const double sobelY[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

	double xRes = 0;
	double yRes = 0;

	for (int imgRow = 0; imgRow < 3; ++imgRow)
	{
		for (int imgCol = 0; imgCol < 3; ++imgCol)
		{
			GLubyte pix[3];
			memcpy(pix, GetOriginalPixel(p.x + imgCol, p.y + imgRow), 3);
			double avg = ((double)pix[0] + pix[1] + pix[2]) / 3.0; // Convert to grayscale
			xRes += avg * sobelX[3 * imgRow + imgCol];
			yRes += avg * sobelY[3 * imgRow + imgCol];
		}
	}

	// Calculate angle of gradient
	double res = atan2Degrees(yRes, xRes);
	return res;
}

double atan2Degrees(double y, double x)
{
	return atan2(y, x) * (180 / M_PI);
}
