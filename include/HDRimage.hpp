#ifndef HDRIMAGE_HPP_INCLUDED
#define HDRIMAGE_HPP_INCLUDED

#include <HDRimageConfig.hpp>
#include <cassert>
#include <array>
#include <cmath>

#define hdr_min( x, y ) \
    ( ( x ) > ( y ) ? ( y ) : ( x ) )

#define hdr_max( x, y ) \
    ( ( x ) > ( y ) ? ( x ) : ( y ) )

#define in_range( x, y, z ) \
    hdr_min( hdr_max( x, y ), z )

namespace details {

// Copy n values from the parameter pack to an output iterator
template < typename OutputIterator >
void copy_n( size_t, OutputIterator )
{
}

template < typename OutputIterator, typename T, typename... Args >
void copy_n( size_t n, OutputIterator out, T const & value, Args... args )
{
  if ( n > 0 ) {
    *out = value;
    copy_n( n - 1, ++out, args... );
  }
}

// Copy n values from the parameter pack to an output iterator, starting at
// the "beginth" element
template < typename OutputIterator >
void copy_range( size_t, size_t, OutputIterator )
{
}

template < typename OutputIterator, typename T, typename... Args >
void copy_range( 
    size_t begin, size_t size, OutputIterator out, T value, Args... args
) {
    if ( begin == 0 ) {
        copy_n( size, out, value, args... );
    }
    else {
        copy_range( begin - 1, size, out, args... );
    }
}

} // namespace details

namespace hdr {

typedef struct {
    /* Define a block of pixel to vectorize code. 
     * AVX format
     */
    float r[ 8 ];
    float g[ 8 ];
    float b[ 8 ];
} pixelBlock;

enum pfmEndianess : int32_t {
    PFM_LITTLE_ENDIAN = -1,
    PFM_BIG_ENDIAN    =  1
};

enum saveFormat : uint32_t {
    ASCIIBitmap    = '1',
    ASCIIGreymap   = '2',
    ASCIIColormap  = '3',
    BinaryBitmap   = '4',
    BinaryGreymap  = '5',
    BinaryColormap = '6'
};
class image {

/* Row major matrix of pixelBlock */
public:
    image( void ) noexcept;
    image( uint32_t, uint32_t, uint32_t = 1 ) noexcept;
    image( image const & ) noexcept;

    // move constructor
    // overload operator= (move)

    image & operator= ( image const & ) noexcept;

    image & create( uint32_t, uint32_t, uint32_t = 1 ) noexcept;
    image & copy  ( image const & ) noexcept;

    INLINE uint32_t getWidth     ( void ) const noexcept;
    INLINE uint32_t getHeight    ( void ) const noexcept;
    INLINE size_t   getPadd      ( void ) const noexcept;

    INLINE bool isEmpty( void ) const noexcept;

    void free ( void ) noexcept;

    void normalise( float = 1 ) noexcept;
    void troncate( float, float ) noexcept;
    void gamma( float ) noexcept;

    int32_t loadPNM( 
        std::string const &
    ) noexcept;
    int32_t savePNM( 
        std::string const &, saveFormat = BinaryColormap 
    ) const noexcept;

    int32_t loadPFM( 
        std::string const &
    ) noexcept;
    int32_t savePFM( 
        std::string const &, saveFormat = BinaryColormap 
    ) const noexcept;

    template < class CWF, class... Args >
    int32_t createHDR( const Args&... args ) noexcept;

    void linearToneMap( uint32_t stops ) noexcept;

    ~image( void ) noexcept;

protected:
    void alloc( uint32_t, uint32_t ) noexcept;

private:
    uint32_t      m_width;
    uint32_t      m_height;
    size_t        m_padd;
    uint32_t      m_max_pixel_chanel;
    pixelBlock**  m_data_2D;
    pixelBlock*   m_data_1D;
};

class hinge_cwf {
public:

    hinge_cwf( void ) noexcept  = default;
    hinge_cwf( hinge_cwf const & ) = default;
    ~hinge_cwf( void ) noexcept = default;

