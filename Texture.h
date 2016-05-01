#ifndef CSE168_TEXTURE_H_INCLUDED
#define CSE168_TEXTURE_H_INCLUDED

#include "Vector3.h"

class Texture {
public:
	Texture() {}
	virtual ~Texture() {}

	virtual Vector3 getColor(Vector3& v) const { return Vector3(0.0f); }
};

#endif //CSE168_TEXTURE_H_INCLUDED
