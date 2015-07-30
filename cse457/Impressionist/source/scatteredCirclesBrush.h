//
// scatteredCirclesBrush.h
//
// The header file for Scattered Circles Brush. 
//

#ifndef SCATTEREDCIRCLESBRUSH_H
#define SCATTEREDCIRCLESBRUSH_H

#include "impBrush.h"

class ScatteredCirclesBrush : public ImpBrush
{
public:
	ScatteredCirclesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushMove(const ImpBrush::Point source, const ImpBrush::Point target);
	void BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target);
	char* BrushName(void);
};

#endif
