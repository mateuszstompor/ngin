//
//  renderbuffer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef renderbuffer_hpp
#define renderbuffer_hpp

#include "../scene/texture.hpp"

namespace ms {
	
	class Renderbuffer : public Resource {
		
	public:
		
		inline 					Renderbuffer		(Texture::Format			format,
													 Texture::AssociatedType	associatedType,
													 unsigned int 				mipMapLevel,
													 unsigned int 				width,
													 unsigned int 				height);
		
								Renderbuffer		(const Texture &) = delete;
			Renderbuffer &		operator = 			(const Renderbuffer &) = delete;
			virtual void 		use					() = 0;
	inline	virtual std::string	get_class			();
			virtual 			~Renderbuffer		() = default;
		
	protected:
		
		Texture::AssociatedType				associatedType;
		Texture::Format						format;
		unsigned int 						mipMapLevel;
		unsigned int 						width;
		unsigned int 						height;
		
	};
	
}

ms::Renderbuffer::Renderbuffer(Texture::Format 			format,
							   Texture::AssociatedType 	associatedType,
							   unsigned int 			mipMapLevel,
							   unsigned int 			width,
							   unsigned int 			height) :
																	format(format),
																	associatedType(associatedType),
																	mipMapLevel(mipMapLevel),
																	width(width),
																	height(height) { }

std::string ms::Renderbuffer::get_class () {
	return "ms::Renderbuffer";
}


#endif /* renderbuffer_hpp */
