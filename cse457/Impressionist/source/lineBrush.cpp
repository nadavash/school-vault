//
// lineBrush.cpp
//
// The implementation of Line Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "lineBrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
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

void LineBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(target.x, target.y, 0);

	switch (dlg->getBrushDirection())
	{
	case BRUSH_DIRECTION_GRADIENT:
		glRotatef(pDoc->GetGradientAngle(source), 0, 0, 1);
		break;
	case BRUSH_DIRECTION_STROKE:
		glRotatef(atan2Degrees(target.y - m_prev.y, target.x - m_prev.x), 0, 0, 1);
		m_prev = target;
		break;
	case BRUSH_DIRECTION_LINE:
	default:
		glRotatef(pDoc->getLineAngle(), 0, 0, 1);
		break;
	}

	glBegin(GL_LINES);
		SetColor(source);
		GLfloat size = pDoc->getSize();
		glVertex2f(-size / 2.f, 0);
		glVertex2f(size / 2.f, 0);
	glEnd();

	glPopMatrix();
}

void LineBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

