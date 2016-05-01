#include "StoneTexture.h"
#include "Worley.h"
#include "Perlin.h"
StoneTexture::StoneTexture(float s, int o) :
     size(s), order(o)
{

}

StoneTexture::~StoneTexture()
{
}
Vector3 StoneTexture::getColor(Vector3& v) const {
    float r;
    float g;
    float b;
    r = g = b = 0.0f;

    float x = v.x*size;
    float y = v.y*size;
    float z = v.z*size;

    float Fnums[order];
    float delta[order][3];
    unsigned int id[3];
    float at[3] = {x, y, z};

    WorleyNoise::noise3D(at, order, Fnums, delta, id);

    float color = Fnums[1] - Fnums[0]; // For stone texturing (according to lecture slides)
    // Cell walls
    if (color < 0.05f) {
		r = 0.85f; g = 0.81f; b = 0.81f;
    //Inside stones
	} else {
		// add Perlin noise
		float noise = PerlinNoise::noise(x, y, z);
		color += fabs(noise);
        // r = g = b = color;
		r += color + (float)(id[0] % 10) / 10.0f;
		g += color + (float)(id[0] % 10) / 25.0f;
		b += color + (float)(id[0] % 10) / 25.0f;
	}

    return Vector3(r, g, b);
}
