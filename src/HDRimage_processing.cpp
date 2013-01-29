#include <HDRimage.hpp>

namespace hdr {

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

}