    INLINE float operator( ) ( float x ) {
        if      ( x < 0   ) return 0;
        else if ( x < 0.5 ) return 2 * x;
        else if ( x < 1   ) return 2 * ( 1 - x );
        else                return 0;
    }

private:
};

uint32_t 
image::getWidth( void ) const noexcept
{
    return m_width;
}

uint32_t 
image::getHeight( void ) const noexcept
{
    return m_height;
}

size_t 
image::getPadd( void ) const noexcept
{
    return m_padd;
}

bool 
image::isEmpty( void ) const noexcept 
{
    return m_data_1D == nullptr;
}

template < class CWF, class... Args >
int32_t 
image::createHDR( Args const &... args ) noexcept {

    uint32_t constexpr N( sizeof... ( Args ) );
    std::array< hdr::image, N > image_set;
    CWF w;

    details::copy_range( 0, N, image_set.begin( ), args... );

    uint32_t width  = image_set[ 0 ].m_width;
    uint32_t height = image_set[ 0 ].m_height;
    for ( uint32_t l = 0; l < N; ++l ) {
        if (
            ( image_set[ l ].m_width != width ) ||
            ( image_set[ l ].m_height != height )
        ) {
            return -1;
        }
    }
    uint32_t width_block_number( ( width - 1 ) / 8 );
    uint32_t width_block_end( width_block_number * 8 );

    create( width, height, 1 );

#define HDR_MERGE_BLOCK( \
    pix_buf, \
    acc_wred, acc_wgreen, acc_wblue, acc_red, \
    acc_green, acc_blue, exposure_time \
) \
    do { \
        float w_temp_red   = w( pix_buf[ 0 ] ); \
        float w_temp_green = w( pix_buf[ 1 ] ); \
        float w_temp_blue  = w( pix_buf[ 2 ] ); \
        acc_wred   += w_temp_red;   \
        acc_wgreen += w_temp_green; \
        acc_wblue  += w_temp_blue;  \
        acc_red    += std::log( pix_buf[ 0 ] / exposure_time ) * w_temp_red;   \
        acc_green  += std::log( pix_buf[ 1 ] / exposure_time ) * w_temp_green; \
        acc_blue   += std::log( pix_buf[ 2 ] / exposure_time ) * w_temp_blue;  \
    } while( 0 )

#pragma omp parallel for
    for ( uint32_t i = 0; i < height; ++i ) {
        for ( uint32_t j = 0; j < width_block_number; ++j ) {
            float acc_red   [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 
            float acc_green [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_blue  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_wred  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_wgreen[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_wblue [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            uint32_t exposure_time( 1 );
            for ( uint32_t l = 0; l < N; ++l ) {
                for ( uint32_t k = 0; k < 8; ++k ) {
                    float pix_buf[ 3 ] = {
                        image_set[ l ].m_data_2D[ i ][ j ].r[ k ],
                        image_set[ l ].m_data_2D[ i ][ j ].g[ k ],
                        image_set[ l ].m_data_2D[ i ][ j ].b[ k ]
                    };
                    HDR_MERGE_BLOCK( 
                        pix_buf, 
                        acc_wred[ k ], acc_wgreen[ k ], acc_wblue[ k ], 
                        acc_red[ k ],  acc_green[ k ], acc_blue[ k ], 
                        exposure_time 
                    );
                }
                exposure_time <<= 2;
            }
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] = 
                    std::exp( acc_red  [ k ] / acc_wred  [ k ] );
                m_data_2D[ i ][ j ].g[ k ] =
                    std::exp( acc_green[ k ] / acc_wgreen[ k ] );
                m_data_2D[ i ][ j ].b[ k ] = 
                    std::exp( acc_blue [ k ] / acc_wblue [ k ] );
            }
        }
        float acc_red   [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 
        float acc_green [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float acc_blue  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float acc_wred  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float acc_wgreen[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float acc_wblue [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        uint32_t exposure_time( 1 );
        for ( uint32_t l = 0; l < N; ++l ) {
            for ( uint32_t k = 0; width_block_end + k < width; ++k ) {
                float pix_buf[ 3 ] = {
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].r[ k ],
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].g[ k ],
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].b[ k ]
                };
                HDR_MERGE_BLOCK( 
                    pix_buf, 
                    acc_wred[ k ], acc_wgreen[ k ], acc_wblue[ k ], 
                    acc_red[ k ], acc_green[ k ], acc_blue[ k ], 
                    exposure_time 
                );            
            }
            exposure_time <<= 2;
        }
        for ( uint32_t k = 0; width_block_end + k < width; ++k ) {
            m_data_2D[ i ][ width_block_number ].r[ k ] =
                std::exp( acc_red   [ k ]/ acc_wred  [ k ] );
            m_data_2D[ i ][ width_block_number ].g[ k ] = 
                std::exp( acc_green [ k ]/ acc_wgreen[ k ] );
            m_data_2D[ i ][ width_block_number ].b[ k ] =
                std::exp( acc_blue  [ k ]/ acc_wblue [ k ] );
        }
    }
    return 0;
}

} // namespace hdr

#endif