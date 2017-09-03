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
	return Point2f(intervalToTent(sample[0]), intervalToTent(sample[1]));
}

float Warp::squareToTentPdf(const Point2f &p) {
	float x = abs(p[0]);
	float y = abs(p[1]);
	return 1.0f - x - y + x*y;
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
	float r = sqrtf(sample[0]);
	float theta = 2.0f * M_PI * sample[1];
	return Point2f(r*cosf(theta), r*sinf(theta));
	//return Point2f(2.0f * sample[0] - 1.0f, 2.0f * sample[1] - 1.0f);
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
	return ((p[0]* p[0] + p[1] * p[1]) < 1.0f) ? INV_PI : 0.0f;
}

// CDF(theta)=theta/(2*pi)
// CDF(phi)=(cos(phi)+1)/2
// dA=sin(phi) dphi dtheta
Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
    float theta = 2.0f*M_PI*sample[0];
    float phi = acosf(2.0f*sample[1]-1.0f);
    return Point3f(sinf(phi)*cosf(theta), cosf(phi), sinf(phi)*sinf(theta));
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
    // return (fabs((v[0]*v[0]+v[1]*v[1]+v[2]*v[2])-1.0f)<1e-5)?INV_FOURPI:0.0f;
    return INV_FOURPI;
}

// oriented in direction (0,0,1)
Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
    float theta = 2.0f*M_PI*sample[0];
    float phi = acosf(sample[1]);
    return Point3f(sinf(phi)*cosf(theta), sinf(phi)*sinf(theta), cosf(phi));
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
    return (v[2]>0.0f)?INV_TWOPI:0.0f;
}

Point2f squareToUniformDiskConcentric(const Point2f &sample) {
    float r1 = 2.0f*sample.x() - 1.0f;
    float r2 = 2.0f*sample.y() - 1.0f;

    /* Modified concencric map code with less branching (by Dave Cline), see
       http://psgraphics.blogspot.ch/2011/01/improved-code-for-concentric-map.html */
    float phi, r;
    if (r1 == 0 && r2 == 0) {
        r = phi = 0;
    } else if (r1*r1 > r2*r2) {
        r = r1;
        phi = (M_PI/4.0f) * (r2/r1);
    } else {
        r = r2;
        phi = (M_PI/2.0f) - (r1/r2) * (M_PI/4.0f);
    }

    float cosPhi=cosf(phi);
    float sinPhi=sinf(phi);
    // math::sincos(phi, &sinPhi, &cosPhi);

    return Point2f(r * cosPhi, r * sinPhi);
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
    float theta = 2.0f*M_PI*sample[0];
    float phi = asinf(M_PI*sample[1]);
    return Point3f(sinf(phi)*cosf(theta), sinf(phi)*sinf(theta), cosf(phi));
    // Point2f p = squareToUniformDiskConcentric(sample);
    // float z = sqrtf(1.0f - p.x()*p.x() - p.y()*p.y());

    // /* Guard against numerical imprecisions */
    // if (fabs(z) < 1e-5)
    //     z = 1e-10f;

    // return Vector3f(p.x(), p.y(), z);
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
    return (v[2]>0.0f)?(v[2]*INV_PI):0.0f;
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
    throw NoriException("Warp::squareToBeckmann() is not yet implemented!");
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
    throw NoriException("Warp::squareToBeckmannPdf() is not yet implemented!");
}

NORI_NAMESPACE_END
