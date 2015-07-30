#include "CatmullRomCurveEvaluator.h"
#include "BezierCurveEvaluator.h"
#include <cassert>
using namespace Animator;

void CatmullRomCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap,
	const float default_val) const
{
	ptvEvaluatedCurvePts.clear();

	// Bezier evaluator for cubic beziers
	BezierCurveEvaluator bce;

	double tension = 0.5;

	// New vector with endpoints doubled
	int iCtrlPtCount = ptvCtrlPts.size();

	// Evaluate curves at ends
	if (!bWrap) {
		// starting curve
		Point s0 = ptvCtrlPts[0];
		Point s1 = ptvCtrlPts[0];
		Point s2 = ptvCtrlPts[1];
		Point s3 = (iCtrlPtCount > 2) ? ptvCtrlPts[2] : ptvCtrlPts[1];
		interpolatePoints(s0, s1, s2, s3, ptvEvaluatedCurvePts, bce, tension);

		// ending curve
		Point e0 = (iCtrlPtCount > 2) ? ptvCtrlPts[iCtrlPtCount - 3] : ptvCtrlPts[iCtrlPtCount - 2];
		Point e1 = ptvCtrlPts[iCtrlPtCount - 2];
		Point e2 = ptvCtrlPts[iCtrlPtCount - 1];
		Point e3 = ptvCtrlPts[iCtrlPtCount - 1];
		interpolatePoints(e0, e1, e2, e3, ptvEvaluatedCurvePts, bce, tension);

		// end points
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}
	else {
		// starting curve
		Point s0 = Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
		Point s1 = ptvCtrlPts[0];
		Point s2 = ptvCtrlPts[1];
		Point s3 = (iCtrlPtCount == 2) ? Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y) : ptvCtrlPts[2];
		interpolatePoints(s0, s1, s2, s3, ptvEvaluatedCurvePts, bce, tension);

		// ending curve
		Point e0 = (iCtrlPtCount == 2) ? Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y) : ptvCtrlPts[iCtrlPtCount - 3];
		Point e1 = ptvCtrlPts[iCtrlPtCount - 2];
		Point e2 = ptvCtrlPts[iCtrlPtCount - 1];
		Point e3 = Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		interpolatePoints(e0, e1, e2, e3, ptvEvaluatedCurvePts, bce, tension);

		// wrapping curve
		std::vector<Animator::Point> wrapPoints;
		Point m0 = ptvCtrlPts[iCtrlPtCount - 2];
		Point m1 = ptvCtrlPts[iCtrlPtCount - 1];
		Point m2 = Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		Point m3 = Point(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
		interpolatePoints(m0, m1, m2, m3, wrapPoints, bce, tension);
		for (int i = 0; i < wrapPoints.size(); i++) {
			if (wrapPoints[i].x > fAniLength)
				wrapPoints[i].x -= fAniLength;
			ptvEvaluatedCurvePts.push_back(wrapPoints[i]);
		}
	}

	// Evaluate curves in middle
	int i;
	for (i = 0; i + 3 < iCtrlPtCount; i++) {
		interpolatePoints(ptvCtrlPts[i + 0], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3], ptvEvaluatedCurvePts, bce, tension);
	}

}

void CatmullRomCurveEvaluator::interpolatePoints(const Animator::Point& p0, const Animator::Point& p1, const Animator::Point& p2, 
					const Animator::Point& p3, std::vector<Animator::Point>& vec, BezierCurveEvaluator& bce, double& tension) const {
	Point w0 = p1;
	double w1x = p1.x + tension / 3 * (p2.x - p0.x);
	double w1y = p1.y + tension / 3 * (p2.y - p0.y);
	Point w1 = Point(w1x, w1y);
	double w2x = p2.x - tension / 3 * (p3.x - p1.x);
	double w2y = p2.y - tension / 3 * (p3.y - p1.y);
	Point w2 = Point(w2x, w2y);
	Point w3 = p2;
	bce.evaluateCubicBezier(w0, w1, w2, w3, vec);
}
