#include <HDRimage.hpp>
#include <Monitoring.hpp>

#define is_power_of_two( n ) ( ( ( n ) != 0 ) && !( ( n ) & ( ( n ) - 1 ) ) )

namespace hdr {

image::image( void ) :
    m_width( 0 ),
    m_height( 0 ),
    m_padd( 0 ),
    m_max_pixel_chanel( 0 ),
    m_data_2D( nullptr ),
    m_data_1D( nullptr )
{

}

image::image( uint32_t width, uint32_t height ) :
    m_width( width ),
    m_height( height ),
    m_max_pixel_chanel( 0 )
{
    alloc( PAGE_SIZE, SSE_ALIGN );
}

image::~image( void ) 
{
    std::free( m_data_2D );
    std::free( m_data_1D );
}

void 
image::alloc( uint32_t align, uint32_t padding ) noexcept {
    if ( 
        ( is_power_of_two( align ) == false ) || 
        ( align < sizeof( std::ptrdiff_t ) ) 
    ) {
        m_data_1D = nullptr;
        m_data_2D = nullptr;
        m_width = 0;
        m_height = 0;
        m_padd = 0;
        return;
    }
    m_padd = ( 1 + ( m_width - 1 ) / 8 ) * sizeof ( pixelBlock );
    m_padd += ( ( -m_padd ) % padding );
    size_t size = m_padd * m_height;

    m_data_1D = reinterpret_cast< pixelBlock * >(
        std::malloc( size + align - 1 )
    );
    if ( m_data_1D == nullptr ) {
        m_data_2D = nullptr;
        m_width = 0;
        m_height = 0;
        m_padd = 0;
    }

    std::ptrdiff_t modulo = align - static_cast< std::ptrdiff_t >(
        reinterpret_cast< std::ptrdiff_t >( m_data_1D ) & ( align - 1 )
    );
    uint8_t * p = reinterpret_cast< uint8_t * >( m_data_1D ) + modulo;
    if (
        static_cast< size_t >(
            p - reinterpret_cast< uint8_t * >( m_data_1D )
        ) < sizeof( std::ptrdiff_t )
    ) {
        goto ERROR;
    }
    reinterpret_cast< std::ptrdiff_t * >( p )[ -1 ] = modulo;

    m_data_2D = reinterpret_cast< pixelBlock ** >(
        std::malloc( m_height * sizeof ( pixelBlock* ) )
    );
    if ( m_data_2D == nullptr ) {
        goto ERROR;
    }
    for ( uint32_t i = 0; i < m_height; ++i ) {
        m_data_2D[ i ] = reinterpret_cast< pixelBlock * >(
            reinterpret_cast< uint8_t * >( m_data_1D ) + m_padd * i
        );
    }
    return;

ERROR:
    std::free( m_data_2D );
    m_data_2D = nullptr;
    m_data_1D = nullptr;
    m_width = 0;
    m_height = 0;
    m_padd = 0;
}

void 
image::free( void ) noexcept
{
    if ( m_data_2D != nullptr ) std::free( m_data_2D );
    if ( m_data_1D != nullptr ) std::free( m_data_1D );
    m_data_2D = nullptr;
    m_data_1D = nullptr;
    m_width = 0;
    m_height = 0;
    m_padd = 0;
}


static void 
skip( std::ifstream & file ) noexcept
/* Read at least 1 char. */
{
    char temp;
    do {
        temp = static_cast< char >( file.get( ) );
        if ( temp == '#') // skip this line:
            file.ignore( std::numeric_limits< std::streamsize >::max( ), '\n' );
    } while ( 
        ( ( temp == ' ' )  || ( temp == '\t' )   || 
          ( temp == '\n' ) || ( temp == '#'  ) ) &&
        ( !file.eof( ) )
    );
    file.putback( temp );
}

void 
image::normalise( float n ) noexcept
{
    /* Stability ?? */
    uint32_t width_block_number( ( m_width - 1 ) / 8 );
    uint32_t width_block_end( width_block_number * 8 );
    float multiplier = n / m_max_pixel_chanel;
    m_max_pixel_chanel = n;

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        /* Inner loop vectorized with gcc ! */
        for ( uint32_t j = 0; j < width_block_number; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] *= multiplier;
                m_data_2D[ i ][ j ].g[ k ] *= multiplier;
                m_data_2D[ i ][ j ].b[ k ] *= multiplier;
            }
        }
        for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
            m_data_2D[ i ][ width_block_number ].r[ k ] *= multiplier;
            m_data_2D[ i ][ width_block_number ].g[ k ] *= multiplier;
            m_data_2D[ i ][ width_block_number ].b[ k ] *= multiplier;
        }
    }
}

