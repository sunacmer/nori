#include <nori/emitter.h>

NORI_NAMESPACE_BEGIN

class AreaLight : public Emitter
{
public:

    AreaLight(const PropertyList &props)
    {
        radiance = props.getColor("radiance");
    }

	string toString() const
	{
		return "AreaLight[]";
	}

private:

    Color3f radiance;
};

 NORI_REGISTER_CLASS(AreaLight, "area");
NORI_NAMESPACE_END