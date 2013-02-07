#ifndef SPHERE_HPP_INCLUDED
#define SPHERE_HPP_INCLUDED

#include <AdvancedGraphicsConfig.hpp>

namespace std {

template< typename T >
static INLINE T
sqr( T x ) {
    return x * x;
}

} // namespace std

namespace details {

// Swap n values from the parameter pack to an output iterator
template < typename OutputIterator >
void copy_n( size_t, OutputIterator )
{
}

template < typename OutputIterator, typename T, typename... Args >
void copy_n( size_t n, OutputIterator out, T const & value, Args... args )
{
  if ( n > 0 ) {
    *out = static_cast< T >( value );
    copy_n( n - 1, ++out, args... );
  }
}

// Swap n values from the parameter pack to an output iterator, starting at
// the "beginth" element
template < typename OutputIterator >
void copy_range( size_t, size_t, OutputIterator )
{
}

template < typename OutputIterator, typename T, typename... Args >
void copy_range(
    size_t begin, size_t size,
    OutputIterator out, T const & value, Args... args
) {
    if ( begin == 0 ) {
        copy_n( size, out, value, args... );
    }
    else {
        copy_range( begin - 1, size, out, args... );
    }
}

}

namespace obj {

template < typename T, uint32_t DIM >
class vect
{
public:
    vect( void ) noexcept;
    template < typename... U >
    vect( U... Args ) noexcept;
    vect( vect< T, DIM > const &  ) noexcept;
    vect( vect< T, DIM >       && ) noexcept;

    void clear ( void ) noexcept;
    void setAll( T    ) noexcept;

    void mult_minus( T, vect< T, DIM > const & )       noexcept;
    T    dot       (    vect< T, DIM > const & ) const noexcept;
    T    norm      ( void                      ) const noexcept;

    T      operator []( uint32_t ) const noexcept;
    T &    operator []( uint32_t )       noexcept;

    vect< T, DIM > & operator = ( vect< T, DIM > const &  ) noexcept;
    vect< T, DIM > & operator = ( vect< T, DIM >       && ) noexcept;

private:
    T m_coord[ DIM ];
};

class sphere {

public:
    sphere ( void ) noexcept;
    sphere ( uint32_t, vect< uint32_t, 3 > const & ) noexcept;
    sphere ( uint32_t, uint32_t, uint32_t, uint32_t = 0 ) noexcept;
    ~sphere( void ) noexcept;

    INLINE uint32_t getRadius ( void ) const noexcept;
    INLINE vect< uint32_t, 3 > const &
                    getCenter ( void ) const noexcept;
    INLINE uint32_t getCenterX( void ) const noexcept;
    INLINE uint32_t getCenterY( void ) const noexcept;
    INLINE uint32_t getCenterZ( void ) const noexcept;

    INLINE void setCenter ( vect< uint32_t, 3 > ) noexcept;
    INLINE void setCenterX( uint32_t ) noexcept;
    INLINE void setCenterY( uint32_t ) noexcept;
    INLINE void setCenterZ( uint32_t ) noexcept;
    INLINE void setCenter (
        uint32_t, uint32_t, uint32_t = 0
    ) noexcept;
    INLINE void setRadius( uint32_t   ) noexcept;

    INLINE vect< float, 3 >
    normalXY     ( uint32_t, uint32_t ) const noexcept;
    INLINE vect< float, 3 >
    normalXY_fast( uint32_t, uint32_t ) const noexcept;

