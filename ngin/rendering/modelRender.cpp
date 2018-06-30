//
//  modelRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "modelRender.hpp"

ms::ModelRender::ModelRender (std::unique_ptr<Framebuffer> && framebuffer, std::unique_ptr<Shader> && shader) :
Render{std::move(framebuffer), std::move(shader)} {}
