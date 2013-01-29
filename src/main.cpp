#include <HDRimage.hpp>

int main( int argc, char* argv[] ) {
	
	if ( argc < 3 )
		return -1;

	hdr::image test;
	test.loadPNM( argv[ 1 ] );
	test.normalise( 255 );
	test.savePNM( argv[ 2 ], hdr::BinaryColormap );
    return 0;
}