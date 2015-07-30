#include "BSplineCurveEvaluator.h"
#include "BezierCurveEvaluator.h"
#include <cassert>
using namespace Animator;

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap,
	const float default_val) const
{
	ptvEvaluatedCurvePts.clear();

	BezierCurveEvaluator bce;

	// New vector with endpoints doubled
	std::vector<Point> ptvCtrlPtsCopy = ptvCtrlPts;

	if (!bWrap) {
		ptvCtrlPtsCopy.insert(ptvCtrlPtsCopy.begin(), ptvCtrlPtsCopy[0]);
		ptvCtrlPtsCopy.insert(ptvCtrlPtsCopy.begin(), ptvCtrlPtsCopy[0]);
		ptvCtrlPtsCopy.push_back(ptvCtrlPtsCopy[ptvCtrlPtsCopy.size() - 1]);
		ptvCtrlPtsCopy.push_back(ptvCtrlPtsCopy[ptvCtrlPtsCopy.size() - 1]);
	}
	else {
		ptvCtrlPtsCopy.insert(ptvCtrlPtsCopy.begin(), Point(
			ptvCtrlPtsCopy[ptvCtrlPtsCopy.size() - 1].x - fAniLength,
			ptvCtrlPtsCopy[ptvCtrlPtsCopy.size() - 1].y
			));
		ptvCtrlPtsCopy.insert(ptvCtrlPtsCopy.begin(), Point(
			ptvCtrlPtsCopy[ptvCtrlPtsCopy.size() - 2].x - fAniLength,
			ptvCtrlPtsCopy[ptvCtrlPtsCopy.size() - 2].y
			));
		ptvCtrlPtsCopy.push_back(Point(
			ptvCtrlPts[0].x + fAniLength,
			ptvCtrlPts[0].y
			));
		ptvCtrlPtsCopy.push_back(Point(
			ptvCtrlPts[1].x + fAniLength,
			ptvCtrlPts[1].y
			));
	}
	int iCtrlPtCount = ptvCtrlPtsCopy.size();

	// Add controls between each pair of points
	int i;
	for (i = 0; i + 3 < iCtrlPtCount; i++) {
		interpolatePoints(ptvCtrlPtsCopy[i + 0], ptvCtrlPtsCopy[i + 1], ptvCtrlPtsCopy[i + 2], ptvCtrlPtsCopy[i + 3], ptvEvaluatedCurvePts, bce);
	}

	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPtsCopy[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPtsCopy[iCtrlPtCount - 1].y));
	}
}

void BSplineCurveEvaluator::interpolatePoints(const Animator::Point& p0, const Animator::Point& p1, const Animator::Point& p2,
					const Animator::Point& p3, std::vector<Animator::Point>& vec, BezierCurveEvaluator& bce) const {

	double v0x = 1.0 / 6.0 * p0.x + 2.0 / 3.0 * p1.x + 1.0 / 6.0 * p2.x;
	double v0y = 1.0 / 6.0 * p0.y + 2.0 / 3.0 * p1.y + 1.0 / 6.0 * p2.y;
	Point v0 = Point(v0x, v0y);

	double v1x = 2.0 / 3.0 * p1.x + 1.0 / 3.0 * p2.x;
	double v1y = 2.0 / 3.0 * p1.y + 1.0 / 3.0 * p2.y;
	Point v1 = Point(v1x, v1y);

	double v2x = 1.0 / 3.0 * p1.x + 2.0 / 3.0 * p2.x;
	double v2y = 1.0 / 3.0 * p1.y + 2.0 / 3.0 * p2.y;
	Point v2 = Point(v2x, v2y);

	double v3x = 1.0 / 6.0 * p1.x + 2.0 / 3.0 * p2.x + 1.0 / 6.0 * p3.x;
	double v3y = 1.0 / 6.0 * p1.y + 2.0 / 3.0 * p2.y + 1.0 / 6.0 * p3.y;
	Point v3 = Point(v3x, v3y);

	bce.evaluateCubicBezier(v0, v1, v2, v3, vec);
}