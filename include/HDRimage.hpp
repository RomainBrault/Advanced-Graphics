#ifndef HDRIMAGE_HPP_INCLUDED
#define HDRIMAGE_HPP_INCLUDED

#include <HDRimageConfig.hpp>

namespace hdr {

typedef struct {
	/* Define a block of pixel to vectorize code. 
	 * AVX format
	 */
	float r[ 8 ];
	float g[ 8 ];
	float b[ 8 ];
} pixelBlock;

enum pfmEndianess : int32_t {
	PFM_LITTLE_ENDIAN = -1,
	PFM_BIG_ENDIAN    =  1
};

enum class pnm_t : uint32_t {
	ASCIIBitmap    = '1',
	ASCIIGreymap   = '2',
	ASCIIColormap  = '3',
	BinaryBitmap   = '4',
	BinaryGreymap  = '5',
	BinaryColormap = '6'
};

enum class pfm_t : uint32_t {
	BinaryGreymap  = 'f',
	BinaryColormap = 'F'
};

class image {
/* Row major matrix of pixelBlock */
public:
	image( void ) noexcept;
	image( uint32_t, uint32_t, uint32_t = 1 ) noexcept;
	image( image const & im ) noexcept;

	// move constructor
	// overload operator= (move + copy)

	image & create( uint32_t, uint32_t, uint32_t = 1 ) noexcept;
	image & copy  ( image const & im ) noexcept;

	INLINE uint32_t getWidth     ( void ) const noexcept;
	INLINE uint32_t getHeight    ( void ) const noexcept;
	INLINE size_t   getPadd      ( void ) const noexcept;

	INLINE bool isEmpty( void ) const noexcept;

	void free ( void ) noexcept;

	void normalise( float = 1 ) noexcept;

	int32_t loadPNM( 
		std::string const &
	) noexcept;
	int32_t savePNM( 
		std::string const &, pnm_t = pnm_t::BinaryColormap 
	) const noexcept;

	int32_t loadPFM( 
		std::string const &
	) noexcept;
	int32_t savePFM( 
		std::string const &, pfm_t = pfm_t::BinaryColormap 
	) const noexcept;

	~image( void ) noexcept;

protected:
	void alloc( uint32_t, uint32_t ) noexcept;

private:
	uint32_t      m_width;
	uint32_t      m_height;
	size_t        m_padd;
	uint32_t      m_max_pixel_chanel;
	pixelBlock**  m_data_2D;
	pixelBlock*   m_data_1D;
};


uint32_t 
image::getWidth( void ) const noexcept
{
	return m_width;
}

uint32_t 
image::getHeight( void ) const noexcept
{
	return m_height;
}

size_t 
image::getPadd( void ) const noexcept
{
	return m_padd;
}

bool 
image::isEmpty( void ) const noexcept {
	return m_data_1D == nullptr;
}

} // namespace hdr

#endif