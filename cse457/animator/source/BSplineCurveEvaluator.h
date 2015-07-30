#ifndef INCLUDED_BSPLINE_CURVE_EVALUATOR_H
#define INCLUDED_BSPLINE_CURVE_EVALUATOR_H 

#pragma once
#include "curveevaluator.h"
#include "BezierCurveEvaluator.h"
class BSplineCurveEvaluator :
	public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Animator::Point>& ptvCtrlPts,
		std::vector<Animator::Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap,
		const float default_val) const;

private:
	void interpolatePoints(const Animator::Point& p0, const Animator::Point& p1, const Animator::Point& p2, const Animator::Point& p3,
		std::vector<Animator::Point>& vec, BezierCurveEvaluator& bce) const;
};

#endif