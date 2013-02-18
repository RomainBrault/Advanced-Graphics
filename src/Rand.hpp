#ifndef RAND_HPP_INCLUDED
#define RAND_HPP_INCLUDED

#include <AdvancedGraphicsConfig.hpp>

namespace rnd {

class xorShift {
public:
    xorShift( void     ) noexcept;
    xorShift( uint32_t ) noexcept;
    ~xorShift( void    ) noexcept;

    xorShift( xorShift const & rng );

    uint32_t rand( void  ) noexcept;

    template < typename T >
    T rand( T x );

    template < typename T >
    T rand( T x, T y );

    void skip( uint32_t ) noexcept;

private:
    uint32_t m_x;
    uint32_t m_y;
    uint32_t m_z;
};

template < typename T >
T
xorShift::rand( T x ) {
    return static_cast< T >(
        x * ( rand( ) / static_cast< float >( 0xFFFFFFFF ) )
    );
}

template < typename T >
T
xorShift::rand( T x, T y ) {
    return static_cast< T >(
        ( y - x ) * ( rand( ) / static_cast< float >( 0xFFFFFFFF  ) ) + x
    );
}

} // namespace rnd

#endif // RAND_HPP_INCLUDED