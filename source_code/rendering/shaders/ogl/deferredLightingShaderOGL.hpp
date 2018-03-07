//
//  deferredLightingShaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_lightingShader_ogl_hpp
#define deferred_lightingShader_ogl_hpp

#include "../deferredLightingShader.hpp"
#include "shaderOGL.hpp"

namespace ms {
	
	class DeferredLightingShaderOGL : public ShaderOGL, public DeferredLightingShader {
	
	public:
		
		DeferredLightingShaderOGL(str_ptr vertexShaderSource, str_ptr fragmentShaderSource);
		
	};
	
}

#endif /* deferred_lightingShader_ogl_hpp */
