#include <HDRimage.hpp>
#include <Sphere.hpp>

#define SFMT BinaryColormap

using namespace hdr;
using namespace obj;

int main( int argc, char* argv[] ) {

    if ( argc < 5 ) {
        return -1;
    }

    float stops = static_cast< float >( std::atof( argv[ 3 ] ) );
    float gamma = static_cast< float >( std::atof( argv[ 4 ] ) );
    uint32_t n_points = static_cast< float >( std::atoi( argv[ 2 ] ) );

    image latlong;
    image temp;

    latlong.loadPFM( argv[ 1 ] );
    if ( latlong.isEmpty( ) == true ) {
        return -1;
    }
    rnd::Uniform< float, rnd::Haynes, 6364136223846793005UL, 1UL >
        rng( time( nullptr ) );
    obj::vect< uint32_t, 2 >* points = latlong.sampleEM( n_points, rng );
    if ( points == nullptr ) {
        return -1;
    }

    temp = latlong;
    temp.savePFM( std::string( "sampleEM_" ) + argv[ 2 ] + ".pfm", SFMT );
    temp.linearToneMap( stops );
    temp.gamma( gamma );
    for ( uint32_t i = 0; i < n_points; ++i ) {
        temp.circleFilled(
            sphere( 2, points[ i ][ 1 ], points[ i ][ 0 ] ), 0, 1, 0
        );
    }
    temp.normalise( 255 );
    temp.savePNM( std::string( "sampleEM_" ) + argv[ 2 ] + ".ppm", SFMT );

    return 0;
}
