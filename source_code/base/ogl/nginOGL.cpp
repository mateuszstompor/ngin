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
						  unsigned int frameBufferHeight
						  ) : NGin(screenWidth, screenHeight) {
    
    forwardRenderer = std::unique_ptr<Render>
	(new ForwardRenderOGL(vertexShaderSource, fragmentShaderSource,screenWidth, screenHeight, frameBufferWidth, frameBufferHeight));
	
}

void ms::NGinOGL::draw_scene() {
	forwardRenderer->clear_frame();
    forwardRenderer->draw_scene(scene);
}

void ms::NGinOGL::unload () {
	NGin::unload();
}
