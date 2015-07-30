//
// scatteredCirclesBrush.cpp
//
// The implementation of Line Brush.
//

#include <cmath>
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredCirclesBrush.h"

extern float frand();

ScatteredCirclesBrush::ScatteredCirclesBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredCirclesBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	glEnable(GL_BLEND);
	BrushMove(source, target);
}

void ScatteredCirclesBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCirclesBrush::BrushMove  document is NULL\n");
		return;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	GLfloat size = (GLfloat)pDoc->getSize();

	for (int i = 0; i < 4; ++i)
	{

		ImpBrush::Point newOffset(size * (frand() - 0.5f), size * (frand() - 0.5f));
		ImpBrush::Point newSource(source.x + newOffset.x, source.y + newOffset.y);
		ImpBrush::Point newTarget(target.x + newOffset.x, target.y + newOffset.y);

		glBegin(GL_POLYGON);
			SetColor(newSource);
			const int numLines = 128;
			float size = (float)pDoc->getSize() / 2.f;
			double angle;
			for (int i = 0; i < numLines; ++i)
			{
				angle = i * 2 * M_PI / numLines;
				glVertex2f(newTarget.x + size * cos(angle), newTarget.y + size * sin(angle));
			}
		glEnd();
	}
}

void ScatteredCirclesBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

