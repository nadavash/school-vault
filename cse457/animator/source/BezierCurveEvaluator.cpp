#include "BezierCurveEvaluator.h"
#include <cassert>
using namespace Animator;

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap,
	const float default_val) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	// ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());

	// add curve pts
	
	int i;
	for (i = 0; i + 3 < iCtrlPtCount; i += 3) {
		evaluateCubicBezier(ptvCtrlPts[0], ptvCtrlPts[1], ptvCtrlPts[2], ptvCtrlPts[3], ptvEvaluatedCurvePts);
	}
	if (!bWrap) {
		// add remaining pts
		for (; i < iCtrlPtCount; i++) {
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
		}

		// add end pts
		ptvEvaluatedCurvePts.push_back(Point(0.0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}
	else {
		if (i + 3 == iCtrlPtCount) {
			// add a curve that connects to beginning
			Point startPoint = ptvCtrlPts[0];
			Point pseudoStartPoint = Point(startPoint.x + fAniLength, startPoint.y);
			std::vector<Animator::Point> wrapPoints;
			evaluateCubicBezier(ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], pseudoStartPoint, wrapPoints);
			for (int j = 0; j < wrapPoints.size(); j++) {
				if (wrapPoints[j].x > fAniLength) {
					wrapPoints[j].x -= fAniLength;
				}
				ptvEvaluatedCurvePts.push_back(wrapPoints[j]);
			}
		}
		else {
			for (; i < iCtrlPtCount; i++) {
				ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
			}
			// add points to connect final and starting points
			double deltaX = fAniLength + ptvCtrlPts[0].x - ptvCtrlPts[iCtrlPtCount - 1].x;
			double ratio = (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) / deltaX;
			double deltaY = ptvCtrlPts[0].y - ptvCtrlPts[iCtrlPtCount - 1].y;
			double yVal = deltaY * ratio + ptvCtrlPts[iCtrlPtCount - 1].y;
			ptvEvaluatedCurvePts.push_back(Point(0.0, yVal));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, yVal));
		}
	}
}

void BezierCurveEvaluator::evaluateCubicBezier(const Animator::Point& v0, const Animator::Point& v1, const Animator::Point& v2, 
												const Animator::Point& v3, std::vector<Animator::Point>& vec) const {
	double timestep = 1.0 / 100;
	double currentTime = v0.x;
	double timeBound = v3.x;
	double timeRange = v3.x - v0.x;
	double prevX = currentTime - 1;
	while (currentTime < timeBound) {
		double relativeTime = currentTime - v0.x; // time within 4 points
		double u = relativeTime / timeRange;
		Point p = evaluatePolynomial(u, v0, v1, v2, v3);
		if (p.x < v3.x && p.x > prevX) {
			vec.push_back(p);
			prevX = p.x;
		}
		currentTime += timestep;
	}
	vec.push_back(v3);
}

Animator::Point BezierCurveEvaluator::evaluatePolynomial(double u, const Animator::Point& v0, const Animator::Point& v1, const Animator::Point& v2, const Animator::Point& v3) const {
	double b0 = pow((1 - u), 3);
	double b1 = 3 * u * pow((1 - u), 2);
	double b2 = 3 * pow(u, 2) * (1 - u);
	double b3 = pow(u, 3);
	double resultX = b0 * v0.x + b1 * v1.x + b2 * v2.x + b3 * v3.x;
	double resultY = b0 * v0.y + b1 * v1.y + b2 * v2.y + b3 * v3.y;
	return Point(resultX, resultY);
}
