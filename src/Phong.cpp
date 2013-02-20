#include <iostream>
#include <cmath>
#include <ctime>
#include <Phong.hpp>

using namespace rnd;
using namespace std;

namespace phong {

    xorShift Phong::rng = xorShift( time(nullptr) );

    Phong::Phong(uint32_t s, float ks, float kd) : ks(ks), kd(kd), s(s) {
	srand( time(nullptr) );
    }

    Samples Phong::generateSamples(uint32_t nb_samples) {
	vector<float*> samples;
	for (uint32_t i = 0; i < nb_samples; ++i) {
	    samples.push_back(sample());
	}
	return samples;
    }
	
    float* Phong::sample() {
/*	float rand = randf(0.0f, 1.0f);
	if (rand >= 0 && rand < kd) {
	    return diffuseSample();
	}
	
	if (rand >= kd && rand < ks) {
	    return specularSample();
	}
	
	if (rand >= ks + kd) // What to do? 
	cout << "No contribution" << endl;*/
	return specularSample();
    }

    float* Phong::diffuseSample() {
	float* dir = new float[2];
	dir[0] = acos(1.0 - sqrt(randf(0.0f, 1.0f)));
	dir[1] = 2 * M_PI * randf(0.0f, 1.0f);
	return dir;
    }

    float* Phong::specularSample() {
	float* dir = new float[2];
	dir[0] = acos(pow(1.0 - randf(0.0f, 1.0f), (float)1/(s+1)));
	dir[1] = 2 * M_PI * randf(0.0f, 1.0f);
	return dir;
    }

    float Phong::randf(float inf, float sup) {
	return ( rand()/(float)RAND_MAX ) * (sup-inf) + inf;
    }
}
