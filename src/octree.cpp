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
	root->bbox = m_bbox;
	root->triangles.resize(m_mesh->getTriangleCount());
	iota(root->triangles.begin(), root->triangles.end(), 0);
	maxdepth = ceil(logf(m_mesh->getTriangleCount()) / logf(8.0f));
	buildImpl(root, 0);
}

void Octree::buildImpl(OctNode* node, int depth)
{
	if (node == nullptr) return;
	if (depth > maxdepth) return;
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

	// 同一个三角形被分在多个节点中
	// accel中的brute同一条ray会多余计算多个三角形 同一个三角形会被不同ray计算
	for (uint32_t i = 0; i < node->triangles.size(); ++i)
	{
		const BoundingBox3f box = m_mesh->getBoundingBox(node->triangles[i]);
		for (int j = 0; j < 8; ++j)
		{
			if (node->children[j]->bbox.overlaps(box))
			{
				node->children[j]->triangles.push_back(node->triangles[i]); // 和多个node交叉的三角形只算到一个node里会有漏一些三角形
			}
		}
	}

	for (int i = 0; i < 8; ++i) buildImpl(node->children[i], depth+1);
}

void Octree::Destroy(OctNode* node)
{
	if (node == nullptr) return;
	for (int i = 0; i < 8; ++i) Destroy(node->children[i]);
	delete node;
}

bool Octree::rayIntersect(const Ray3f &ray, Intersection &its, bool shadowRay) const
{
	intersect(root, Ray3f(ray), its, shadowRay);
	return (its.idx < 0xffffffff);
}

// 带返回值的递归函数
void Octree::intersect(OctNode* node, Ray3f &ray, Intersection &its, bool shadowRay) const
{
	if (!node->bbox.rayIntersect(ray)) return;
	if (node->isleaf())
	{
		bool flag = false;
		for (uint32_t i = 0; i < node->triangles.size(); ++i)
		{
			float u, v, t;
			uint32_t idx = node->triangles[i];
			if (m_mesh->rayIntersect(idx, ray, u, v, t))
			{
				ray.maxt = its.t = t;
				its.uv = Point2f(u, v);
				its.mesh = m_mesh;
				its.idx = idx;
				flag = true;
				if (shadowRay) return;// 用its.idx < 0xffffffff判断是否相交 所以把这句从原代码中移下来
			}
		}
		if (flag) CalcIntersectionInfo(its);// 访问its的数据ajax慢10ms??
	}
	else
	{
		// TODO 貌似从大到小排列更快 对节点中的三角形按照距离排序优化时间
		for (int i = 0; i < 8; ++i)
		{
			intersect(node->children[i], ray, its, shadowRay);
		}
	}
}

NORI_NAMESPACE_END