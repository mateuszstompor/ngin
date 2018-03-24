//
//  reusableGeometry.hpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef reusable_geometry_h
#define reusable_geometry_h

namespace ms {
	
	namespace quad {
		
		const static float vertices[] = {
			
			-1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0, -1.0f, 0.0f
			
		};
		
		const static float textureCoordinates[] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};
		
	}
	
	namespace quad_indicies {
		
		const static float vertices[] = {
			-1.0f, -1.0f, 0.0f,
			-1.0, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};
		
		const static float textureCoordinates[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};
		
		const static unsigned int indicies[] = {
			0, 1, 2,
			0, 2, 3
		};
		
	}
	
}

#endif //reusable_geometry_h
