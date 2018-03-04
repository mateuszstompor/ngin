//
//  sceneNodeOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef scene_node_ogl_hpp
#define scene_node_ogl_hpp

#if __APPLE__

	#if ios_build

		#import <OpenGLES/ES3/gl.h>
		#import <OpenGLES/ES3/glext.h>

	#else

		#include <OpenGL/gl3.h>
		#include <OpenGL/gl.h>

	#endif

#endif

#include "../sceneNode.hpp"

namespace ms {
	
	class SceneNodeOGL : public SceneNode {
		
	public:
		
		void 	use			() override;
		void	load		() override;
		void 	unload		() override;
		
	protected:
		
		GLuint vertexArray;
		
	};
	
}

#endif /* scene_node_ogl_hpp */
