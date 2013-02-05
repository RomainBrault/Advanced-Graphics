#include <HDRimage.hpp>

using namespace hdr;

int main( void ) {

    image test1( 511, 511, 1 );

    test1.fill( 0 );
    test1.circleFilledXYZ( 511 / 2, 511 / 2, 511 / 2 );

    test1.normalise( 255 );
    test1.savePNM( "question2.ppm" );
    return 0;
}