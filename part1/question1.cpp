#include <HDRimage.hpp>

#define SFMT BinaryColormap

using namespace hdr;

int main( int argc, char* argv[] ) {

    if ( argc < 10 )
        return -1;

    uint32_t n_inputs = argc - 3;
    image* inputs = new image[ n_inputs ];
    if ( inputs == nullptr ) {
        return -1;
    }

    for ( uint32_t i = 0; i < n_inputs; ++i ) {
        inputs[ i ].loadPFM( argv[ i + 1 ] );
    }
    float stops = static_cast< float >( atof( argv[ 8 ] ) );
    float gamma = static_cast< float >( atof( argv[ 9 ] ) );

    image res, temp;
    res.createHDR< pol_cwf >( inputs, n_inputs );
    std::string img_id_lin(
        std::string( "_s" ) + argv[ 8 ] + "_g" + argv[ 9 ] + "_"
    );
    std::string img_id_hist(
        std::string( "_s" ) + argv[ 8 ] + "_g" +
        std::to_string( gamma * 0.45 ) + "_"
    );

    std::cout << "Dynamic range: " << res.dynamicRange( )  << std::endl;
    temp = res;
    temp.savePFM( "result_raw.pfm", SFMT );
    temp.normalise( 255 );
    temp.savePNM( "result_raw.ppm", SFMT );

    temp = res;
    temp.linearToneMap( stops );
    temp.gamma( gamma );
    temp.savePFM( std::string( "result_lin" ) + img_id_lin + ".pfm", SFMT );
    temp.normalise( 255 );
    temp.savePNM( std::string( "result_lin" ) + img_id_lin + ".ppm", SFMT );

    res.histEqToneMap( res.dynamicRange( ) );
    res.gamma( gamma * 0.45f );
    res.savePFM( std::string( "result_hist" ) + img_id_hist + ".pfm", SFMT );
    res.normalise( 255 );
    res.savePNM( std::string( "result_hist" ) + img_id_hist + ".ppm", SFMT );
    return 0;
}