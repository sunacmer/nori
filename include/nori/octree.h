#pragma once

#include <nori/accel.h>

NORI_NAMESPACE_BEGIN

struct OctNode
{
	OctNode(const BoundingBox3f& box);
	~OctNode();

	bool isleaf() const;

	OctNode* children[8];
	BoundingBox3f bbox;
	vector<uint32_t> triangles;
};

class Octree : public Accel
{
public:

	Octree();
	~Octree();

	virtual void build();
	virtual bool rayIntersect(const Ray3f &ray, Intersection &its, bool shadowRay) const;

private:

	int maxdepth;
	OctNode* root;
	void Destroy(OctNode* node);
	void buildImpl(OctNode* node, int depth);
	void intersect(OctNode* node, Ray3f &ray, Intersection &its, bool shadowRay) const;
};

NORI_NAMESPACE_END