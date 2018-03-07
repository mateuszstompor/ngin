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
			
			-1, 1, 0,
			1, 1, 0,
			-1, -1, 0,
			-1, -1, 0,
			1, 1, 0,
			1, -1, 0
			
		};
		
		const static float textureCoordinates[] = {
			0, 1,
			1, 1,
			0, 0,
			0, 0,
			1, 1,
			1, 0
		};
		
	}
	
}

#endif //reusable_geometry_h
