//
// circleBrush.cpp
//
// The implementation of Circle Brush.
//
#include <cmath>
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glEnable(GL_BLEND);
	BrushMove(source, target);

}

void CircleBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL)
	{
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const int numLines = 128;

	glBegin(GL_POLYGON);
		SetColor(source);
		float size = (float)pDoc->getSize() / 2.f;
		double angle;
		for (int i = 0; i < numLines; ++i)
		{
			angle = i * 2 * M_PI / numLines;
			glVertex2f(target.x + size * cos(angle), target.y + size * sin(angle));
		}
	glEnd();

}

void CircleBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

