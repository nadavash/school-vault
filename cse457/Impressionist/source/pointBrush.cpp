//
// pointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointBrush.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();
	double alpha = pDoc->getAlpha();

	glPointSize( (float)size );
	glEnable(GL_BLEND);

	BrushMove( source, target );
}

void PointBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin( GL_POINTS );
		SetColor( source );

		glVertex2d( target.x, target.y );

	glEnd();
}

void PointBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
	glDisable(GL_BLEND);
}

