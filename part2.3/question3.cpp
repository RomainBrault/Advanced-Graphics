#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <Phong.hpp>

#define SFMT BinaryColormap

using namespace std;
using namespace hdr;
using namespace phong;
using namespace obj;

int main(int argc, char** argv) {

    if ( argc < 8 ) {
        return -1;
    }

    uint32_t nb_samples = std::atoi( argv[ 2 ] );
    uint32_t s          = std::atoi( argv[ 3 ] );
    float    ks         = std::atof( argv[ 4 ] );
    float    kd         = std::atof( argv[ 5 ] );

    image latlong;
    latlong.loadPFM(string( argv[ 1 ] ) );
    if ( latlong.isEmpty( ) == true ) {
        return -1;
    }

    Phong p( s, ks, kd );
    p.generateSamples( nb_samples, time( nullptr ) );
    if ( p.isInitialised( ) == false ) {
        return -1;
    }

    image tmp = latlong;

    for ( uint32_t i = 0; i < nb_samples; ++i) {
        uint32_t theta = p.getTheta( i, tmp.getHeight( ) );
        uint32_t phi   = p.getPhi  ( i, tmp.getWidth ( ) );

        tmp.circleFilled( sphere( 2, phi, theta ) , 0.0, 1.0, 0.0);
    }

    tmp.linearToneMap( std::atof( argv[ 6 ] ) );
    tmp.gamma(std::atof( argv[ 7 ] ) );
    tmp.normalise( 255 );
    tmp.savePNM( "test.ppm", SFMT );

}
