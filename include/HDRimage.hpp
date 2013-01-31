#ifndef HDRIMAGE_HPP_INCLUDED
#define HDRIMAGE_HPP_INCLUDED

#include <HDRimageConfig.hpp>
#include <cassert>
#include <array>
#include <cmath>

#define hdr_in_range( x, y, z ) \
    std::min( std::max( x, y ), z )

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
    INLINE float    getMaxChanel ( void ) const noexcept;

    INLINE void setMaxChanel( float ) noexcept;

    void updateMaxChanel( void ) noexcept;

    INLINE bool isEmpty( void ) const noexcept;

    void free ( void ) noexcept;

    void normalise( float = 1    ) noexcept;
    void troncate ( float, float ) noexcept;
    void gamma    ( float        ) noexcept;

    float maxPixelValue( void ) const noexcept;
    float minPixelValue( void ) const noexcept;
    float dynamicRange ( void ) const noexcept;

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
    float         m_max_pixel_chanel;
    pixelBlock**  m_data_2D;
    pixelBlock*   m_data_1D;
};

class pol_cwf {
public:

    pol_cwf ( void ) noexcept  = default;
    pol_cwf ( pol_cwf const & ) = default;
    ~pol_cwf( void ) noexcept = default;

    INLINE float operator( ) ( float x ) {
        float center_x = x - 1;
        return 16 * x * x * center_x * center_x;
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

float
image::getMaxChanel( void ) const noexcept
{
    return m_max_pixel_chanel;
}

void
image::setMaxChanel( float val ) noexcept
{
    m_max_pixel_chanel = val;
}

template < class CWF >
static INLINE void
hdr_merge_block( 
    float pix_red, float pix_green, float pix_blue,
    float & acc_wgray,
    float & acc_red , float & acc_green, float & acc_blue, 
    uint32_t exposure_time
) noexcept {

    CWF w;
    float pix_grey       = ( pix_red + pix_green + pix_blue ) / 3;
    if ( ( pix_grey > 0.005 ) && ( pix_grey < 0.92 ) ) {
        float w_gray = w( pix_grey );
        float radiance_red   = pix_red   / exposure_time;
        float radiance_green = pix_green / exposure_time;
        float radiance_blue  = pix_blue  / exposure_time;
        acc_red   += std::log( radiance_red   ) * w_gray;
        acc_green += std::log( radiance_green ) * w_gray;
        acc_blue  += std::log( radiance_blue  ) * w_gray;
        acc_wgray += w_gray;
    }
}

template < class CWF, class... Args >
int32_t 
image::createHDR( Args const &... args ) noexcept {

    uint32_t constexpr N( sizeof... ( Args ) );
    std::array< hdr::image, N > image_set;

    details::copy_range( 0, N, image_set.begin( ), args... );

    uint32_t width  = image_set[ 0 ].m_width;
    uint32_t height = image_set[ 0 ].m_height;
    for ( uint32_t l = 1; l < N; ++l ) {
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

#pragma omp parallel for
    for ( uint32_t i = 0; i < height; ++i ) {
        for ( uint32_t j = 0; j < width_block_number; ++j ) {
            float acc_red   [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 
            float acc_green [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_blue  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_wgray [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            uint32_t exposure_time( 1 );
            for ( uint32_t l = 0; l < N; ++l ) {
                for ( uint32_t k = 0; k < 8; ++k ) {
                    hdr_merge_block< CWF >( 
                        image_set[ l ].m_data_2D[ i ][ j ].r[ k ],
                        image_set[ l ].m_data_2D[ i ][ j ].g[ k ],
                        image_set[ l ].m_data_2D[ i ][ j ].b[ k ],
                        acc_wgray[ k ],
                        acc_red [ k ], acc_green [ k ], acc_blue [ k ], 
                        exposure_time 
                    );
                }
                exposure_time <<= 2;
            }
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] = 
                    std::exp( acc_red  [ k ] / acc_wgray[ k ] );
                m_data_2D[ i ][ j ].g[ k ] =
                    std::exp( acc_green[ k ] / acc_wgray[ k ] );
                m_data_2D[ i ][ j ].b[ k ] = 
                    std::exp( acc_blue [ k ] / acc_wgray[ k ] );
            }
        }
        float acc_red   [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 
        float acc_green [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float acc_blue  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float acc_wgray [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        uint32_t exposure_time( 1 );
        for ( uint32_t l = 0; l < N; ++l ) {
            for ( uint32_t k = 0; width_block_end + k < width; ++k ) {
                hdr_merge_block< CWF >( 
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].r[ k ],
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].g[ k ],
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].b[ k ],
                    acc_wgray[ k ], 
                    acc_red [ k ], acc_green [ k ], acc_blue [ k ], 
                    exposure_time 
                );        
            }
            exposure_time <<= 2;
        }
        for ( uint32_t k = 0; width_block_end + k < width; ++k ) {
            m_data_2D[ i ][ width_block_number ].r[ k ] =
                std::exp( acc_red   [ k ]/ acc_wgray[ k ] );
            m_data_2D[ i ][ width_block_number ].g[ k ] = 
                std::exp( acc_green [ k ]/ acc_wgray[ k ] );
            m_data_2D[ i ][ width_block_number ].b[ k ] =
                std::exp( acc_blue  [ k ]/ acc_wgray[ k ] );
        }
    }

#undef HDR_MERGE_BLOCK

    return 0;
}

} // namespace hdr

#endif