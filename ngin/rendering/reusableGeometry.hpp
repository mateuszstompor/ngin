//
//  reusableGeometry.hpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

namespace ms {
	
	namespace quad {
		
		const static float vertices[] = {
			
			- 1.0f,  1.0f, 0.0f,
			  1.0f,  1.0f, 0.0f,
			- 1.0f, -1.0f, 0.0f,
			- 1.0f, -1.0f, 0.0f,
			  1.0f,  1.0f, 0.0f,
			  1.0f, -1.0f, 0.0f
			
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
	
	namespace quad_indexed {
		
		const static float vertices[] = {
			-1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
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

