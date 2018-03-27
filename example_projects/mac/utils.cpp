//
//  utils.cpp
//  ngin_mac
//
//  Created by Mateusz Stompór on 28/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "utils.hpp"

void prepareRenderDoc() {
	
#ifdef __WIN32__
	
	HINSTANCE hGetProcIDDLL = LoadLibrary("renderdoc.dll");
	
	if (!hGetProcIDDLL) {
		std::cout << "could not load the dynamic library" << std::endl;
		return exit(0);
	}
	
	HMODULE mod = GetModuleHandleA("renderdoc.dll");
	if(mod) {
		pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
		
		RENDERDOC_API_1_0_0 *rdoc_api = NULL;
		int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_0_0, (void **)&rdoc_api);
		
		assert(ret == 1);
		assert(rdoc_api->StartFrameCapture != NULL && rdoc_api->EndFrameCapture != NULL);
		
		int major = 999, minor = 999, patch = 999;
		std::cout<<rdoc_api->GetLogFilePathTemplate()<<std::endl;
		rdoc_api->GetAPIVersion(&major, &minor, &patch);
		
		assert(major == 1 && minor >= 0 && patch >= 0);
	}
	
#endif
	
}
