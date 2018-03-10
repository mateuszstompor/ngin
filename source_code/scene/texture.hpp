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

#include "../resources/resource.hpp"

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//
//		parameters (x,y,z) in texture coordinates are called (s,t,r)	//
//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

namespace ms {
	
	class Texture : public Resource {
		
	public:
		
		enum class MinFilter;
		enum class MagFilter;
		enum class Wrapping;
		
		inline 			Texture		(MinFilter 				minFilter,
									 MagFilter 				magFilter,
									 Wrapping 				sWrapping,
									 Wrapping 				tWrapping,
									 unsigned int 			mipMapLevel,
									 unsigned int 			width,
									 unsigned int 			height
									 );
		Texture &		operator = 	(const Texture &) = delete;
						Texture		(const Texture &) = delete;
		virtual void 	use			() = 0;
		virtual 		~Texture	() = default;
		
	public:
	
		MinFilter 								minFilter;
		MagFilter 								magFilter;
		Wrapping 								sWrapping;
		Wrapping 								tWrapping;
		unsigned int 							mipMapLevel;
		unsigned int 							width;
		unsigned int 							height;
		
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

ms::Texture::Texture (MinFilter minF, MagFilter magF, Wrapping sWrap, Wrapping tWrap, unsigned int mipMapL, unsigned int wid, unsigned int hei) :
minFilter(minF), magFilter(magF), sWrapping(sWrap), tWrapping(tWrap), mipMapLevel(mipMapL), width(wid), height(hei) { }

#endif /* texture_hpp */
