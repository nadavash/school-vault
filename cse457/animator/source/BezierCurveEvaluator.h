#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H 

#pragma once
#include "curveevaluator.h"
class BezierCurveEvaluator :
	public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Animator::Point>& ptvCtrlPts,
		std::vector<Animator::Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap,
		const float default_val) const;

	void evaluateCubicBezier(const Animator::Point& v1, const Animator::Point& v2, const Animator::Point& v3, const Animator::Point& v4, std::vector<Animator::Point>& vec) const;

private:
	Animator::Point evaluatePolynomial(double u, const Animator::Point& v1, const Animator::Point& v2, const Animator::Point& v3, const Animator::Point& v4) const;
};

#endif