int32_t
image::loadPNM( std::string const & file_path ) noexcept
{
    std::ifstream pnm_file;
    uint32_t width_block_number;
    uint32_t width_block_end;
    uint32_t elt_size;

    pnm_file.open( file_path.c_str( ), std::ios::in | std::ios::binary );
    if ( !pnm_file ) {
        goto FILE_ERROR;
    }

    // read magic number
    char magic_number[ 2 ];
    pnm_file.read( magic_number, 2 * sizeof ( uint8_t ) );
    if ( magic_number[ 0 ] != 'P' ) {
        pnm_file.close( );
        goto FILE_ERROR;
    }

    m_width = 0;
    m_height = 0;
    skip( pnm_file );
    pnm_file >> m_width;
    skip( pnm_file );
    pnm_file >> m_height;
    skip( pnm_file );
    if ( ( m_width == 0 ) || ( m_height == 0 ) ) {
        pnm_file.close( );
        goto FILE_ERROR;
    }

    switch ( magic_number[ 1 ] ) {
        case '1': // ASCII bitmap
            goto FILE_ERROR;
            break;
        case '2': // ASCII greymap
            pnm_file >> m_max_pixel_chanel;
            skip( pnm_file );
            alloc( PAGE_SIZE, SSE_ALIGN );
            width_block_number = ( m_width - 1 ) / 8;
            width_block_end = width_block_number * 8;
            for ( uint32_t i = 0; i < m_height; ++i ) {
                for ( uint32_t j = 0; j < width_block_number; ++j ) {
                    for ( uint32_t k = 0; k < 8; ++k ) {
                        float grey_val;
                        pnm_file >> grey_val;
                        skip( pnm_file );
                        m_data_2D[ i ][ j ].r[ k ] = grey_val;
                        m_data_2D[ i ][ j ].g[ k ] = grey_val;
                        m_data_2D[ i ][ j ].b[ k ] = grey_val;
                    }
                }
                for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                    float grey_val;
                    pnm_file >> grey_val;
                    skip( pnm_file );
                    m_data_2D[ i ][ width_block_number ].r[ k ] = grey_val;
                    m_data_2D[ i ][ width_block_number ].g[ k ] = grey_val;
                    m_data_2D[ i ][ width_block_number ].b[ k ] = grey_val;
                }
            }
            break;
        case '3': // ASCII RGB
            pnm_file >> m_max_pixel_chanel;
            skip( pnm_file );
            alloc( PAGE_SIZE, SSE_ALIGN );
            width_block_number = ( m_width - 1 ) / 8;
            width_block_end = width_block_number * 8;
            for ( uint32_t i = 0; i < m_height; ++i ) {
                for ( uint32_t j = 0; j < width_block_number; ++j ) {
                    for ( uint32_t k = 0; k < 8; ++k ) {
                        float red, blue, green;
                        pnm_file >> red;
                        skip( pnm_file );
                        pnm_file >> blue;
                        skip( pnm_file );
                        pnm_file >> green;
                        skip( pnm_file );
                        m_data_2D[ i ][ j ].r[ k ] = red;
                        m_data_2D[ i ][ j ].g[ k ] = blue;
                        m_data_2D[ i ][ j ].b[ k ] = green;
                    }
                }
                for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                    float red, blue, green;
                    pnm_file >> red;
                    pnm_file >> blue;
                    pnm_file >> green;
                    m_data_2D[ i ][ width_block_number ].r[ k ] = red;
                    m_data_2D[ i ][ width_block_number ].g[ k ] = blue;
                    m_data_2D[ i ][ width_block_number ].b[ k ] = green;
                }
            }
            break;
        case '4': // binary bitmap
            pnm_file.close( );
            goto FILE_ERROR;
            break;
        case '5': // binary greymap
            pnm_file >> m_max_pixel_chanel;
            skip( pnm_file );
            alloc( PAGE_SIZE, SSE_ALIGN );
            width_block_number = ( m_width - 1 ) / 8;
            width_block_end = width_block_number * 8;
            elt_size = m_max_pixel_chanel > 255 ? 2 : 1;
            if ( elt_size == 1 ) {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint8_t buffer[ 8 ];
                        pnm_file.read( 
                            reinterpret_cast< char * >( buffer ), 
                            8 * sizeof ( uint8_t ) 
                        );
                        skip( pnm_file );
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            m_data_2D[ i ][ j ].r[ k ] = 
                                static_cast< float >( buffer[ k ] );
                            m_data_2D[ i ][ j ].g[ k ] = 
                                static_cast< float >( buffer[ k ] );
                            m_data_2D[ i ][ j ].b[ k ] = 
                                static_cast< float >( buffer[ k ] );
                        }
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint8_t grey_val( 0 );
                        pnm_file.read( 
                            reinterpret_cast< char * >( &grey_val ), 
                            sizeof ( uint8_t ) 
                        );
                        skip( pnm_file );
                        m_data_2D[ i ][ width_block_number ].r[ k ] = 
                            static_cast< float >( grey_val );
                        m_data_2D[ i ][ width_block_number ].g[ k ] = 
                            static_cast< float >( grey_val );
                        m_data_2D[ i ][ width_block_number ].b[ k ] = 
                            static_cast< float >( grey_val );
                    }
                }
            }
            else {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint16_t buffer[ 24 ];
                        pnm_file.read( 
                            reinterpret_cast< char * >( buffer ), 
                            24 * sizeof ( uint16_t ) 
                        );
                        skip( pnm_file );
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            m_data_2D[ i ][ j ].r[ k ] = 
                                static_cast< float >( buffer[ k ] );
                            m_data_2D[ i ][ j ].g[ k ] = 
                                static_cast< float >( buffer[ k ] );
                            m_data_2D[ i ][ j ].b[ k ] = 
                                static_cast< float >( buffer[ k ] );
                        }
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint16_t grey_val( 0 );
                        pnm_file.read( 
                            reinterpret_cast< char * >( &grey_val ), 
                            sizeof ( uint16_t ) 
                        );
                        skip( pnm_file );
                        m_data_2D[ i ][ width_block_number ].r[ k ] = 
                            static_cast< float >( grey_val );
                        m_data_2D[ i ][ width_block_number ].g[ k ] = 
                            static_cast< float >( grey_val );
                        m_data_2D[ i ][ width_block_number ].b[ k ] = 
                            static_cast< float >( grey_val );
                    }
                }
            }
            break;
        case '6': // binary RGB
            pnm_file >> m_max_pixel_chanel;
            skip( pnm_file );
            alloc( PAGE_SIZE, SSE_ALIGN );
            width_block_number = ( m_width - 1 ) / 8;
            width_block_end = width_block_number * 8;
            elt_size = m_max_pixel_chanel > 255 ? 2 : 1;
            if ( elt_size == 1 ) {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint8_t buffer[ 24 ];
                        pnm_file.read( 
                            reinterpret_cast< char * >( buffer ), 
                            24 * sizeof ( uint8_t ) 
                        );
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            m_data_2D[ i ][ j ].r[ k ] = 
                                static_cast< float >( buffer[ 3 * k     ] );
                            m_data_2D[ i ][ j ].g[ k ] = 
                                static_cast< float >( buffer[ 3 * k + 1 ] );
                            m_data_2D[ i ][ j ].b[ k ] = 
                                static_cast< float >( buffer[ 3 * k + 2 ] );
                        }
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint8_t colors[ 3 ];
                        pnm_file.read( 
                            reinterpret_cast< char * >( colors ), 
                            3 * sizeof ( uint8_t ) 
                        );
                        m_data_2D[ i ][ width_block_number ].r[ k ] = 
                            static_cast< float >( colors[ 0 ] );
                        m_data_2D[ i ][ width_block_number ].g[ k ] = 
                            static_cast< float >( colors[ 1 ] );
                        m_data_2D[ i ][ width_block_number ].b[ k ] = 
                            static_cast< float >( colors[ 2 ] );
                    }
                }
            }
            else {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint16_t buffer[ 24 ];
                        pnm_file.read( 
                            reinterpret_cast< char * >( buffer ), 
                            24 * sizeof ( uint16_t ) 
                        );
                        skip( pnm_file );
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            m_data_2D[ i ][ j ].r[ k ] = 
                                static_cast< float >( buffer[ 3 * k     ] );
                            m_data_2D[ i ][ j ].g[ k ] = 
                                static_cast< float >( buffer[ 3 * k + 1 ] );
                            m_data_2D[ i ][ j ].b[ k ] = 
                                static_cast< float >( buffer[ 3 * k + 2 ] );
                        }
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint16_t colors[ 3 ];
                        pnm_file.read( 
                            reinterpret_cast< char * >( colors ), 
                            3 * sizeof ( uint16_t ) 
                        );
                        skip( pnm_file );
                        m_data_2D[ i ][ width_block_number ].r[ k ] = 
                            static_cast< float >( colors[ 0 ] );
                        m_data_2D[ i ][ width_block_number ].g[ k ] = 
                            static_cast< float >( colors[ 1 ] );
                        m_data_2D[ i ][ width_block_number ].b[ k ] = 
                            static_cast< float >( colors[ 2 ] );
                    }
                }
            }
            break;
        default:
            pnm_file.close( );
            goto FILE_ERROR;
            break;
    }

    pnm_file.close( );
    if ( !pnm_file ) {
        goto FILE_ERROR;
    }
    return 0;


