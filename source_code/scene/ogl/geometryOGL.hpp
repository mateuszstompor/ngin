//
//  geometryOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef model_ogl_hpp
#define model_ogl_hpp

#include "../geometry.hpp"
#include "../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class GeometryOGL : public Geometry {
		
	public:
		
				void 			use_normals 			() override;
				void 			use_vertices 			() override;
				void 			use_indicies 			() override;
				void 			use_texture_coord	 	() override;
		virtual	std::string		get_class				() override;
				void 			load					() override;
				void 			unload					() override;
				void 			load_vertices_to_buffer	();
		
	protected:
		
		GLuint 	normalsBuffer;
		GLuint 	positionsBuffer;
		GLuint 	texturesCooridnatesBuffer;
		
		GLuint 	indiciesBuffer;
		
	};
	
}

#endif /* model_ogl_hpp */
