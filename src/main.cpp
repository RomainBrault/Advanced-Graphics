#include <HDRimage.hpp>

using namespace hdr;

int main( int argc, char* argv[] ) {
	
	if ( argc < 3 )
		return -1;

	image test;
	test.loadPFM( argv[ 1 ] );
	image test2( test );
	test2.normalise( 255 );
	test2.savePNM( "test2.pfm", pnm_t::BinaryColormap );
	test.normalise( 255 );
	test.savePNM( "test1.pfm", pnm_t::BinaryColormap );
    return 0;
}