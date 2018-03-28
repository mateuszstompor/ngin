//
//  utils.hpp
//  ngin_mac
//
//  Created by Mateusz Stompór on 28/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef utils_example_hpp
#define utils_example_hpp

#include <cassert>
#include <iostream>

#ifdef __WIN32__
	#include <glad/glad.h>
	#include "../../third-party-libs/renderdoc.h"
#endif

void prepareRenderDoc();

#endif /* utils_example_hpp */
