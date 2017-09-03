#include <nori/emitter.h>

NORI_NAMESPACE_BEGIN

class AreaLight : public Emitter
{
public:
    AreaLight(const PropertyList &props)
    {
        radiance = props.getColor("radiance");
    }

private:

    Color3f radiance;
};

// NORI_REGISTER_CLASS(NormalIntegrator, "normals");

NORI_NAMESPACE_END