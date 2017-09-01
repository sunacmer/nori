/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <nori/warp.h>
#include <nori/vector.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

Point2f Warp::squareToUniformSquare(const Point2f &sample) {
    return sample;
}

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
    return ((sample.array() >= 0).all() && (sample.array() <= 1).all()) ? 1.0f : 0.0f;
}

float Warp::intervalToTent(float sample) {
	float sign;

	if (sample < 0.5f) {
		sign = 1;
		sample *= 2;
	}
	else {
		sign = -1;
		sample = 2 * (sample - 0.5f);
	}

	return sign * (1 - std::sqrt(sample));
}

Point2f Warp::squareToTent(const Point2f &sample) {
    //throw NoriException("Warp::squareToTent() is not yet implemented!");
	return Point2f(intervalToTent(sample[0]), intervalToTent(sample[1]));
}

float Warp::squareToTentPdf(const Point2f &p) {
    //throw NoriException("Warp::squareToTentPdf() is not yet implemented!");
	float x = abs(p[0]);
	float y = abs(p[1]);
	return 1.0f - x - y + x*y;
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
    //throw NoriException("Warp::squareToUniformDisk() is not yet implemented!");
	float r = sqrtf(sample[0]);
	float theta = 2.0f * M_PI * sample[1];
	return Point2f(r*cosf(theta), r*sinf(theta));
	//return Point2f(2.0f * sample[0] - 1.0f, 2.0f * sample[1] - 1.0f);
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
    //throw NoriException("Warp::squareToUniformDiskPdf() is not yet implemented!");
	return ((p[0]* p[0] + p[1] * p[1]) < 1.0f) ? INV_PI : 0.0f;
}

Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
    throw NoriException("Warp::squareToUniformSphere() is not yet implemented!");
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
    throw NoriException("Warp::squareToUniformSpherePdf() is not yet implemented!");
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
    throw NoriException("Warp::squareToUniformHemisphere() is not yet implemented!");
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
    throw NoriException("Warp::squareToUniformHemispherePdf() is not yet implemented!");
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
    throw NoriException("Warp::squareToCosineHemisphere() is not yet implemented!");
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
    throw NoriException("Warp::squareToCosineHemispherePdf() is not yet implemented!");
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
    throw NoriException("Warp::squareToBeckmann() is not yet implemented!");
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
    throw NoriException("Warp::squareToBeckmannPdf() is not yet implemented!");
}

NORI_NAMESPACE_END
