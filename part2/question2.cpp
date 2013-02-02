#include <HDRimage.hpp>

using namespace hdr;

int main( void ) {

    image test1( 511, 511, 1 );

    test1.fill( 1 );
    test1.circleFilled( 511 / 2, 511 / 2, 511 / 2 );
    // test1.negatif( );

    test1.savePNM( "question2.ppm" );
    return 0;
}