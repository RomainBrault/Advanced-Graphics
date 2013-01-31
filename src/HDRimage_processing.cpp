#include <HDRimage.hpp>

namespace hdr {

void 
image::normalise( float n ) noexcept
{
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    updateMaxChanel( );
    /* conditioning ?? */
    float mlt = n / m_max_pixel_chanel;
    m_max_pixel_chanel = n;

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            /* Inner loop vectorized with gcc ! */
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] *= mlt;
                m_data_2D[ i ][ j ].g[ k ] *= mlt;
                m_data_2D[ i ][ j ].b[ k ] *= mlt;
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] *= mlt;
            m_data_2D[ i ][ wblock_index ].g[ k ] *= mlt;
            m_data_2D[ i ][ wblock_index ].b[ k ] *= mlt;
        }
    }
}

float
image::maxPixelValue( void ) const noexcept {
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    float max_red   = 0;
    float max_green = 0;
    float max_blue  = 0;

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
image::troncate( float min, float max ) noexcept
{
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );

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
image::linearToneMap( uint32_t stops ) noexcept
{
    /* conditioning ?? */
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    uint32_t mlt = ( 1 << ( stops - 1 ) );

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        /* Inner loop vectorized with gcc ! */
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] *= mlt;
                m_data_2D[ i ][ j ].g[ k ] *= mlt;
                m_data_2D[ i ][ j ].b[ k ] *= mlt ;
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] *= mlt;
            m_data_2D[ i ][ wblock_index ].g[ k ] *= mlt;
            m_data_2D[ i ][ wblock_index ].b[ k ] *= mlt;
        }
    }
}

void 
image::gamma( float pow_val ) noexcept
{
    /* conditioning ?? */
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    float gamma_val( 1/ pow_val );

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

}