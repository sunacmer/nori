#include <numeric>
#include <nori/octree.h>

NORI_NAMESPACE_BEGIN

OctNode::OctNode(const BoundingBox3f& box)
{
	bbox = box;
	for (int i = 0; i < 8; ++i) children[i] = nullptr;
}

OctNode::~OctNode()
{
	triangles.swap(vector<uint32_t>());
}

bool OctNode::isleaf() const
{
	return (children[0] == nullptr);
}

Octree::Octree()
{
	root = new OctNode(m_bbox);
}

Octree::~Octree()
{
	Destroy(root);
}

void Octree::build()
{
	root->triangles.resize(m_mesh->getTriangleCount());
	iota(root->triangles.begin(), root->triangles.end(), 0);
	buildImpl(root);
	cout << "octree build finish" << endl;
}

void Octree::buildImpl(OctNode* node)
{
	if (node == nullptr) return;
	if (node->triangles.size() < 10) return;

	Point3f min = node->bbox.min;
	Point3f max = node->bbox.max;
	Point3f center = node->bbox.getCenter();
	// 前半部分
	node->children[0] = new OctNode(BoundingBox3f(min, center));
	node->children[1] = new OctNode(BoundingBox3f(Point3f(center.x(), min.y(), min.z()), Point3f(max.x(), center.y(), center.z())));
	node->children[2] = new OctNode(BoundingBox3f(Point3f(center.x(), center.y(), min.z()), Point3f(max.x(), max.y(), center.z())));
	node->children[3] = new OctNode(BoundingBox3f(Point3f(min.x(), center.y(), min.z()), Point3f(center.x(), max.y(), center.z())));
	// 后半部分
	node->children[4] = new OctNode(BoundingBox3f(Point3f(min.x(), min.y(), center.z()), Point3f(center.x(), center.y(), max.z())));
	node->children[5] = new OctNode(BoundingBox3f(Point3f(center.x(), min.y(), center.z()), Point3f(max.x(), center.y(), max.z())));
	node->children[6] = new OctNode(BoundingBox3f(Point3f(center.x(), center.y(), center.z()), Point3f(max.x(), max.y(), max.z())));
	node->children[7] = new OctNode(BoundingBox3f(Point3f(min.x(), center.y(), center.z()), Point3f(center.x(), max.y(), max.z())));

	for (uint32_t i = 0; i < node->triangles.size(); ++i)
	{
		const BoundingBox3f box = m_mesh->getBoundingBox(node->triangles[i]);
		for (int j = 0; j < 8; ++j)
		{
			if (node->children[j]->bbox.overlaps(box))
			{
				node->children[j]->triangles.push_back(i); continue;// TODO overlaps?
			}
		}
	}

	for (int i = 0; i < 8; ++i) buildImpl(node->children[i]);
}

void Octree::Destroy(OctNode* node)
{
	if (node == nullptr) return;
	for (int i = 0; i < 8; ++i) Destroy(node->children[i]);
	delete node;
}

bool Octree::rayIntersect(const Ray3f &ray, Intersection &its, bool shadowRay) const
{
	if (intersect(root, ray, its, shadowRay))
	{
		return true;
	}
	return false;// TODO intersection info
}

bool Octree::intersect(OctNode* node, const Ray3f &ray_, Intersection &its, bool shadowRay) const
{
	if (!node->bbox.rayIntersect(ray_)) return;
	if (node->isleaf())
	{
		Ray3f ray(ray_);
		bool flag = false;
		for (uint32_t i = 0; i < node->triangles.size(); ++i)
		{
			float u, v, t;
			uint32_t idx = node->triangles[i];
			if (m_mesh->rayIntersect(idx, ray, u, v, t))
			{
				if (shadowRay) return true;
				ray.maxt = its.t = t;
				its.uv = Point2f(u, v);
				its.mesh = m_mesh;
				its.idx = idx;
				flag = true;
			}
		}
		return flag;
	}
	for (int i = 0; i < 8; ++i)
	{
		intersect(node->children[i], ray_, its, shadowRay);
	}
}

NORI_NAMESPACE_END