FILE_ERROR:
    std::cerr << "Cannot read file '" 
                      << file_path 
                      << "'." 
                      << std::endl;
    free( );
    return -1;
}

int32_t 
image::savePNM( 
    std::string const & file_path, uint32_t magic_number 
) const noexcept {
    std::ofstream pnm_file;
    uint32_t width_block_number;
    uint32_t width_block_end;
    uint32_t elt_size;

    pnm_file.open( 
        file_path.c_str( ), 
        std::ios::out | std::ios::binary | std::ios::trunc
    );
    if ( !pnm_file ) {
        goto FILE_ERROR;
    }

    pnm_file.put( 'P' );
    pnm_file.put( static_cast< char >( magic_number ) );

    pnm_file << "\n" << m_width << " " << m_height << "\n";


    switch ( magic_number ) {
        case '1': // ASCII bitmap
            pnm_file.close( );
            goto FILE_ERROR;
            break;
        case '2': // ASCII greymap
            pnm_file.close( );
            goto FILE_ERROR;
            break;
        case '3': // ASCII RGB
            pnm_file.close( );
            goto FILE_ERROR;
            break;
        case '4': // binary bitmap
            pnm_file.close( );
            goto FILE_ERROR;
            break;
        case '5': // binary greymap
            pnm_file << m_max_pixel_chanel << "\n";
            width_block_number = ( m_width - 1 ) / 8;
            width_block_end = width_block_number * 8;
            elt_size = m_max_pixel_chanel > 255 ? 2 : 1;
            if ( elt_size == 1 ) {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint8_t buffer[ 8 ];
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            buffer[ k ] = static_cast< uint8_t >( 
                                ( m_data_2D[ i ][ j ].r[ k ] +
                                  m_data_2D[ i ][ j ].g[ k ] +
                                  m_data_2D[ i ][ j ].b[ k ] ) /3
                            );
                        }
                        pnm_file.write( 
                            reinterpret_cast< char * >( buffer ), 
                            8 * sizeof ( uint8_t ) 
                        );
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint8_t grey_val;
                        grey_val = static_cast< uint8_t >(
                            ( m_data_2D[ i ][ width_block_number ].r[ k ] +
                              m_data_2D[ i ][ width_block_number ].g[ k ] +
                              m_data_2D[ i ][ width_block_number ].b[ k ] ) / 3
                        );
                        pnm_file.write( 
                            reinterpret_cast< char * >( &grey_val ), 
                            sizeof ( uint8_t ) 
                        );
                    }
                }
            }
            else {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint16_t buffer[ 8 ];
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            buffer[ k ] = static_cast< uint16_t >( 
                                ( m_data_2D[ i ][ j ].r[ k ] +
                                  m_data_2D[ i ][ j ].g[ k ] +
                                  m_data_2D[ i ][ j ].b[ k ] ) /3
                            );
                        }
                        pnm_file.write( 
                            reinterpret_cast< char * >( buffer ), 
                            8 * sizeof ( uint16_t ) 
                        );
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint16_t grey_val;
                        grey_val = static_cast< uint16_t >(
                            ( m_data_2D[ i ][ width_block_number ].r[ k ] +
                              m_data_2D[ i ][ width_block_number ].g[ k ] +
                              m_data_2D[ i ][ width_block_number ].b[ k ] ) / 3
                        );
                        pnm_file.write( 
                            reinterpret_cast< char * >( &grey_val ), 
                            sizeof ( uint16_t ) 
                        );
                    }
                }
            }
            break;
            break;
        case '6': // binary RGB
            pnm_file << m_max_pixel_chanel << "\n";
            width_block_number = ( m_width - 1 ) / 8;
            width_block_end = width_block_number * 8;
            elt_size = m_max_pixel_chanel > 255 ? 2 : 1;
            if ( elt_size == 1 ) {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint8_t buffer[ 24 ];
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            buffer[ 3 * k     ] = static_cast< uint8_t >( 
                                m_data_2D[ i ][ j ].r[ k ] 
                            );
                            buffer[ 3 * k + 1 ] = static_cast< uint8_t >( 
                                m_data_2D[ i ][ j ].g[ k ] 
                            );
                            buffer[ 3 * k + 2 ] = static_cast< uint8_t >( 
                                m_data_2D[ i ][ j ].b[ k ] 
                            );
                        }
                        pnm_file.write( 
                            reinterpret_cast< char * >( buffer ), 
                            24 * sizeof ( uint8_t ) 
                        );
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint8_t colors[ 3 ];
                        colors[ 0 ] = static_cast< uint8_t >(
                            m_data_2D[ i ][ width_block_number ].r[ k ]
                        );
                        colors[ 1 ] = static_cast< uint8_t >(
                            m_data_2D[ i ][ width_block_number ].g[ k ]
                        );
                        colors[ 2 ] = static_cast< uint8_t >(
                            m_data_2D[ i ][ width_block_number ].b[ k ]
                        );
                        pnm_file.write( 
                            reinterpret_cast< char * >( colors ), 
                            3 * sizeof ( uint8_t ) 
                        );
                    }
                }
            }
            else {
                for ( uint32_t i = 0; i < m_height; ++i ) {
                    for ( uint32_t j = 0; j < width_block_number; ++j ) {
                        uint16_t buffer[ 24 ];
                        for ( uint32_t k = 0; k < 8; ++k ) {
                            buffer[ 3 * k     ] = static_cast< uint16_t >( 
                                m_data_2D[ i ][ j ].r[ k ] 
                            );
                            buffer[ 3 * k + 1 ] = static_cast< uint16_t >( 
                                m_data_2D[ i ][ j ].g[ k ] 
                            );
                            buffer[ 3 * k + 2 ] = static_cast< uint16_t >( 
                                m_data_2D[ i ][ j ].b[ k ] 
                            );
                        }
                        pnm_file.write( 
                            reinterpret_cast< char * >( buffer ), 
                            24 * sizeof ( uint16_t ) 
                        );
                    }
                    for ( uint32_t k = 0; width_block_end + k < m_width; ++k ) {
                        uint16_t colors[ 3 ];
                        colors[ 0 ] = static_cast< uint16_t >(
                            m_data_2D[ i ][ width_block_number ].r[ k ]
                        );
                        colors[ 1 ] = static_cast< uint16_t >(
                            m_data_2D[ i ][ width_block_number ].g[ k ]
                        );
                        colors[ 2 ] = static_cast< uint16_t >(
                            m_data_2D[ i ][ width_block_number ].b[ k ]
                        );
                        pnm_file.write( 
                            reinterpret_cast< char * >( colors ), 
                            3 * sizeof ( uint16_t ) 
                        );
                    }
                }
            }
            break;
        default:
            pnm_file.close( );
            goto FILE_ERROR;
            break;
    }

    pnm_file.close( );
    if ( !pnm_file ) {
        goto FILE_ERROR;
    }
    return 0;

FILE_ERROR:
    std::cerr << "Failed to create consistent file '" 
                      << file_path 
                      << "'." 
                      << std::endl;
    return -1;
}

} // namespace hdr