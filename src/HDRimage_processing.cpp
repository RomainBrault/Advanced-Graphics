#include <HDRimage.hpp>

#define hdr_in_range( x, y, z ) \
    std::min( std::max( x, y ), z )

namespace hdr {

void
image::normalise( float n ) noexcept
{
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    /* conditioning ?? */
    float mlt = n / m_max_pixel_chanel;
    m_max_pixel_chanel = n;
    m_min_pixel_chanel = 0;

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            /* Inner loop vectorized with gcc ! */
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] =
                    ( m_data_2D[ i ][ j ].r[ k ] - m_min_pixel_chanel ) * mlt;
                m_data_2D[ i ][ j ].g[ k ] =
                    ( m_data_2D[ i ][ j ].g[ k ] - m_min_pixel_chanel ) * mlt;
                m_data_2D[ i ][ j ].b[ k ] =
                    ( m_data_2D[ i ][ j ].b[ k ] - m_min_pixel_chanel ) * mlt;
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] =
                ( m_data_2D[ i ][ wblock_index ].r[ k ] - m_min_pixel_chanel )
                * mlt;
            m_data_2D[ i ][ wblock_index ].g[ k ] =
                ( m_data_2D[ i ][ wblock_index ].g[ k ] - m_min_pixel_chanel )
                * mlt;
            m_data_2D[ i ][ wblock_index ].b[ k ] =
                ( m_data_2D[ i ][ wblock_index ].b[ k ] - m_min_pixel_chanel )
                * mlt;
        }
    }
}

void
image::plot4Points( uint32_t cx, uint32_t cy, uint32_t x, uint32_t y ) noexcept
{
    setPixel( cx + x, cy + y, 0, 0, 0 );
    if ( x != 0 ) setPixel( cx - x, cy + y, 0, 0, 0 );
    if ( y != 0 ) setPixel( cx + x, cy - y, 0, 0, 0 );
    setPixel( cx - x, cy - y, 0, 0, 0 );
}

void
image::plot8Points( uint32_t cx, uint32_t cy, uint32_t x, uint32_t y ) noexcept
{
    plot4Points( cx, cy, x, y );
    if ( x != y ) plot4Points( cx, cy, y, x );
}

void
image::plot2Lines( uint32_t cx, uint32_t cy, uint32_t x, uint32_t y ) noexcept
{
    line( cx + x, cy + y, cx - x, cy + y );
    line( cx + x, cy - y, cx - x, cy - y );
}

void
image::plot4Lines( uint32_t cx, uint32_t cy, uint32_t x, uint32_t y ) noexcept
{
    plot2Lines( cx, cy, x, y );
    plot2Lines( cx, cy, y, x );
}

void
image::circle( uint32_t cx, uint32_t cy, uint32_t radius ) noexcept
{
    if ( radius == 0 ) {
        return;
    }

    int32_t error = -radius;
    uint32_t x = radius;
    uint32_t y = 0;

    while ( x > y )
    {
        plot8Points(cx, cy, x, y);

        error += y;
        ++y;
        error += y;
        if ( error >= 0 )
        {
            error -= x;
            --x;
            error -= x;
        }
    }
    plot4Points(cx, cy, x, y);
}

void
image::circleFilled( uint32_t cx, uint32_t cy, uint32_t radius ) noexcept
{
    if ( radius == 0 ) {
        return;
    }

    int32_t error = -radius;
    uint32_t x = radius;
    uint32_t y = 0;

    while ( x > y )
    {
        plot4Lines(cx, cy, x, y);

        error += y;
        ++y;
        error += y;
        if ( error >= 0 )
        {
            error -= x;
            --x;
            error -= x;
        }
    }
    plot2Lines(cx, cy, x, y);
}

