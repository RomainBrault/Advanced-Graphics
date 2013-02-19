#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <Rand.hpp>
#include <ctime>

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
    obj::vect< uint32_t, 2 >* points = latlong.sampleEM( n_points, time( nullptr ) );

    temp = latlong;
    temp.linearToneMap( stops );
    temp.gamma( gamma );
    for ( uint32_t i = 0; i < n_points; ++i ) {
        temp.circleFilled( sphere( 2, points[ i ][ 1 ], points[ i ][ 0 ] ), 0, 1, 0 ) ;
    }
    temp.normalise( 255 );
    temp.savePNM( "latlong.ppm", SFMT );

    return 0;
}