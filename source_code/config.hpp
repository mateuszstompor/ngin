//
//  config.hpp
//  ngin
//
//  Created by Mateusz Stompór on 23/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#ifdef DEBUG

//ResourceCoordinator
//#define RC_ASSERTIONS
//#define RC_LOADS
//#define RC_UNLOADS
//#define RC_ALLOCATIONS
//#define RC_DEALLOCATIONS

//Resource
//#define R_LOADS
//#define R_UNLOADS
//#define R_ALLOCATIONS
//#define R_DEALLOCATIONS

//Loader
//#define LD_MAT_COMP_COUNT
//#define LD_MAT_SUMMARY

//ProxyOGL
#define POGL_UNEXISING_UNIFORMS
#define POGL_UNEXISING_BLOCK_UNIFORMS

//NGin
#define NGIN_COUNT_FPS

#endif

#endif /* config_hpp */
