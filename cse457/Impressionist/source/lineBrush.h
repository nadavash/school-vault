//
// lineBrush.h
//
// The header file for Line Brush. 
//

#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#include "impBrush.h"

class LineBrush : public ImpBrush
{
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushMove(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target);
	char* BrushName(void);

private:
	ImpBrush::Point m_prev;
};

#endif
