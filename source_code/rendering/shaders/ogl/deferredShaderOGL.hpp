//
//  deferredShaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferredShaderOGL_hpp
#define deferredShaderOGL_hpp

#include "shaderOGL.hpp"
#include "../deferredShader.hpp"
#include "../../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class DeferredShaderOGL : public ShaderOGL, public DeferredShader {
	
	public:
		
						DeferredShaderOGL(std::string vertexShaderSource,
										  std::string fragmentShaderSource);
		
		virtual void 	_load						() 									override;
		virtual void 	set_projection_matrix		(const math::mat4 & proj)			override;
		virtual void 	set_camera_transformation	(const math::mat4 & transf)			override;
		virtual void	set_model_transformation	(const math::mat4 & modelTransf)	override;
		virtual void	set_has_material			(bool doesItHave)	 				override;
		virtual void	set_has_diffuse_texture		(bool doesItHave)	 				override;
		virtual void	set_has_specular_texture	(bool doesItHave)	 				override;
		virtual void	set_has_normal_texture		(bool doesItHave)	 				override;
		
	protected:
		
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

#endif /* deferredShaderOGL_hpp */
