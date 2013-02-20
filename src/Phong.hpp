#ifndef __PHONG__HPP__
#define __PHONG__HPP__

#include <vector>
#include <cstdlib>
#include <ctime>
#include <Rand.hpp>

typedef std::vector<float*> Samples;

namespace phong {

    class Phong {
    public:
	Phong(uint32_t s, float ks = 0.5, float kd = 0.5);
	Samples generateSamples(uint32_t nb_samples);

    private:
	float* sample();
	float* diffuseSample();
	float* specularSample();
	float randf(float inf, float sup);

	float ks;
	float kd;
	uint32_t s;
	static rnd::xorShift rng;
};

}

#endif
