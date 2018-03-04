//
//  utils.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>

#include <vector>

namespace ms {
    
	namespace utils {
	
		std::string load_contents_of_file(std::string pathToFile) {
			std::ifstream inputStream(pathToFile, std::ios_base::in);
			std::stringstream strStream;
			strStream << inputStream.rdbuf();
			return strStream.str();
		}
		
	}
    
}

#endif /* utils_hpp */
