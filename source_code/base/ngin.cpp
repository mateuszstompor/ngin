//
//  ngin.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "ngin.hpp"

ms::NGin::NGin() : scene(new Scene()) {
    
}

void ms::NGin::sygnalize_loading_ability() {

}

void ms::NGin::unload() {
	ResourceCoordinator::sharedInstance->unload_all_resources();
}
