#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <Phong.hpp>

#define SFMT BinaryColormap

using namespace std;
using namespace hdr;
using namespace obj;

int main(int argc, char** argv) {

    if ( argc < 5 ) {
        return -1;
    }

    float stops = static_cast< float >( std::atof( argv[ 3 ] ) );
    float gamma = static_cast< float >( std::atof( argv[ 4 ] ) );
    uint32_t n_points = static_cast< float >( std::atoi( argv[ 2 ] ) );
    uint32_t img_size = 511;

    image r_sphere( img_size, img_size, 1 );
    image probe( img_size, img_size, 1 );
    if ( r_sphere. isEmpty( ) == true ) {
        return -1;
    }

    image latlong;
    image temp(img_size, img_size, 3);

    sphere s( img_size / 2, img_size / 2, img_size / 2 );
    vect< float, 3 > view( 0, 0, 1 );

    latlong.loadPFM( argv[ 1 ] );
    if ( latlong.isEmpty( ) == true ) {
        return -1;
    }
    rnd::Uniform< float > rng( time( nullptr ) );
<<<<<<< HEAD
    brdf::model b( view, 1.0, 0.0, 1.0 );
    r_sphere.render( s, latlong, view, n_points, b, rng );

    temp = r_sphere;
    temp.linearToneMap( stops );
    temp.gamma( gamma );
=======
    obj::vect< uint32_t, 2 >* points = latlong.sampleEM( n_points, rng );
    if ( points == nullptr ) {
        return -1;
    }

    /* Add code here. */

    temp.renderIS(s, view, latlong, brdf::model(view, 1.0, 0.0, 1.0), points, n_points);

    temp.linearToneMap( 7 );
    temp.gamma( 2.2 );

>>>>>>> a9dd86aed033eae61db0711bd32c8f3e1a7faaeb
    temp.normalise( 255 );
    temp.savePNM( "diffuseprobe.ppm", SFMT );
    return 0;
}
