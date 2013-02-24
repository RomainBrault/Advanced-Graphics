#include <BRDF.hpp>

namespace brdf {

    model::model(
        obj::vect< float, 3 > w0, float s, float ks, float kd
    ) noexcept {
        m_w0 = w0;
        m_s  = s;
        m_ks = ks;
        m_kd = kd;
    }
    model::model(
        float x, float y, float z, float s, float ks, float kd
    ) noexcept {
        m_w0[ 0 ] = x;
        m_w0[ 1 ] = y;
        m_w0[ 2 ] = z;
        m_s  = s;
        m_ks = ks;
        m_kd = kd;
    }

    model::model( model const & m ) noexcept {
        m_w0 = m.m_w0;
        m_s  = m.m_s;
        m_ks = m.m_ks;
        m_kd = m.m_kd;
    }

    model::model( model && m ) noexcept {
        std::swap( m_w0, m.m_w0 );
        m_s  = m.m_s;
        m_ks = m.m_ks;
        m_kd = m.m_kd;
    }

    model::~model( void ) noexcept {
    }

} // brdf