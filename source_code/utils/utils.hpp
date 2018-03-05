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
	
	#define FILE_DOESNT_EXIST "There is no file in such location"
	
}

namespace ms {
    
	namespace utils {
	
		std::string load_contents_of_file(std::string pathToFile) {
			std::ifstream inputStream(pathToFile, std::ios_base::in);
			if(!inputStream.good()) {
				std::cerr << FILE_DOESNT_EXIST << std::endl;
			}
			std::stringstream strStream;
			strStream << inputStream.rdbuf();
			return strStream.str();
		}
		
	}
    
}

#endif /* utils_hpp */
