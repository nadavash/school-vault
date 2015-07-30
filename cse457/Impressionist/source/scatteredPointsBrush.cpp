//
// scatteredPointsBrush.cpp
//
// The implementation of Points Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredPointsBrush.h"

extern float frand();

ScatteredPointsBrush::ScatteredPointsBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredPointsBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glPointSize(1);
	glEnable(GL_BLEND);

	BrushMove(source, target);
}

void ScatteredPointsBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointsBrush::BrushMove  document is NULL\n");
		return;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float size = (float)pDoc->getSize();

	for (int i = 0; i < 10 * (int)size; ++i)
	{
		ImpBrush::Point newTarget(target.x + size * (frand() - 0.5f),
			target.y + size * (frand() - 0.5f));

		glBegin(GL_POINTS);
		SetColor(newTarget);
		glVertex2d(newTarget.x, newTarget.y);
		glEnd();
	}
}

void ScatteredPointsBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