void
image::line( uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2 ) noexcept
{
    int32_t F;
    uint32_t x, y;
    if ( x1 > x2 ) { // Swap points if p1 is on the right of p2
        std::swap( x1, x2 );
        std::swap( y1, y2 );
    }

    /* Handle trivial cases separately for algorithm speed up.
     * Trivial case 1: m = +/-INF (Vertical line) */
    if ( x1 == x2 ) {
        if ( y1 > y2 ) std::swap( y1, y2 );

        x = x1;
        y = y1;
        while ( y <= y2 ) {
            setPixel( x, y, 0, 0, 0 );
            y++;
        }
        return;
    }
    // Trivial case 2: m = 0 (Horizontal line)
    else if ( y1 == y2 ) {
        x = x1;
        y = y1;

        while ( x <= x2 ) {
            setPixel( x, y, 0, 0, 0 );
            x++;
        }
        return;
    }

    int32_t dy            = y2 - y1;  // y-increment from p1 to p2
    int32_t dx            = x2 - x1;  // x-increment from p1 to p2
    int32_t dy2           = ( dy << 1 );  // dy << 1 == 2*dy
    int32_t dx2           = ( dx << 1 );
    int32_t dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
    int32_t dy2_plus_dx2  = dy2 + dx2;


    if ( dy >= 0 ) { // m >= 0
        /* Case 1: 0 <= m <= 1 (Original case) */
        if ( dy <= dx ) {
            F = dy2 - dx; // initial F
            x = x1;
            y = y1;
            while ( x <= x2 ) {
                setPixel( x, y, 0, 0, 0 );
                if ( F <= 0 ) F += dy2;
                else {
                    ++y;
                    F += dy2_minus_dx2;
                }
                ++x;
            }
        }
        /* Case 2: 1 < m < INF (Mirror about y=x line
         * replace all dy by dx and dx by dy) */
        else {
            F = dx2 - dy; // initial F
            y = y1;
            x = x1;
            while ( y <= y2 ) {
                setPixel( x, y, 0, 0, 0 );
                if ( F <= 0 ) F += dx2;
                else {
                    ++x;
                    F -= dy2_minus_dx2;
                }
                ++y;
            }
        }
    }
    else { // m < 0
        /* Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy) */
        if ( dx >= -dy ) {
            F = -dy2 - dx; // initial F
            x = x1;
            y = y1;
            while ( x <= x2 ) {
                setPixel( x, y, 0, 0, 0 );
                if ( F <= 0 ) F -= dy2;
                else {
                    y--;
                    F -= dy2_plus_dx2;
                }
                ++x;
            }
        }
        /* Case 4: -INF < m < -1 (Mirror about x-axis and mirror
         * about y=x line, replace all dx by -dy and dy by dx) */
        else {
            F = dx2 + dy;    // initial F
            y = y1;
            x = x1;
            while ( y >= y2 ) {
                setPixel( x, y, 0, 0, 0 );
                if ( F <= 0 ) F += dx2;
                else {
                    ++x;
                    F += dy2_plus_dx2;
                }
                --y;
            }
        }
    }
}

void
image::negatif( void ) noexcept
{
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            /* Inner loop vectorized with gcc ! */
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] =
                    m_max_pixel_chanel - m_data_2D[ i ][ j ].r[ k ];
                m_data_2D[ i ][ j ].g[ k ] =
                    m_max_pixel_chanel - m_data_2D[ i ][ j ].g[ k ];
                m_data_2D[ i ][ j ].b[ k ] =
                    m_max_pixel_chanel - m_data_2D[ i ][ j ].b[ k ];
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] =
                m_max_pixel_chanel - m_data_2D[ i ][ wblock_index ].r[ k ];
            m_data_2D[ i ][ wblock_index ].g[ k ] =
                m_max_pixel_chanel - m_data_2D[ i ][ wblock_index ].g[ k ];
            m_data_2D[ i ][ wblock_index ].b[ k ] =
                m_max_pixel_chanel - m_data_2D[ i ][ wblock_index ].b[ k ];
        }
    }
}

void
image::fill( float val ) noexcept
{
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    if ( val > m_max_pixel_chanel ) {
        m_max_pixel_chanel = val;
    }
    else if ( val < m_min_pixel_chanel ) {
        m_min_pixel_chanel = val;
    }

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            /* Inner loop vectorized with gcc ! */
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] = val;
                m_data_2D[ i ][ j ].g[ k ] = val;
                m_data_2D[ i ][ j ].b[ k ] = val;
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] = val;
            m_data_2D[ i ][ wblock_index ].g[ k ] = val;
            m_data_2D[ i ][ wblock_index ].b[ k ] = val;
        }
    }
}

float
image::maxPixelValue( void ) const noexcept {
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    float max_red   = m_min_pixel_chanel - 1;
    float max_green = m_min_pixel_chanel - 1;
    float max_blue  = m_min_pixel_chanel - 1;

    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                if ( m_data_2D[ i ][ j ].r[ k ] > max_red   )
                    max_red   = m_data_2D[ i ][ j ].r[ k ];
                if ( m_data_2D[ i ][ j ].g[ k ] > max_green )
                    max_green = m_data_2D[ i ][ j ].g[ k ];
                if ( m_data_2D[ i ][ j ].b[ k ] > max_blue  )
                    max_blue  = m_data_2D[ i ][ j ].b[ k ];
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            if ( m_data_2D[ i ][ wblock_index ].r[ k ] > max_red   )
                max_red   = m_data_2D[ i ][ wblock_index ].r[ k ];
            if ( m_data_2D[ i ][ wblock_index ].g[ k ] > max_green )
                max_green = m_data_2D[ i ][ wblock_index ].g[ k ];
            if ( m_data_2D[ i ][ wblock_index ].b[ k ] > max_blue  )
                max_blue  = m_data_2D[ i ][ wblock_index ].b[ k ];
        }
    }
    return std::max( std::max( max_red, max_green ), max_blue );
}

