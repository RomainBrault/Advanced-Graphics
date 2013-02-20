#include <iostream>
#include <ctime>
#include <Phong.hpp>

using namespace rnd;
using namespace std;
using namespace obj;

namespace phong {

    Phong::Phong(uint32_t s, float ks, float kd) : ks(ks), kd(kd), s(s), rng( time(nullptr) ) {
	samples = nullptr;
    }
    
    Phong::~Phong() {
	delete[] samples;
    }

    void Phong::generateSamples(uint32_t nb_samples) {
	samples = new vect< float, 2 >[nb_samples];
//	#pragma omp parallel for
	for (uint32_t i = 0; i < nb_samples; ++i) {
	    sample( samples[ i ][ 0 ], samples[ i ][ 1 ] );
	}
    }
	
    void Phong::sample(float& theta, float& phi) {
	float rand = rng.rand(0.0f, 1.0f);
	if (rand >= 0 && rand < kd) {
	    diffuseSample(theta, phi);
	    return;
	}
	
	if (rand >= kd && rand < ks) {
	    specularSample(theta, phi);
	    return;
	}
	
	if (rand >= ks + kd) // What to do? 
	cout << "No contribution" << endl;
	specularSample(theta, phi);
    }

    void Phong::diffuseSample(float& theta, float& phi) {
	theta = acos(1.0 - sqrt(rng.rand(0.0f, 1.0f)));
	phi = 2 * M_PI * rng.rand(0.0f, 1.0f);
    }

    void Phong::specularSample(float& theta, float& phi) {
	theta = acos(pow(1.0 - rng.rand(0.0f, 1.0f), (float)1/(s+1)));
	phi = 2 * M_PI * rng.rand(0.0f, 1.0f);
    }
}
