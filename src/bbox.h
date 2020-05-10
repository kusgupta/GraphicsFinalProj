#pragma once

#include <glm/vec3.hpp>
class ray;

class kdBox {
	bool bEmpty;
	bool dirty;
	glm::dvec3 bmin;
	glm::dvec3 bmax;
	double bArea   = 0.0;
	double bVolume = 0.0;

public:
	kdBox();
	kdBox(glm::vec3 bMin, glm::vec3 bMax);
    float RAY_EPSILON = .00001;
	glm::dvec3 getMin() const { return bmin; }
	glm::dvec3 getMax() const { return bmax; }
	bool isEmpty() { return bEmpty; }
	void setEmpty() { bEmpty = true; }

	void setMin(glm::dvec3 bMin)
	{
		bmin   = bMin;
		dirty  = true;
		bEmpty = false;
	}
	void setMax(glm::dvec3 bMax)
	{
		bmax   = bMax;
		dirty  = true;
		bEmpty = false;
	}

	void setMin(int i, double val)
	{
		if (i >= 0 && i <= 2) {
			bmin[i] = val;
			bEmpty = false;
		}
	}

	void setMax(int i, double val)
	{
		if (i >= 0 && i <= 2) {
			bmax[i] = val;
			bEmpty = false;
		}
	}

	// Does this bounding box intersect the target?
	bool intersects(const kdBox& target) const;

	// does the box contain this point?
	bool intersects(const glm::dvec3& point) const;

	// if the ray hits the box, put the "t" value of the intersection
	// closest to the origin in tMin and the "t" value of the far
	// intersection
	// in tMax and return true, else return false.
	bool intersect(glm::dvec3 position, glm::dvec3 direction, double& tMin, double& tMax) const;

	void operator=(const kdBox& target);
	double area();
	void merge(const kdBox& bBox);
};
