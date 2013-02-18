#include <HDRimage.hpp>
#include <Sphere.hpp>
#include <ctime>

#define SFMT BinaryColormap

using namespace hdr;

int main( int argc, char* argv[] ) {

    if ( argc < 4 ) {
        return -1;
    }

    float stops = static_cast< float >( std::atof( argv[ 2 ] ) );
    float gamma = static_cast< float >( std::atof( argv[ 3 ] ) );

    image latlong;
    image temp;

    latlong.loadPFM( argv[ 1 ] );
    if ( latlong.isEmpty( ) == true ) {
        return -1;
    }

    latlong.linearToneMap( stops );
    latlong.gamma( gamma );
    obj::vect< uint32_t, 2 >* points = latlong.sampleEM( 64, 1000, time( nullptr ) );

    for ( uint32_t i = 0; i < 64; ++i ) {
        std::cout << points[ i ] [ 0 ] << " // " << points[ i ][ 1 ] << std::endl;
    }

    temp = latlong;
    temp.normalise( 255 );
    temp.savePNM( "latlong.ppm", SFMT );

    return 0;
}