//
// scatteredLinesBrush.cpp
//
// The implementation of Line Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredLinesBrush.h"

extern float frand();

ScatteredLinesBrush::ScatteredLinesBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredLinesBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glLineWidth(pDoc->getLineWidth());
	glEnable(GL_BLEND);

	if (pDoc->m_pUI->getBrushDirection() == BRUSH_DIRECTION_STROKE)
	{
		m_prev = target;
	}
	else
		BrushMove(source, target);
}

void ScatteredLinesBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredLinesBrush::BrushMove  document is NULL\n");
		return;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	double strokeAngle = atan2Degrees(target.y - m_prev.y, target.x - m_prev.x);
	m_prev = target;

	GLfloat size = (GLfloat)pDoc->getSize();

	for (int i = 0; i < 4; ++i)
	{
		ImpBrush::Point newOffset(size * (frand() - 0.5f), size * (frand() - 0.5f));
		ImpBrush::Point newSource(source.x + newOffset.x, source.y + newOffset.y);
		ImpBrush::Point newTarget(target.x + newOffset.x, target.y + newOffset.y);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(newTarget.x, newTarget.y, 0);
		switch (dlg->getBrushDirection())
		{
		case BRUSH_DIRECTION_GRADIENT:
			glRotatef(pDoc->GetGradientAngle(newSource), 0, 0, 1);
			break;
		case BRUSH_DIRECTION_STROKE:
			glRotatef(strokeAngle, 0, 0, 1);
			break;
		case BRUSH_DIRECTION_LINE:
		default:
			glRotatef(pDoc->getLineAngle(), 0, 0, 1);
			break;
		}

		glBegin(GL_LINES);
			SetColor(newSource);
			glVertex2d(-size / 2.f, 0);
			glVertex2d(size / 2.f, 0);
		glEnd();

		glPopMatrix();
	}
}

void ScatteredLinesBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

