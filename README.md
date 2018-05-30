# ngin

## Requirements

#### macOS
<ul>
  <li>CMake</li>
  <li>clang (tested on <b>clang-900.0.39.2</b>)</li>
</ul>

#### Windows
<ul>
  <li>CMake</li>
  <li>Visual Studio compiler</li>
</ul>

#### iOS
<ul>
  <li>XCode 9+</li>
</ul>

## Installation

Project works on both Windows and macOS platforms. Use cmake in order to compile and run. macOS version can be opened as xcode project from
ngin/example_projects/mac/ngin_mac.xcodeproj as well

## 20.05.2017 Transparency
<img src="/screenshots/transparency.jpg" width="100%" style="float: left; display:inline-block;"/>

## 16.04.2018 Spot lights shadow mapping with depth linearization
<img src="/screenshots/spot_lights_shadow_mapping.jpg" width="100%" style="float: left; display:inline-block;"/>
<img src="/screenshots/ngin.jpg" width="100%" style="float: left; display:inline-block;"/>
<p>POV in the corner</p>
<img src="/screenshots/spot_lights_shadow_maps.jpg" width="100%" style="float: left; display:inline-block;"/>

## 15.04.2018 Directional lights shadow mapping with PCF
<img src="/screenshots/shadow_mapping.jpg" width="100%" style="float: left; display:inline-block;"/>

## 09.04.2018 Orthographic cameara
<img src="/screenshots/projections.jpg" width="100%" style="float: left; display:inline-block;"/>

## 09.04.2018 Frustum culling
<p>Removes models which aren't visible from point of view of camera from rendering pipeline. I marked using red color vector which describes direction in which camera looks to help you understand how does it work</p>
<img src="/screenshots/frustum_culling.jpg" width="100%" style="float: left; display:inline-block;"/>

## 30.03.2018 Normal mapping
<img src="/screenshots/normal_mapping.jpg" width="100%" style="float: left; display:inline-block;"/>

## 29.03.2018 Current pipeline
<img src="/screenshots/pipeline.jpg" width="100%" style="float: left; display:inline-block;"/>

## 29.03.2018 Vignette
<img src="/screenshots/vignette.jpg" width="100%" style="float: left; display:inline-block;"/>

## 24.03.2018 Gaussian blur, Bloom
<img src="/screenshots/bloom.jpg" width="100%" style="float: left; display:inline-block;"/>

## 24.03.2018 HDR + Adaptive tone mapping
<img src="/screenshots/hdr_tone_mapping.jpg" width="100%" style="float: left; display:inline-block;"/>

## 20.03.2018 Blinn-Phong specular shading
<p>Another variation of phong lighting model</p>

## 19.03.2018 Multiple resolutions
<p>It could increase performance if there are many objects placed on the scene. I rendered picture which is placed below in ultra low resolution as an instance</p>
<img src="/screenshots/potato_mode.jpg" width="100%" style="float: left; display:inline-block;"/>

## 19.03.2018 Light attenuation
<p>In other words it means that amount of light given by source decreases with rising distance</p>
<img src="/screenshots/spot_lights.jpg" width="100%" style="float: left; display:inline-block;"/>


## 17.03.2018 Gouraud shading
<p>Provides great performance but looks rather awful</p>
<img src="/screenshots/gouraud_shading.jpg" width="100%" style="float: left; display:inline-block;"/>

## 16.03.2018 Multiple light sources/renderers integration
<p>In order to generate frame which you can see below I had to combine two rendering techniques - forward and deferred rendering. Moreover I implemented point, directional and spot lights. They can be used simultaneously</p>
<img src="/screenshots/multiple_light_sources.jpg" width="100%" style="float: left; display:inline-block;"/>


## 15.03.2018 Simple UV Texture mapping
<img src="/screenshots/simpleUVTexturing.png" width="100%" style="float: left; display:inline-block;"/>

## 13.03.2018 Assimp materials and models loading
<p>I have integrated project with assimp library. It enables engine to load models, their materials and textures. You can see that there is one additional image comparing to the post from the previous day. It represents specular component of material</p>
<img src="screenshots/specular_component_assimp_loading.png" width="100%" style="float: left; display:inline-block;"/>

## 12.03.2018 Basics of deferred & forward renderer have been implemented
<p>In the picture below you can see process of rendering. In the first one normal vectors, colors and positions of objects in world coordinates are rendered into three separate textures. Then, in the second pass these textures are used to calculate influence of lights placed on the scene and final color of each pixel</p>
<img src="screenshots/basic_shading.png" width="100%" style="float: left; display:inline-block;"/>

## 02.03.2018 Development process start
<p>I have created initial version of class diagram for the project. It has been constantly updated since the beginning of development.
I want to avoid major changes and in my opinion UML diagrams are the best way of doing it properly</p>
<img src="screenshots/class_diagram.png" width="100%" style="float: left; display:inline-block;"/>
