//
//  postprocessRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>

#include "../render.hpp"
#include "../shaders/shader.hpp"

namespace ms {

    /**
     * Klasa odpowiada za nanoszenie efektu na klatkę obrazu.
     * Efekty postprocesowe stanowią ostatni, opcjonalny krok podczas generowania klatki obrazu.
     * Są one częścią fasady biblioteki - klasy NGin.
     * @see NGin
     */
	class PostprocessRender : public Render {
        
        friend class NGin;
        
    public:

        /**
         * Służy do sprawdzenie, czy efekt postprocesowy jest obecnie włączony.
         */
        constexpr bool                          is_enabled          () const { return isOn; }
        /**
         * Służy do określania czy efekt postprocesowy powinien poddawać obraz modyfikacjom.
         */
        void                                    set_enabled         (bool state);
        virtual                                 ~PostprocessRender  () = default;
        
    protected:
                                                PostprocessRender   (std::vector<std::weak_ptr<Texture2D>>  input,
                                                                     std::unique_ptr<Framebuffer> &&        framebuffer,
                                                                     std::unique_ptr<Shader> &&             shaderProgram);
        virtual void                            draw                () const;
        
	private:
        
        std::string                             get_class           () const override;
		void                                    _load               () override;
		void                                    _unload             () override;
		bool                                    isOn;
		std::vector<std::weak_ptr<Texture2D>> 	inputTextures;
		std::unique_ptr<Drawable> 				quad;

	};
	
}
