#include <Sphere.hpp>

namespace obj {

sphere::sphere( void ) noexcept
{
    m_radius = 0;
    m_center.clear( );
}

sphere::sphere( uint32_t radius, vect< uint32_t, 3 > const & p ) noexcept
{
    m_radius = radius;
    m_center = p;
}

sphere::sphere(
    uint32_t radius, uint32_t center_x, uint32_t center_y, uint32_t center_z
) noexcept {
    m_radius = radius;
    m_center[ 0 ] = center_x;
    m_center[ 1 ] = center_y;
    m_center[ 2 ] = center_z;
}

sphere::~sphere( void ) noexcept
{

}



} //namespace obj
