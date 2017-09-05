#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>

NORI_NAMESPACE_BEGIN

class WhittedIntegrator : public Integrator {
public:
	WhittedIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        /* Find the surface that is visible in the requested direction */
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Color3f(0.0f);

		// sample area light
		if (!its.mesh->isEmitter())
		{
			//scene->EEmitter;
			Point3f y;
			Normal3f ny;
			float eta1 = (float)rand() / RAND_MAX;
			float eta2 = (float)rand() / RAND_MAX;
			its.mesh->samplePosition(Point2f(eta1, eta2), y, ny);
			Vector3f wi = y - its.p;
			wi /= wi.norm();
			BSDFQueryRecord rec(its.shFrame.toLocal(wi), its.shFrame.toLocal(-ray.d), ESolidAngle);
			Color3f fr = its.mesh->getBSDF()->eval(rec);
			//float G = ;
			//float Le = ;
			return fr;
		}
        Normal3f n = its.shFrame.n.cwiseAbs();
        return Color3f(n.x(), n.y(), n.z());
    }

    std::string toString() const {
        return "WhittedIntegrator[]";
    }
};

NORI_REGISTER_CLASS(WhittedIntegrator, "whitted");
NORI_NAMESPACE_END