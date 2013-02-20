#ifndef __PHONG__HPP__
#define __PHONG__HPP__

#include<HDRimage.hpp>

#include <vector>
#include <ctime>
#include <Sphere.hpp>
#include <Rand.hpp>

namespace phong {

    class Phong {
    public:
	Phong(uint32_t s, float ks = 0.5, float kd = 0.5);
	~Phong();
	void generateSamples(uint32_t nb_samples);
	INLINE uint32_t getTheta(uint32_t i, uint32_t height);
	INLINE uint32_t getPhi(uint32_t i, uint32_t width);

    private:
	void sample(float& theta, float& phi);
	void diffuseSample(float& theta, float& phi);
	void specularSample(float& theta, float& phi);

	float ks;
	float kd;
	uint32_t s;
	obj::vect<float, 2>* samples;
	rnd::xorShift rng;
};

    uint32_t Phong::getTheta(uint32_t i, uint32_t height) {
	return static_cast<uint32_t>(samples[ i ][ 0 ] / M_PI * height);
    }

    uint32_t Phong::getPhi(uint32_t i, uint32_t width) {
	return static_cast<uint32_t>(samples[ i ][ 1 ] / (2 * M_PI) * width);
    }

}

#endif
