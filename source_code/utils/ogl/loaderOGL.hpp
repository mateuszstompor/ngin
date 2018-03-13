//
//  loaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef loaderOGL_hpp
#define loaderOGL_hpp

#include "../loader.hpp"
#include "../../scene/ogl/geometryOGL.hpp"

namespace ms {

	class LoaderOGL : public Loader {
	
	public:
		
	protected:
		
		inline virtual std::shared_ptr<Geometry> 		get_geometry	() override;
		
	};
	
}

std::shared_ptr<ms::Geometry> ms::LoaderOGL::get_geometry() {
	return std::shared_ptr<ms::Geometry>(new GeometryOGL());
}

#endif /* loaderOGL_hpp */
