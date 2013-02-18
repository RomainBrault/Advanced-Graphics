#include <Rand.hpp>

namespace rnd {

xorShift::xorShift( void ) noexcept {
    m_x = 123456789;
    m_y = 362436069;
    m_z = 521288629;
}

xorShift::xorShift( uint32_t seed ) noexcept {
    m_x = 123456789;
    m_y = 362436069;
    m_z = 521288629;
    skip( seed & 0x0000FFFF );
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
    m_x ^= m_x << 16;
    m_x ^= m_x >> 5;
    m_x ^= m_x << 1;

    uint32_t t = m_x;
    m_x = m_z;
    m_y = m_z;
    m_z = t ^ m_x ^ m_y;

    return m_z;
}

void
xorShift::skip( uint32_t n ) noexcept {
    for ( uint32_t i = 0; i < n; ++i ) {
        m_x ^= m_x << 16;
        m_x ^= m_x >> 5;
        m_x ^= m_x << 1;

        uint32_t t = m_x;
        m_x = m_z;
        m_y = m_z;
        m_z = t ^ m_x ^ m_y;
    }
}

} // namespace rnd