    INLINE vect< float, 3 >
    reflectanceXY(
        uint32_t, uint32_t, vect< float, 3 > const &
    ) const noexcept;
    INLINE vect< float, 3 >
    reflectanceXY_fast(
        uint32_t, uint32_t, vect< float, 3 > const &
    ) const noexcept;

private:
    uint32_t            m_radius;
    vect< uint32_t, 3 > m_center;
};






template < typename T, uint32_t DIM >
template < typename... U >
vect< T, DIM >::vect( U... args ) noexcept
{
    details::copy_range( 0, DIM,  m_coord, args... );
}

template < typename T, uint32_t DIM >
vect< T, DIM >::vect( void )
{

}

template < typename T, uint32_t DIM >
vect< T, DIM >::vect( vect< T, DIM > const & p )
{
    std::memcpy( m_coord, p.m_coord, DIM * sizeof ( T ) );
}

template < typename T, uint32_t DIM >
vect< T, DIM >::vect( vect< T, DIM > && p )
{
    std::swap( m_coord, p.m_coord );
}

template < typename T, uint32_t DIM >
void
vect< T, DIM >::clear( void ) noexcept
{
    std::memset( m_coord, 0, DIM * sizeof ( T ) );
}

template < typename T, uint32_t DIM >
void
vect< T, DIM >::setAll( T val ) noexcept
{
    for ( uint32_t i = 0; i < DIM; ++i ) {
        m_coord[ i ] = val;
    }
}

template < typename T, uint32_t DIM >
void
vect< T, DIM >::mult_minus(
    T mult,
    vect< T, DIM > const & offset
) noexcept {
    for ( uint32_t i = 0; i < DIM; ++i ) {
        m_coord[ i ] = mult * m_coord[ i ] - offset[ i ];
    }
}

template < typename T, uint32_t DIM >
T
vect< T, DIM >::dot( vect< T, DIM > const & p ) const noexcept
{
    T res( 0 );
    for ( uint32_t i = 0; i < DIM; ++i ) {
        res += m_coord[ i ] * p.m_coord[ i ];
    }
    return res;
}

template < typename T, uint32_t DIM >
T
vect< T, DIM >::norm( void ) const noexcept
{
    T res( 0 );
    for ( uint32_t i = 0; i < DIM; ++i ) {
        res += m_coord[ i ] * m_coord[ i ];
    }
    return std::sqrt( res );
}

template< typename T, uint32_t DIM >
T
vect< T, DIM >::operator []( uint32_t idx ) const noexcept
{
    return m_coord[ idx ];
}

template< typename T, uint32_t DIM >
T &
vect< T, DIM >::operator []( uint32_t idx ) noexcept
{
    return m_coord[ idx ];
}

template< typename T, uint32_t DIM >
vect< T, DIM > &
vect< T, DIM >::operator =( vect< T, DIM > const & p ) noexcept
{
    std::cout << "cpy" << std::endl;
    std::memcpy( m_coord, p.m_coord, DIM * sizeof ( T ) );
    return *this;
}

template< typename T, uint32_t DIM >
vect< T, DIM > &
vect< T, DIM >::operator =( vect< T, DIM > && p ) noexcept
{
    std::swap( m_coord, p.m_coord );
    return *this;
}


vect< uint32_t, 3 > const &
sphere::getCenter( void ) const noexcept
{
    return m_center;
}

uint32_t
sphere::getCenterX( void ) const noexcept
{
    return m_center[ 0 ];
}

uint32_t
sphere::getCenterY( void ) const noexcept
{
    return m_center[ 1 ];
}

uint32_t
sphere::getCenterZ( void ) const noexcept
{
    return m_center[ 2 ];
}

uint32_t
sphere::getRadius( void ) const noexcept
{
    return m_radius;
}

void
sphere::setCenter( vect< uint32_t, 3 > p ) noexcept
{
    m_center = p;
}

void
sphere::setCenterX( uint32_t x ) noexcept
{
    m_center[ 0 ] = x;
}

void
sphere::setCenterY( uint32_t y ) noexcept
{
    m_center[ 1 ] = y;
}

void
sphere::setCenterZ( uint32_t z ) noexcept
{
    m_center[ 2 ] = z;
}

void
sphere::setCenter( uint32_t x, uint32_t y, uint32_t z ) noexcept
{
    m_center[ 0 ] = x;
    m_center[ 1 ] = y;
    m_center[ 2 ] = z;
}

void
sphere::setRadius( uint32_t radius ) noexcept
{
    m_radius = radius;
}

vect< float, 3 >
sphere::normalXY( uint32_t x, uint32_t y ) const noexcept
{
    uint32_t r2   = m_radius * m_radius;
    int32_t  x_c  = x - m_center[ 0 ];
    int32_t  y_c  = y - m_center[ 1 ];
    uint32_t xs_c = std::sqr( x_c );
    uint32_t ys_c = std::sqr( y_c );
    uint32_t s_xs_ys_c = xs_c + ys_c;
    vect< float, 3 > res;
    if ( s_xs_ys_c > r2 ) {
        res.setAll( -1 );
        return res;
    }
    uint32_t zs_c = r2 - s_xs_ys_c;
    float norm = static_cast< float >(  sqrt( xs_c + ys_c + zs_c ) );
    res[ 0 ] = static_cast< float >( x_c ) / norm;
    res[ 1 ] = static_cast< float >( y_c ) / norm;
    res[ 2 ] = std::sqrt( static_cast< float >( zs_c ) ) / norm;
    return res;
}

vect< float, 3 >
sphere::normalXY_fast( uint32_t x, uint32_t y ) const noexcept
{
    uint32_t r2   = m_radius * m_radius;
    int32_t  x_c  = x - m_center[ 0 ];
    int32_t  y_c  = y - m_center[ 1 ];
    uint32_t xs_c = std::sqr( x_c );
    uint32_t ys_c = std::sqr( y_c );
    uint32_t s_xs_ys_c = xs_c + ys_c;
    vect< float, 3 > res;

    uint32_t zs_c = r2 - s_xs_ys_c;
    float norm = static_cast< float >(  sqrt( xs_c + ys_c + zs_c ) );
    res[ 0 ] = static_cast< float >( x_c ) / norm;
    res[ 1 ] = static_cast< float >( y_c ) / norm;
    res[ 2 ] = std::sqrt( static_cast< float >( zs_c ) ) / norm;
    return res;
}

vect< float, 3 >
sphere::reflectanceXY(
    uint32_t x, uint32_t y, vect< float, 3 > const & view
) const noexcept {

    vect< float, 3 > nv = normalXY( x, y );
    if ( // if black ( not in the sphere )
        ( nv[ 2 ] == -1 )
    ) {
        return nv;
    }
    float scal = nv.dot( view );
    nv.mult_minus( 2 * scal, view );
    return nv;
}


vect< float, 3 >
sphere::reflectanceXY_fast(
    uint32_t x, uint32_t y, vect< float, 3 > const & view
) const noexcept {

    vect< float, 3 > nv = normalXY_fast( x, y );
    if ( // if black ( not in the sphere )
        ( nv[ 2 ] == -1 )
    ) {
        return nv;
    }
    float scal = nv.dot( view );
    nv.mult_minus( 2 * scal, view );
    return nv;
}

} //namespace obj

#endif