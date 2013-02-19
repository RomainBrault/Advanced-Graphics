#include <Rand.hpp>

namespace rnd {

xorShift::xorShift( void ) noexcept {
    m_x = 123456789;
    m_y = 362436069;
    m_z = 521288629;
    m_w = 88675123;
}

xorShift::xorShift( uint32_t ) noexcept {
    m_x = 123456789;
    m_y = 362436069;
    m_z = 521288629;
    m_w = 88675123;

}

xorShift::~xorShift( void ) noexcept {

}

xorShift::xorShift( xorShift const & rng ) noexcept {
    m_x = rng.m_x;
    m_y = rng.m_y;
    m_z = rng.m_z;
}

uint32_t
xorShift::rand( void ) noexcept {
    uint32_t t = m_x ^ ( m_x << 15 );
    m_x = m_y;
    m_y = m_z;
    m_z = m_w;
    m_w = ( m_w ^ ( m_w >> 21 ) ) ^ ( t ^ ( t >> 4 ) );

    return m_z;
}

void
xorShift::skip( uint32_t n ) noexcept {
    for ( uint32_t i = 0; i < n; ++i ) {
        uint32_t t = m_x ^ ( m_x << 15 );
        m_x = m_y;
        m_y = m_z;
        m_z = m_w;
        m_w = ( m_w ^ ( m_w >> 21 ) ) ^ ( t ^ ( t >> 4 ) );
    }
}

} // namespace rnd
