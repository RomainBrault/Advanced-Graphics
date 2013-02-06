#include <HDRimage.hpp>
#include <Sphere.hpp>

using namespace hdr;
using namespace obj;

int main( int argc, char * argv[] ) {

    if ( argc < 5 ) {
        return -1;
    }
    uint32_t img_size = std::atoi( argv[ 2 ] );
    sphere s( img_size / 2, img_size / 2, img_size / 2 );
    vect< float, 3 > view( 0, 0, 1 );

    if ( argc > 7 ) {
        view[ 0 ] = std::atof( argv[ 5 ] );
        view[ 1 ] = std::atof( argv[ 6 ] );
        view[ 2 ] = std::atof( argv[ 7 ] );
        float norm = view.norm( );
        view[ 0 ] /= norm;
        view[ 1 ] /= norm;
        view[ 2 ] /= norm;
    }
    else if ( argc >= 5 ) {
        return -1;
    }

    image r_sphere( img_size, img_size, 1 );
    image latlong;

    latlong.loadPFM( argv[ 1 ] );
    if ( latlong.isEmpty( ) == true ) {
        return -1;
    }

    image temp = r_sphere;
    temp.fill( -1 );
    temp.reflectanceSphere( s, view );
    temp.savePFM( "question2_reflection.pfm" );
    temp.normalise( 255 );
    temp.savePNM( "question2_reflection.ppm" );

    r_sphere.fill( 0 );
    r_sphere.mapLatLong( s, view, latlong );
    r_sphere.savePFM( "question2.pfm" );
    r_sphere.linearToneMap( std::atof( argv[ 3 ] ) );
    r_sphere.gamma( std::atof( argv[ 4 ] ) );
    r_sphere.normalise( 255 );
    r_sphere.savePNM( "question2.ppm" );
    return 0;
}