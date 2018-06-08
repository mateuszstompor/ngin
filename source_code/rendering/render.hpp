//
//  render.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "../resources/resource.hpp"
#include "../scene/scene.hpp"
#include "other/framebuffer.hpp"
#include "shaders/shader.hpp"

namespace ms {

    /**
     * Baza dla wszystkich renderów.
     * Stworzona w celu przedstawienia na bufor ramki obiektu, bądź efektu graficznego.
     * Stanowi integralną, niewidoczną dla użytkownika część biblioteki.
     * Choć nie jest to wylistowane klasa posiada powiązany ze sobą bufor ramki, na który przedstawia efkety pracy oraz program cieniującego, którego używa do ich generowania.
     * @see Framebuffer
     * @see Shader
     */
    class Render : public Resource {
	public:

		virtual							~Render 		() = default;

    protected:
										Render			(std::unique_ptr<Framebuffer> && 	framebuffer,
												         std::unique_ptr<Shader> && 		shader);
		virtual void					use		     	();
		virtual void					use		     	(Framebuffer & framebuffer);
		virtual	void					_load			() override;
		virtual	void 					_unload			() override;
		virtual std::string 			get_class		() const override;
		Framebuffer &					get_framebuffer () { return *framebuffer; }
		Shader &						get_shader		() { return *shader; }
		
		std::unique_ptr<Framebuffer> 	framebuffer;
		std::unique_ptr<Shader>			shader;

    };
    
}

