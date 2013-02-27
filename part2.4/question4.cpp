#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <Phong.hpp>

#define SFMT BinaryColormap

using namespace std;
using namespace hdr;
using namespace obj;

int main(int argc, char** argv) {

    if ( argc < 6 ) {
        return -1;
    }

    float stops = static_cast< float >( std::atof( argv[ 4 ] ) );
    float gamma = static_cast< float >( std::atof( argv[ 5 ] ) );
    uint32_t n_points = static_cast< float >( std::atoi( argv[ 3 ] ) );
    uint32_t img_size = static_cast< float >( std::atoi( argv[ 2 ] ) );

    image r_sphere( img_size, img_size, 1 );
    image probe( img_size, img_size, 1 );
    if ( r_sphere. isEmpty( ) == true ) {
        return -1;
    }

    image latlong;
    image temp;

    sphere s( img_size / 2, img_size / 2, img_size / 2 );
    vect< float, 3 > view( 0, 0, 1 );

    latlong.loadPFM( argv[ 1 ] );
    if ( latlong.isEmpty( ) == true ) {
        return -1;
    }
    rnd::Uniform< float, rnd::Haynes, 6364136223846793005UL, 1UL >
        rng( time( nullptr ) );
    brdf::model b( view, 1.0, 0.0, 1.0 );
    r_sphere.render( s, latlong, view, n_points, b, rng );

    temp = r_sphere;
    temp.linearToneMap( stops );
    temp.gamma( gamma );
    temp.normalise( 255 );
    temp.savePNM( "diffuseprobe.ppm", SFMT );
    return 0;
}
