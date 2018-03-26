//
//  drawableOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef drawable_ogl_hpp
#define drawable_ogl_hpp

#include "../drawable.hpp"
#include "../../utils/ogl/proxyOGL.hpp"
#include "../../rendering/reusableGeometry.hpp"
#include "geometryOGL.hpp"

namespace ms {
	
	class DrawableOGL : public virtual Drawable {
		
	public:
		
		friend class Loader;
		
												DrawableOGL		();
				void 							use				() override;
				void							_load			() override;
				void 							_unload			() override;
				std::string 					get_class 		() override;
				void							draw			() override;
		static	std::shared_ptr<DrawableOGL>	get_quad		();
		virtual									~DrawableOGL	() = default;
		
	protected:
		
		GLuint 	vertexArray;
		
	};
	
}

#endif /* drawable_ogl_hpp */
