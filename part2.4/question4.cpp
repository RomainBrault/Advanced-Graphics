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
    rnd::Uniform< float > rng( time( nullptr ) );
    obj::vect< uint32_t, 2 >* points = latlong.sampleEM( n_points, rng );
    if ( points == nullptr ) {
        return -1;
    }
    temp = latlong;

    /* Add code here. */

    temp.renderIS(s, view, latlong, brdf::model(view, 1.0, 0.0, 1.0), points, n_points);

    temp.linearToneMap( 9 );
    temp.gamma( 2.2 );
    temp.normalise( 255 );
    temp.savePNM( "latlong.ppm", SFMT );

    return 0;
}
