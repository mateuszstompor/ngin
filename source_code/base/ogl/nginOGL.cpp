//
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

ms::NGinOGL::NGinOGL (	std::shared_ptr<std::string> forwardRenderVertexShaderSource,
						std::shared_ptr<std::string> forwardRenderFragmentShaderSource,
					  	std::shared_ptr<std::string> deferredRenderVertexShaderSource,
					  	std::shared_ptr<std::string> deferredRenderFragmentShaderSource,
					  	std::shared_ptr<std::string> deferredRenderLightingVertexShaderSource,
					  	std::shared_ptr<std::string> deferredRenderLightingFragmentShaderSource,
						unsigned int screenWidth,
						unsigned int screenHeight,
						unsigned int frameBufferWidth,
						unsigned int frameBufferHeight,
						float camNear,
						float camFar,
						float fovDegrees,
						float aspect,
					  	GLuint defaultFBO
						) : NGin(screenWidth, screenHeight, camNear, camFar, fovDegrees, aspect) {
    
    forwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRenderOGL(forwardRenderVertexShaderSource,
						  forwardRenderFragmentShaderSource,
						  screenWidth,
						  screenHeight,
						  frameBufferWidth,
						  frameBufferHeight));
	
	auto defPtr = std::unique_ptr<DeferredRenderOGL> (new DeferredRenderOGL(deferredRenderVertexShaderSource,
																			  deferredRenderFragmentShaderSource,
																			  deferredRenderLightingVertexShaderSource,
																			  deferredRenderLightingFragmentShaderSource,
																			  screenWidth,
																			  screenHeight,
																			  frameBufferWidth,
																			  frameBufferHeight));
	defPtr->set_default_FBO(defaultFBO);
	deferredRenderer = std::move(defPtr);
	
}

void ms::NGinOGL::load () {
	forwardRenderer->load();
	deferredRenderer->load();
	deferredRenderer->use();
}

void ms::NGinOGL::draw_scene() {
	deferredRenderer->clear_frame();
	deferredRenderer->draw_scene(scene.get());
}
