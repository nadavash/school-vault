//
// scatteredLinesBrush.h
//
// The header file for Scattered Lines Brush. 
//

#ifndef SCATTEREDLINESBRUSH_H
#define SCATTEREDLINESBRUSH_H

#include "impBrush.h"

class ScatteredLinesBrush : public ImpBrush
{
public:
	ScatteredLinesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushMove(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target);
	char* BrushName(void);

private:
	ImpBrush::Point m_prev;
};

#endif
