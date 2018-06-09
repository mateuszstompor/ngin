//
//  texture.hpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <iostream>
#include <cstdint>

#include "textureUtilities.hpp"
#include "../resources/resource.hpp"
#include "../utils/proxyOGL.hpp"

namespace ms {
	/**
	 * Zadaniem klasy jest reprezentowanie dwuwymiarowego obrazu w formie prostokątnego rastra.
	 * Tekstura jest elementem sceny, który może być powiązany z materiałem w celu nadania mu bardziej szczegółowego wyglądu za pomocą koloru.
	 * Dodatkowo, tekstura wykorzystywana może być jako bufor ramki obrazu.
	 * @see Scene
	 * @see Material
	 */
	class Texture2D : public Resource {
		
        friend class Renderbuffer;
        friend class Framebuffer;
        friend class Shader;
        
        using byte = std::uint8_t;
        
	public:

	/**
	 * @param format Specyfikuje ile kanałów oraz ile bitów przeznaczyć na reprezentacje każdego z nich.
	 * @param associatedType Określa typ liczbowy powinien zostać użyty do reprezentacji tekstury.
	 * @param width Szerokość tekstury
	 * @param height Wysokość tekstury
	 * @param name nazwa tekstury
	 * @param minFilter Specyfikuje jakie filtrowanie należy zastosować dla tekstury która wyświetlana jest w wymiarze mniejszym niż dla niej domyślny.
	 * @param magFilter Specyfikuje jakie filtrowanie należy zastosować dla tekstury która wyświetlana w rozciągnięciu.
	 * @param sWrapping Określa czym wypełnić pole które nie zostało objęte teksturą, w osi s(x).
	 * @param tWrapping Określa czym wypełnić pole które nie zostało objęte teksturą, w osi t(y).
	 * @param mipMapLevel Specyfikuje ilość pozimów mipmap do wygenerowania.
	 */
                                    Texture2D           (texture::Format             format,
                                                         texture::AssociatedType     associatedType,
                                                         unsigned int                width,
                                                         unsigned int                height,
                                                         std::string                 name = "",
                                                         texture::MinFilter          minFilter = texture::MinFilter::nearest,
                                                         texture::MagFilter          magFilter = texture::MagFilter::nearest,
                                                         texture::Wrapping           sWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           tWrapping = texture::Wrapping::repeat,
                                                         unsigned int                mipMapLevel = 0);
                                    ~Texture2D          ();
        /**
         * Klasa w obecnym stadium rozwoju biblioteki nie wspiera kopiowania.
         */
                                    Texture2D           (const Texture2D & texture) = delete;
        /**
         * Klasa w obecnym stadium rozwoju biblioteki nie wspiera kopiowania.
         */
                    Texture2D &	    operator =			(const Texture2D & texture) = delete;
                    std::string		get_class			() const override;
        /**
         * Metoda, która pozwala skopiować dane do wewnątrznych zasobów klasy.
         * Dzięki użytkownik jest w stanie sam tworzyć dane w locie i na ich podstawie generować tekstury.
         * @param data wskazuje na adres w pamięci z którego należy zacząć pobieranie danych.
         * @param size określa ilość danych w bajtach koniecznych do skopiowania.
         */
                    void            copy_data           (byte const * data, size_t size);
        
	private:
        
        constexpr   GLuint          get_underlying_id   () const { return glTexture; }
                    void            _load               () override;
                    void            _unload             () override;
                    void             use                ();

        std::string                 name;
        texture::Format             format;
        texture::AssociatedType     associatedType;
        texture::MinFilter          minFilter;
        texture::MagFilter          magFilter;
        texture::Wrapping           sWrapping;
        texture::Wrapping           tWrapping;
        unsigned int                mipMapLevel;
        unsigned int                width;
        unsigned int                height;
        byte *                      rawData;
        GLenum                      glInternalFormat;
        GLenum                      glColorFormat;
        GLenum                      glType;
        GLenum                      glTarget;
        GLuint                      glTexture;
		
	};
	
}
