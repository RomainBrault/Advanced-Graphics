#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <Phong.hpp>

#define SFMT BinaryColormap

using namespace std;
using namespace hdr;
using namespace phong;
using namespace obj;

int main(int argc, char** argv) {

    if (argc < 3)
	return -1;

    Phong p(atof(argv[1]), 0.4, 0.6);
    int nb_samples = atoi(argv[2]);
    p.generateSamples(nb_samples);

    image latlong;
    latlong.loadPFM("../CO417-HW1/GraceCathedral/grace_latlong.pfm");
    image tmp = latlong;

    for (int i = 0; i < nb_samples; ++i) {
	uint32_t theta = p.getTheta(i, tmp.getHeight());
	uint32_t phi = p.getPhi(i, tmp.getWidth());
	
	tmp.circleFilled( sphere(2, phi, theta) , 0.0, 1.0, 0.0);
    }
    tmp.linearToneMap( 9.0 );
    tmp.gamma( 2.2 );
    tmp.normalise(255);
    tmp.savePNM("test.ppm", SFMT);

}
