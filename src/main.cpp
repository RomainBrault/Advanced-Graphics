#include <HDRimage.hpp>

using namespace hdr;

int main( int argc, char* argv[] ) {
	
	if ( argc < 3 )
		return -1;


	image test1, test2, test3, test4, test5, test6, test7, res;

	test1.loadPFM( argv[ 1 ] );
	test2.loadPFM( argv[ 2 ] );
	test3.loadPFM( argv[ 3 ] );
	test4.loadPFM( argv[ 4 ] );
	test5.loadPFM( argv[ 5 ] );
	test6.loadPFM( argv[ 6 ] );
	test7.loadPFM( argv[ 7 ] );

	res.createHDR< pol_cwf >( 
		test1, test2, test3, test4, test5, test6, test7 
	);
	std::cout << res.dynamicRange( )  << std::endl<< std::endl;
	res.normalise( 1 );
	res.linearToneMap( atoi( argv[ 8 ] ) );
	res.troncate( 0, 1 );
	res.gamma( atof( argv[ 9 ] ) );
	res.normalise( 255 );
	res.savePNM( "temp.ppm", BinaryColormap );
    return 0;
}