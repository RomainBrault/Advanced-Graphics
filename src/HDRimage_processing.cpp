#include <HDRimage.hpp>

namespace hdr {

void 
image::normalise( float n ) noexcept
{
    /* conditioning ?? */
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    float mlt = n / m_max_pixel_chanel;
    m_max_pixel_chanel = n;

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        /* Inner loop vectorized with gcc ! */
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
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
                    in_range( m_data_2D[ i ][ j ].r[ k ], min, max );
                m_data_2D[ i ][ j ].g[ k ] = 
                    in_range( m_data_2D[ i ][ j ].g[ k ], min, max );
                m_data_2D[ i ][ j ].b[ k ] = 
                    in_range( m_data_2D[ i ][ j ].b[ k ], min, max );
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] = 
                in_range( m_data_2D[ i ][ wblock_index ].r[ k ], min, max );
            m_data_2D[ i ][ wblock_index ].g[ k ] = 
                in_range( m_data_2D[ i ][ wblock_index ].g[ k ], min, max );
            m_data_2D[ i ][ wblock_index ].b[ k ] = 
                in_range( m_data_2D[ i ][ wblock_index ].b[ k ], min, max );
        }
    }
}

void 
image::linearToneMap( uint32_t stops ) noexcept
{
    /* conditioning ?? */
    uint32_t wblock_index( ( m_width - 1 ) / 8 );
    uint32_t wblock_end( wblock_index * 8 );
    uint32_t mlt = ( 2 << ( stops - 1 ) );

#pragma omp parallel for
    for ( uint32_t i = 0; i < m_height; ++i ) {
        /* Inner loop vectorized with gcc ! */
        for ( uint32_t j = 0; j < wblock_index; ++j ) {
            for ( uint32_t k = 0; k < 8; ++k ) {
                m_data_2D[ i ][ j ].r[ k ] = 
                    m_data_2D[ i ][ j ].r[ k ] * mlt;
                m_data_2D[ i ][ j ].g[ k ] = 
                    m_data_2D[ i ][ j ].g[ k ] * mlt;
                m_data_2D[ i ][ j ].b[ k ] = 
                    m_data_2D[ i ][ j ].b[ k ] * mlt;
            }
        }
        for ( uint32_t k = 0; wblock_end + k < m_width; ++k ) {
            m_data_2D[ i ][ wblock_index ].r[ k ] = 
                m_data_2D[ i ][ wblock_index ].r[ k ] * mlt;
            m_data_2D[ i ][ wblock_index ].g[ k ] = 
                m_data_2D[ i ][ wblock_index ].g[ k ] * mlt;
            m_data_2D[ i ][ wblock_index ].b[ k ] = 
                m_data_2D[ i ][ wblock_index ].b[ k ] * mlt;
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