float
image::minPixelValue( void ) const noexcept {
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    float min_red   = m_max_pixel_chanel + 1;
    float min_green = m_max_pixel_chanel + 1;
    float min_blue  = m_max_pixel_chanel + 1;

    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                if ( m_data_2D[ i ][ j ].r[ k ] < min_red   )
                    min_red   = m_data_2D[ i ][ j ].r[ k ];
                if ( m_data_2D[ i ][ j ].g[ k ] < min_green )
                    min_green = m_data_2D[ i ][ j ].g[ k ];
                if ( m_data_2D[ i ][ j ].b[ k ] < min_blue  )
                    min_blue  = m_data_2D[ i ][ j ].b[ k ];
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            if ( m_data_2D[ i ][ wblock_index ].r[ k ] < min_red   )
                min_red   = m_data_2D[ i ][ wblock_index ].r[ k ];
            if ( m_data_2D[ i ][ wblock_index ].g[ k ] < min_green )
                min_green = m_data_2D[ i ][ wblock_index ].g[ k ];
            if ( m_data_2D[ i ][ wblock_index ].b[ k ] < min_blue  )
                min_blue  = m_data_2D[ i ][ wblock_index ].b[ k ];
        }
    }
    return std::min( std::min( min_red, min_green ), min_blue );
}

float
image::dynamicRange( void ) const noexcept
{
    return maxPixelValue( ) / minPixelValue( );
}

void
image::updateMaxChanel( void ) noexcept
{
    m_max_pixel_chanel = maxPixelValue( );
}

void
image::updateMinChanel( void ) noexcept
{
    m_min_pixel_chanel = minPixelValue( );
}

void
image::troncate( float min, float max ) noexcept
{
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    m_max_pixel_chanel = max;
    m_min_pixel_chanel = min;

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] =
                    hdr_in_range( m_data_2D[ i ][ j ].r[ k ], min, max );
                m_data_2D[ i ][ j ].g[ k ] =
                    hdr_in_range( m_data_2D[ i ][ j ].g[ k ], min, max );
                m_data_2D[ i ][ j ].b[ k ] =
                    hdr_in_range( m_data_2D[ i ][ j ].b[ k ], min, max );
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] =
                hdr_in_range( m_data_2D[ i ][ wblock_index ].r[ k ], min, max );
            m_data_2D[ i ][ wblock_index ].g[ k ] =
                hdr_in_range( m_data_2D[ i ][ wblock_index ].g[ k ], min, max );
            m_data_2D[ i ][ wblock_index ].b[ k ] =
                hdr_in_range( m_data_2D[ i ][ wblock_index ].b[ k ], min, max );
        }
    }
}

void
image::linearToneMap( float stops ) noexcept
{
    normalise( std::pow( 2, stops ) );
    troncate( 0, 1 );
}

void
image::gamma( float pow_val ) noexcept
{
    /* conditioning ?? */
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    float gamma_val( 1/ pow_val );
    m_max_pixel_chanel = std::pow( m_max_pixel_chanel, gamma_val );
    m_min_pixel_chanel = std::pow( m_min_pixel_chanel, gamma_val );

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] =
                    std::pow( m_data_2D[ i ][ j ].r[ k ], gamma_val );
                m_data_2D[ i ][ j ].g[ k ] =
                    std::pow( m_data_2D[ i ][ j ].g[ k ], gamma_val );
                m_data_2D[ i ][ j ].b[ k ] =
                    std::pow( m_data_2D[ i ][ j ].b[ k ], gamma_val );
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] =
                std::pow( m_data_2D[ i ][ wblock_index ].r[ k ], gamma_val );
            m_data_2D[ i ][ wblock_index ].g[ k ] =
                std::pow( m_data_2D[ i ][ wblock_index ].g[ k ], gamma_val );
            m_data_2D[ i ][ wblock_index ].b[ k ] =
                std::pow( m_data_2D[ i ][ wblock_index ].b[ k ], gamma_val );
        }
    }
}

