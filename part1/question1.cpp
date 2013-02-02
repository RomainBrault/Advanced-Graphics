#include <HDRimage.hpp>

using namespace hdr;

int main( int argc, char* argv[] ) {

    if ( argc < 10 )
        return -1;


    image test1, test2, test3, test4, test5, test6, test7, res, temp;

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
    temp = res;
    temp.savePFM( "result_raw.pfm", BinaryColormap );
    temp.normalise( 255 );
    temp.savePNM( "result_raw.ppm", BinaryColormap );

    temp = res;
    temp.linearToneMap( static_cast< float >( atof( argv[ 8 ] ) ) );
    temp.gamma( static_cast< float >( atof( argv[ 9 ] ) ) );
    temp.savePFM( "result_lin.pfm", BinaryColormap );
    temp.normalise( 255 );
    temp.savePNM( "result_lin.ppm", BinaryColormap );

    res.histEqToneMap( res.dynamicRange( ) );
    res.gamma( static_cast< float >( atof( argv[ 9 ] ) * 0.45 ) );
    res.savePFM( "result_hist.pfm", BinaryColormap );
    res.normalise( 255 );
    res.savePNM( "result_hist.ppm", BinaryColormap );
    return 0;
}