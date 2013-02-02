#ifndef HDRIMAGE_HPP_INCLUDED
#define HDRIMAGE_HPP_INCLUDED

#include <AdvancedGraphicsConfig.hpp>

namespace details {

// Copy n values from the parameter pack to an output iterator
template < typename OutputIterator >
void move_n( size_t, OutputIterator )
{
}

template < typename OutputIterator, typename T, typename... Args >
void move_n( size_t n, OutputIterator out, T & value, Args... args )
{
  if ( n > 0 ) {
    *out = std::move( value );
    move_n( n - 1, ++out, args... );
  }
}

// Copy n values from the parameter pack to an output iterator, starting at
// the "beginth" element
template < typename OutputIterator >
void move_range( size_t, size_t, OutputIterator )
{
}

template < typename OutputIterator, typename T, typename... Args >
void move_range(
    size_t begin, size_t size, OutputIterator out, T value, Args... args
) {
    if ( begin == 0 ) {
        move_n( size, out, value, args... );
    }
    else {
        move_range( begin - 1, size, out, args... );
    }
}

template < class CWF >
static INLINE void
merge_block(
    float pix_red, float pix_green, float pix_blue,
    float & acc_wred, float & acc_wgreen, float & acc_wblue,
    float & acc_red , float & acc_green , float & acc_blue,
    uint32_t exposure_time
) noexcept {

    CWF w;
    if ( ( pix_red > 0.005 ) && ( pix_red < 0.92 ) ) {
        float w_red = w( pix_red );
        acc_red  += std::log( pix_red / exposure_time ) * w_red;
        acc_wred += w_red;
    }
    if ( ( pix_green > 0.005 ) && ( pix_green < 0.92 ) ) {
        float w_green = w( pix_green );
        acc_green  += std::log( pix_green / exposure_time ) * w_green;
        acc_wgreen += w_green;
    }
    if ( ( pix_blue > 0.005 ) && ( pix_blue < 0.92 ) ) {
        float w_blue = w( pix_blue );
        acc_blue  += std::log( pix_blue / exposure_time ) * w_blue;
        acc_wblue += w_blue;
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
    image( uint32_t, uint32_t, float = 1 ) noexcept;
    image( image const &  ) noexcept;
    image( image       && ) noexcept;

    image & operator= ( image const &  ) noexcept;
    image & operator= ( image       && ) noexcept;

    image & create( uint32_t, uint32_t, float = 1 ) noexcept;
    image & copy  ( image const & ) noexcept;

    image & swap( image & ) noexcept;

    INLINE uint32_t getWidth     ( void ) const noexcept;
    INLINE uint32_t getHeight    ( void ) const noexcept;
    INLINE size_t   getPadd      ( void ) const noexcept;
    INLINE float    getMaxChanel ( void ) const noexcept;
    INLINE float    getMinChanel ( void ) const noexcept;

    INLINE void setMaxChanel( float ) noexcept;
    INLINE void setMinChanel( float ) noexcept;

    void updateMaxChanel( void ) noexcept;
    void updateMinChanel( void ) noexcept;

    INLINE bool isEmpty( void ) const noexcept;

    void free ( void ) noexcept;

    void fill     ( float val      ) noexcept;
    void normalise( float = 1      ) noexcept;
    void troncate ( float, float   ) noexcept;
    void gamma    ( float          ) noexcept;

    float maxPixelValue( void ) const noexcept;
    float minPixelValue( void ) const noexcept;
    float dynamicRange ( void ) const noexcept;

    INLINE void setPixel( uint32_t, uint32_t, float, float, float ) noexcept;
           void circle  ( uint32_t, uint32_t, uint32_t            ) noexcept;
           void line    ( uint32_t, uint32_t, uint32_t, uint32_t  ) noexcept;
           void negatif ( void                                    ) noexcept;

           void circleFilled( uint32_t, uint32_t, uint32_t ) noexcept;

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

    void linearToneMap( float stops    ) noexcept;
    void histEqToneMap( uint32_t = 256 ) noexcept;

    ~image( void ) noexcept;

protected:
    void alloc( uint32_t, uint32_t ) noexcept;

    void plot8Points( uint32_t, uint32_t, uint32_t, uint32_t ) noexcept;
    void plot4Points( uint32_t, uint32_t, uint32_t, uint32_t ) noexcept;

    void plot4Lines ( uint32_t, uint32_t, uint32_t, uint32_t ) noexcept;
    void plot2Lines ( uint32_t, uint32_t, uint32_t, uint32_t ) noexcept;

private:
    uint32_t      m_width;
    uint32_t      m_height;
    size_t        m_padd;
    float         m_max_pixel_chanel;
    float         m_min_pixel_chanel;
    pixelBlock**  m_data_2D;
    pixelBlock*   m_data_1D;
};

class pol_cwf {
public:

    pol_cwf ( void ) = default;
    pol_cwf ( pol_cwf const & ) = default;
    ~pol_cwf( void ) = default;

    INLINE float operator( ) ( float x ) {
        float center_x = 1 - x;
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

float
image::getMinChanel( void ) const noexcept
{
    return m_min_pixel_chanel;
}

void
image::setMinChanel( float val ) noexcept
{
    m_min_pixel_chanel = val;
}

void
image::setPixel( uint32_t x, uint32_t y, float r, float g, float b ) noexcept
{
    uint32_t x_block = x / 8;
    uint32_t x_index = x % 8;
    m_data_2D[ y ][ x_block ].r[ x_index ] = r;
    m_data_2D[ y ][ x_block ].g[ x_index ] = g;
    m_data_2D[ y ][ x_block ].b[ x_index ] = b;
}

template < class CWF, class... Args >
int32_t
image::createHDR( Args const &... args ) noexcept {

    uint32_t constexpr N( sizeof... ( Args ) );
    std::array< hdr::image, N > image_set;

    details::move_range( 0, N, image_set.begin( ), args... );

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
            float acc_wred  [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_wgreen[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            float acc_wblue [ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            uint32_t exposure_time( 1 );
            for ( uint32_t l = 0; l < N; ++l ) {
                for ( uint32_t k = 0; k < 8; ++k ) {
                    details::merge_block< CWF >(
                        image_set[ l ].m_data_2D[ i ][ j ].r[ k ],
                        image_set[ l ].m_data_2D[ i ][ j ].g[ k ],
                        image_set[ l ].m_data_2D[ i ][ j ].b[ k ],
                        acc_wred[ k ], acc_wgreen[ k ], acc_wblue[ k ],
                        acc_red [ k ], acc_green [ k ], acc_blue [ k ],
                        exposure_time
                    );
                }
                exposure_time <<= 2;
            }
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] =
                    ( acc_wred  [ k ] == 0 ) ?
                    m_max_pixel_chanel :
                    std::exp( acc_red  [ k ] / acc_wred  [ k ] );
                m_data_2D[ i ][ j ].g[ k ] =
                    ( acc_wgreen[ k ] == 0 ) ?
                    m_max_pixel_chanel :
                    std::exp( acc_green[ k ] / acc_wgreen[ k ] );
                m_data_2D[ i ][ j ].b[ k ] =
                    ( acc_wblue[ k ] == 0 ) ?
                    m_max_pixel_chanel :
                    std::exp( acc_blue[ k ] / acc_wblue[ k ] );
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
                details::merge_block< CWF >(
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].r[ k ],
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].g[ k ],
                    image_set[ l ].m_data_2D[ i ][ width_block_number ].b[ k ],
                    acc_wred[ k ], acc_wgreen[ k ], acc_wblue[ k ],
                    acc_red [ k ], acc_green [ k ], acc_blue [ k ],
                    exposure_time
                );
            }
            exposure_time <<= 2;
        }
        for ( uint32_t k = 0; width_block_end + k < width; ++k ) {
                m_data_2D[ i ][ width_block_number ].r[ k ] =
                    ( acc_wred  [ k ] == 0 ) ?
                    m_max_pixel_chanel :
                    std::exp( acc_red  [ k ] / acc_wred[ k ] );
                m_data_2D[ i ][ width_block_number ].g[ k ] =
                    ( acc_wgreen[ k ] == 0 ) ?
                    m_max_pixel_chanel :
                    std::exp( acc_green[ k ] / acc_wgreen[ k ] );
                m_data_2D[ i ][ width_block_number ].b[ k ] =
                    ( acc_wblue[ k ] == 0 ) ?
                    m_max_pixel_chanel :
                    std::exp( acc_blue[ k ] / acc_wblue[ k ] );
        }
    }

    return 0;
}

} // namespace hdr

namespace std {

    static INLINE void
    swap( hdr::image & im1, hdr::image & im2 )
        noexcept(
            ( is_nothrow_move_constructible< hdr::image >::value ) &&
            ( is_nothrow_move_assignable< hdr::image >::value    )
        )
    {
        im1.swap( im2 );
    }

} // namespace std

#endif