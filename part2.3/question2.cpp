#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <Phong.hpp>

#define SFMT BinaryColormap

using namespace std;
using namespace hdr;
using namespace phong;
using namespace obj;

int main(int argc, char** argv) {

    cout << argv[1] << " " << argv[2] << endl;

    Phong p(atof(argv[1]));
    Samples samples = p.generateSamples(atoi(argv[2]));

    image latlong;
    latlong.loadPFM("../CO417-HW1/GraceCathedral/grace_latlong.pfm");
    image tmp = latlong;
    tmp.linearToneMap( 9.0 );
    tmp.gamma( 2.2 );

    for (Samples::iterator it = samples.begin(); it != samples.end(); ++it) {
	tmp.circleFilled( sphere(2, (int)(*it)[1]/(2 * M_PI) * tmp.getWidth(), (int)(M_PI - (*it)[0])/M_PI * tmp.getHeight(), 0) , 0.0, 1.0, 0.0);
//	tmp.setPixel((int)(*it)[1]/(2 * M_PI) * tmp.getWidth(), (int)(*it)[0]/M_PI * tmp.getHeight(), 1.0, 1.0, 1.0);
	cout << "phi = " << (*it)[1]/( 2 * M_PI) * tmp.getWidth() << " theta = " << (M_PI - (*it)[0])/M_PI * tmp.getHeight() << endl;	
    }
    cout << tmp.getWidth() << " " << tmp.getHeight() << endl;
    tmp.normalise(255);
    tmp.savePNM("test.ppm", SFMT);

}
