//
// scatteredPointsBrush.h
//
// The header file for Scattered Lines Brush. 
//

#ifndef SCATTEREDPOINTSBRUSH_H
#define SCATTEREDPOINTSBRUSH_H

#include "impBrush.h"

class ScatteredPointsBrush : public ImpBrush
{
public:
	ScatteredPointsBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushMove(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target);
	char* BrushName(void);
};

#endif
