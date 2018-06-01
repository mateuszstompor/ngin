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

#include "proxyOGL.hpp"

namespace ms {
    
	namespace utils {

		template <typename T>
		inline std::string ptr_to_string(T * ptr) {
			std::stringstream stream;
			stream << static_cast<const void *>(ptr);
			return stream.str();
		}

		template <typename T>
		inline T* ptr_from_string (std::string ptrInString){
			std::stringstream ss;
			ss << ptrInString;
			unsigned long long int i;
			ss >> std::hex >> i;
			return reinterpret_cast<T *>(i);
		}
		
		inline std::string load_contents_of_file(std::string pathToFile) {
			std::ifstream inputStream(pathToFile, std::ios_base::in);
			if(!inputStream.good())
				std::cerr << "There is no file in such location" << '\n';
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
        
        template <typename T>
        inline void map_buffer_range_for_writing(GLenum bufferType, GLuint id, std::size_t amountOfElements, GLenum purpose, std::function<void(T*)> lambda) {
            mglBindBuffer(bufferType, id);
            mglBufferData(bufferType, amountOfElements * sizeof(T), nullptr, purpose);
            //TODO find out why this needs workaround
            //#define GL_MAP_WRITE_BIT 0x0002
            lambda(static_cast<T*>(mglMapBufferRange(bufferType, 0, amountOfElements * sizeof(T), 0x0002)));
            mglUnmapBuffer(bufferType);
            mglBindBuffer(bufferType, 0);
        }
        
        template <typename T>
        inline void map_buffer_range_for_read(GLenum bufferType, GLuint id, std::size_t amountOfElements, std::function<void(T*)> lambda) {
            mglBindBuffer(bufferType, id);
            //TODO find out why this needs workaround
            //#define GL_MAP_READ_BIT 0x0001
            lambda(static_cast<T*>(mglMapBufferRange(bufferType, 0, amountOfElements * sizeof(T), 0x0001)));
            mglUnmapBuffer(bufferType);
            mglBindBuffer(bufferType, 0);
        }
        
	}
    
}
