#include <iostream>
#include <Phong.hpp>

namespace phong {

    Phong::Phong( uint32_t s, float m_ks, float m_kd ) noexcept :
        m_ks( m_ks ), m_kd( m_kd ), m_s( s ), m_rng( time( nullptr ) )
    {
        m_samples = nullptr;
    }

    Phong::~Phong( ) {
        delete [] m_samples;
    }

    void
    Phong::generateSamples( uint32_t n_samples ) noexcept
    {
        m_samples = new ( std::nothrow ) obj::vect< float, 2 >[ n_samples ];
        if ( m_samples == nullptr ) {
            return;
        }
        for (uint32_t i = 0; i < n_samples; ++i)
        {
            sample( m_samples[ i ][ 0 ], m_samples[ i ][ 1 ] );
        }
    }

    void
    Phong::sample( float& theta, float& phi ) noexcept
    {
        float rand = m_rng( 0.f, 1.f );
        if ( ( rand >= 0 ) && ( rand < m_kd ) ) {
            diffuseSample( theta, phi );
            return;
        }

        if ( ( rand >= m_kd ) && ( rand < m_ks ) ) {
            specularSample( theta, phi );
            return;
        }

        if ( rand >= m_ks + m_kd ) {
            std::cerr << "No contribution" << std::endl;
        }
        specularSample(theta, phi);
    }

    void
    Phong::diffuseSample( float& theta, float& phi ) noexcept
    {
        theta = std::acos( 1.f - std::sqrt( m_rng( 0.f, 1.f ) ) );
        phi = 2 * M_PI * m_rng( 0.f, 1.f );
    }

    void
    Phong::specularSample( float& theta, float& phi ) noexcept
    {
        theta = std::acos(
            std::pow( 1.f - m_rng( 0.f, 1.f ), 1.f / ( m_s + 1.f ) )
        );
        phi = 2 * M_PI * m_rng( 0.f, 1.f );
    }
}
