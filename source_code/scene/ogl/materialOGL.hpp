//
//  materialOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 27/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef materialOGL_hpp
#define materialOGL_hpp

#include "../material.hpp"
#include "../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class MaterialOGL : public Material {
		
	public:
		
		 						MaterialOGL		(math::vec3 ambient,
												 math::vec3 diffuse,
												 math::vec3 specular,
												 float shininess,
												 float opacity,
												 std::string name);
		
		virtual	void			use				() override;
		
	protected:
		
		virtual void			_load			() override;
		virtual void			_unload			() override;
		
		GLuint					bufferId;
		
	};
	
}

#endif /* materialOGL_hpp */
