//
//  deferredShader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferredShader_hpp
#define deferredShader_hpp

#include "./shader.hpp"
#include "../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class DeferredShader : public Shader {
	
	public:
		
				DeferredShader				(std::string vertexShaderSource,
											 std::string fragmentShaderSource);
		
		void 	_load						();
		void 	set_projection_matrix		(const math::mat4 & proj);
		void 	set_camera_transformation	(const math::mat4 & transf);
		void	set_model_transformation	(const math::mat4 & modelTransf);
		void	set_has_material			(bool doesItHave);
		void	set_has_diffuse_texture		(bool doesItHave);
		void	set_has_specular_texture	(bool doesItHave);
		void	set_has_normal_texture		(bool doesItHave);
		
	private:
		
		GLint			projectionMatrixLocation;
		GLint			cameraTransformationLocation;
		GLint			modelTransformationLocation;
		
		GLint			hasMaterialLocation;
		GLint			hasDiffuseTextureLocation;
		GLint			hasSpecularTextureLocation;
		GLint			hasNormalTextureLocation;
		
		GLint			materialBlockLocation;
		
	};
	
}

#endif /* deferredShader_hpp */
