//
//  utils.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <iostream>
#include <fstream>
#include <streambuf>
#include <chrono>
#include <sstream>
#include <functional>
#include <vector>

namespace ms {
	
	#define FILE_DOESNT_EXIST "There is no file in such location"
	
}

namespace ms {
    
	namespace utils {

		template <typename T>
		inline std::string ptr_to_string(T* ptr) {
			std::stringstream stream;
			stream << static_cast<const void *>(ptr);
			return stream.str();
		}

		template <typename T>
		inline T* ptr_from_string (std::string ptrInString){
			std::stringstream ss;
			ss<<ptrInString;
			long long unsigned int i;
			ss>>std::hex>>i;
			return reinterpret_cast<T *>(i);
		}
		
		inline std::string load_contents_of_file(std::string pathToFile) {
			std::ifstream inputStream(pathToFile, std::ios_base::in);
			if(!inputStream.good()) {
				std::cerr << FILE_DOESNT_EXIST << '\n';
			}
			std::stringstream strStream;
			strStream << inputStream.rdbuf();
			return strStream.str();
		}
		
		template<typename T>
		inline long measure_time(std::function<void(void)> lambda) {
			auto start = std::chrono::high_resolution_clock::now();
			lambda();
			auto now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<T>(now - start).count();
		}
		
	}
    
}
