//
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

ms::NGinOGL::NGinOGL (	  std::shared_ptr<std::string> vertexShaderSource,
						  std::shared_ptr<std::string> fragmentShaderSource,
						  unsigned int screenWidth,
						  unsigned int screenHeight,
						  unsigned int frameBufferWidth,
						  unsigned int frameBufferHeight,
					  	  float camNear,
						  float camFar,
					      float fovDegrees,
					      float aspect
						  ) : NGin(screenWidth, screenHeight, camNear, camFar, fovDegrees, aspect) {
    
    forwardRenderer = std::unique_ptr<Render>
	(new ForwardRenderOGL(vertexShaderSource, fragmentShaderSource,screenWidth, screenHeight, frameBufferWidth, frameBufferHeight));
	
}

void ms::NGinOGL::load () {
	forwardRenderer->load();
}

void ms::NGinOGL::draw_scene() {
	forwardRenderer->use();
	forwardRenderer->clear_frame();
    forwardRenderer->draw_scene(scene);
}

void ms::NGinOGL::unload () {
	forwardRenderer->unload();
	NGin::unload();
}