void
image::histEqToneMap( uint32_t H_SIZE ) noexcept
{
    float const exp_rate( ( H_SIZE - 1 ) / m_max_pixel_chanel );

    double* hist = new double[ 6 * H_SIZE ];
    std::memset( hist, 6 * H_SIZE * sizeof ( double ), 0 );

    double* hist_red     = hist;
    double* hist_green   = hist + 1 * H_SIZE;
    double* hist_blue    = hist + 2 * H_SIZE;
    double* hist_red_s   = hist + 3 * H_SIZE;
    double* hist_green_s = hist + 4 * H_SIZE;
    double* hist_blue_s  = hist + 5 * H_SIZE;

    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                hist_red  [ static_cast< uint32_t >(
                    ( m_data_2D[ i ][ j ].r[ k ] - m_min_pixel_chanel )
                    * exp_rate
                ) ]++;
                hist_green[ static_cast< uint32_t >(
                    ( m_data_2D[ i ][ j ].g[ k ] - m_min_pixel_chanel )
                    * exp_rate
                ) ]++;
                hist_blue [ static_cast< uint32_t >(
                    ( m_data_2D[ i ][ j ].b[ k ] - m_min_pixel_chanel )
                    * exp_rate
                ) ]++;
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            hist_red  [ static_cast< uint32_t >(
                ( m_data_2D[ i ][ wblock_index ].r[ k ] - m_min_pixel_chanel )
                 * exp_rate
            ) ]++;
            hist_green[ static_cast< uint32_t >(
                ( m_data_2D[ i ][ wblock_index ].g[ k ] - m_min_pixel_chanel )
                 * exp_rate
            ) ]++;
            hist_blue [ static_cast< uint32_t >(
                ( m_data_2D[ i ][ wblock_index ].b[ k ] - m_min_pixel_chanel )
                 * exp_rate
            ) ]++;
        }
    }

    double const N( m_width * m_height );
    for ( uint32_t i = 0; i < H_SIZE; ++i ) {
        hist_red  [ i ] /= N;
        hist_green[ i ] /= N;
        hist_blue [ i ] /= N;
    }

    hist_red_s  [ 0 ] = hist_red  [ 0 ];
    hist_green_s[ 0 ] = hist_green[ 0 ];
    hist_blue_s [ 0 ] = hist_blue [ 0 ];
    for ( uint32_t i = 1; i < H_SIZE; ++i ) {
        hist_red_s  [ i ] = hist_red_s  [ i - 1 ] + hist_red  [ i ];
        hist_green_s[ i ] = hist_green_s[ i - 1 ] + hist_green[ i ];
        hist_blue_s [ i ] = hist_blue_s [ i - 1 ] + hist_blue [ i ];
    }

    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] = m_max_pixel_chanel * (
                    hist_red_s  [ static_cast< uint32_t >(
                        ( m_data_2D[ i ][ j ].r[ k ] - m_min_pixel_chanel )
                        * exp_rate
                    ) ]
                    + m_min_pixel_chanel
                );
                m_data_2D[ i ][ j ].g[ k ] = m_max_pixel_chanel * (
                    hist_green_s[ static_cast< uint32_t >(
                        ( m_data_2D[ i ][ j ].g[ k ] - m_min_pixel_chanel )
                        * exp_rate
                    ) ]
                    + m_min_pixel_chanel
                );
                m_data_2D[ i ][ j ].b[ k ] = m_max_pixel_chanel * (
                    hist_blue_s [ static_cast< uint32_t >(
                        ( m_data_2D[ i ][ j ].b[ k ] - m_min_pixel_chanel )
                        * exp_rate
                    ) ]
                    + m_min_pixel_chanel
                );
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] = m_max_pixel_chanel * (
                hist_red_s  [ static_cast< uint32_t >( (
                    m_data_2D[ i ][ wblock_index ].r[ k ] - m_min_pixel_chanel
                    ) * exp_rate
                ) ]
                + m_min_pixel_chanel
            );
            m_data_2D[ i ][ wblock_index ].g[ k ] = m_max_pixel_chanel * (
                hist_green_s[ static_cast< uint32_t >( (
                    m_data_2D[ i ][ wblock_index ].g[ k ] - m_min_pixel_chanel
                    ) * exp_rate
                ) ]
                + m_min_pixel_chanel
            );
            m_data_2D[ i ][ wblock_index ].b[ k ] = m_max_pixel_chanel * (
                hist_blue_s [ static_cast< uint32_t >( (
                    m_data_2D[ i ][ wblock_index ].b[ k ] - m_min_pixel_chanel
                    ) * exp_rate
            ) ]
                + m_min_pixel_chanel
            );
        }
    }

}

}