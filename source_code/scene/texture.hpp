//
//  texture.hpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>

#include "../resources/resource.hpp"

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//
//		parameters (x,y,z) in texture coordinates are called (s,t,r)	//
//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

namespace ms {

	class Texture : public Resource {

	protected:

		typedef unsigned char byte;

	public:

		enum class MinFilter;
		enum class MagFilter;
		enum class Type;
		enum class Wrapping;
		enum class Format;
		enum class AssociatedType;

		 inline 						Texture			(Type					type,
														 std::string			name,
														 Format					format,
														 AssociatedType			associatedType,
														 MinFilter 				minFilter,
														 MagFilter 				magFilter,
														 Wrapping 				sWrapping,
														 Wrapping 				tWrapping,
														 unsigned int 			mipMapLevel,
														 unsigned int 			width,
														 unsigned int 			height
														 );

		inline 			void			copy_data		(byte* data, size_t size);
		inline 			int				channels_amount () const;
						Texture &		operator = 		(const Texture &) = delete;
										Texture			(const Texture &) = delete;
		inline 	virtual std::string		get_class		() const override;
				virtual void 			use				() = 0;
		inline 	virtual 				~Texture		();

	public:

		std::string								name;
		Type									type;
		Format									format;
		AssociatedType							associatedType;
		MinFilter 								minFilter;
		MagFilter 								magFilter;
		Wrapping 								sWrapping;
		Wrapping 								tWrapping;
		unsigned int 							mipMapLevel;
		unsigned int 							width;
		unsigned int 							height;
		byte *									rawData;

	};

}

enum class ms::Texture::MinFilter {
	nearest,
	linear,
	nearest_mipmap_nearest,
	linear_mipmap_nearest,
	nearest_mipmap_linear,
	linear_mipmap_linear
};

enum class ms::Texture::MagFilter {
	nearest,
	linear
};

enum class ms::Texture::Wrapping {
	repeat,
	mirrored_repeat,
	clamp_to_edge,
	clamp_to_border
};

enum class ms::Texture::Type {
	tex_2d,
	tex_cube_map
};

enum class ms::Texture::Format {
	rgb_16_16_16,
	rgb_8_8_8,
	rgba_8_8_8_8,
	r_8,
	depth_16,
	depth_24,
	depth_32
};

enum class ms::Texture::AssociatedType {
	FLOAT,
	UNSIGNED_BYTE,
    UNSIGNED_INT,
    UNSIGNED_SHORT
};

ms::Texture::Texture (Type type,
                      std::string name,
                      Format format,
                      AssociatedType associatedType,
                      MinFilter minFilter,
                      MagFilter magFilter,
					  Wrapping sWrap,
                      Wrapping tWrap,
                      unsigned int mipMapLevel,
                      unsigned int width,
                      unsigned int height) :    type{type},
											    minFilter{minFilter},
												magFilter{magFilter},
												sWrapping(sWrap),
												tWrapping(tWrap),
												mipMapLevel{mipMapLevel},
                                                width{width},
												height{height},
												name{name},
                                                format{format},
                                                associatedType{associatedType},
												rawData{nullptr} { }

void ms::Texture::copy_data (byte* data, size_t size) {
    if (rawData) {
        delete [] rawData;
    }
	rawData = new byte [size];
	std::memcpy(rawData, data, size);
}

std::string ms::Texture::get_class () const {
	return "ms::Texture";
}

int ms::Texture::channels_amount () const {
	switch (this->format) {
        case Format::depth_16:
        case Format::depth_24:
        case Format::depth_32:
		case Format::r_8:
			return 1;
		case Format::rgba_8_8_8_8:
			return 4;
		case Format::rgb_8_8_8:
		case Format::rgb_16_16_16:
			return 3;
		default:
			std::cerr << "critical error, format not handled" << std::endl;
			assert(false);
	}
}

ms::Texture::~Texture() {
    if (rawData) {
        delete [] rawData;
    }
}

#endif /* texture_hpp */
