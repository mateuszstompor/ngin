#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <memory>

#import "Renderer.h"
#import "../../../source_code/umbrellaHeader.hpp"

using namespace ms;
using namespace math;

std::unique_ptr<ms::Framebuffer> framebuffer;
std::unique_ptr<ms::Renderbuffer> depthRenderBuffer;

extern std::unique_ptr<ms::NGin> engine;

extern CGPoint translation;
extern CGPoint rotation;

using namespace ms;
using namespace std;
using namespace math;

@interface Renderer ()
{
    __weak EAGLContext * _context;
    GLuint _colorRenderbuffer;
}
@end

@implementation Renderer

- (instancetype)initWithContext:(EAGLContext*)context AndDrawable:(id<EAGLDrawable>)drawable {

    _context = context;
    
    framebuffer = make_unique<Framebuffer>(1, 1, 680, 375);
    framebuffer->use();
    
    glGenRenderbuffers(1, &_colorRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
	[_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:drawable];
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);

	GLint backingWidth;
	GLint backingHeight;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    depthRenderBuffer = std::make_unique<ms::Renderbuffer>(ms::Texture2D::Format::depth_24,
                                                           ms::Texture2D::AssociatedType::UNSIGNED_INT,
                                                           0,
                                                           backingWidth,
                                                           backingHeight);
    
    framebuffer->bind_depth_buffer(std::move(depthRenderBuffer));
	
    framebuffer->configure();

    float width = backingWidth;
    float height = backingHeight;
    unsigned int shadowResolution = 1024;
    auto cam = std::make_unique<ms::PerspectiveCamera>(0.01f, 100, 90, float(backingWidth)/backingHeight);

    engine = std::make_unique<NGin>(width, height, backingWidth, backingHeight, shadowResolution, std::move(cam), std::move(framebuffer));

    NSString* model = [[NSBundle mainBundle] pathForResource:@"sponza" ofType:@"obj"];
    std::string modelPath = std::string([model cStringUsingEncoding:NSUTF8StringEncoding]);
    
    NSString* sphereModel = [[NSBundle mainBundle] pathForResource:@"sphere" ofType:@"obj"];
    std::string sphereModelPath = std::string([sphereModel cStringUsingEncoding:NSUTF8StringEncoding]);
    
    NSString* coneModel = [[NSBundle mainBundle] pathForResource:@"cone" ofType:@"obj"];
    std::string coneModelPath = std::string([coneModel cStringUsingEncoding:NSUTF8StringEncoding]);
    
    engine->scene.set_directional_light(std::make_unique<DirectionalLight>(ms::math::vec3{ 1.0f, 1.0f, 1.0f}, 50, vec3(0.0f, -1.0f, 0.0f).normalized(), true));

    mat4 lookat = transform::look_at(vec3(0.0f, 4.0f, 0.0f),
                                     vec3( 0.0f, 0.0f,  0.0f),
                                     vec3( 0.0f, 0.0f,  1.0f));
    
    engine->scene.get_directional_light()->get_transformation() = lookat;
    
    SpotLight sl1({1.0f, 0.0f, 0.0f}, 50.0f, {1.0f, 0.0f, 0.0f}, 50.0f, {1.0f, 0.0f, 0.0f}, true);
    engine->scene.get_spot_lights().push_back(sl1);
    
    engine->load_model(modelPath);

    for(int i = 0; i < engine->scene.get_nodes().size(); ++i) {
        engine->scene.get_nodes()[i]->transformation = ms::math::transform::scale<float, 4>({0.02f, 0.02f, 0.02f}) * engine->scene.get_nodes()[i]->transformation;
    }

    engine->load();
	
	if (nil == self) {
		glDeleteRenderbuffers(1, &_colorRenderbuffer);
	}
	
	return self;
}

- (void)render {

    
    vec3 camDir = back(engine->scene.get_camera().get_transformation());
    vec3 camRight = -1.0f * right(engine->scene.get_camera().get_transformation());
    
    engine->scene.get_camera().get_transformation() = transform::rotate_about_y_radians<float, 4>(0.03f * rotation.x) * engine->scene.get_camera().get_transformation();
    engine->scene.get_camera().get_transformation() = transform::rotate_about_x_radians<float, 4>(-0.03f * rotation.y) * engine->scene.get_camera().get_transformation();
    
    engine->scene.get_camera().get_transformation() =  engine->scene.get_camera().get_transformation() * transform::translate<float, 4>(0.07f * camDir * translation.y);
    engine->scene.get_camera().get_transformation() =  engine->scene.get_camera().get_transformation() * transform::translate<float, 4>(0.07f * camRight * translation.x);
    engine->draw_scene();
	
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
	
}

- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer {

    GLint backingWidth;
    GLint backingHeight;

    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);

    // TELL RENDER THAT VIEW WAS RESIZED IN THIS PLACE !!
    //[super resizeWithWidth:backingWidth AndHeight:backingHeight];

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }

    return YES;
}

- (void)dealloc {
	
    engine->unload();
    
	if (_colorRenderbuffer) {
		glDeleteRenderbuffers(1, &_colorRenderbuffer);
		_colorRenderbuffer = 0;
	}
	
//    framebuffer->unload();
//    depthRenderBuffer->unload();
    engine = nullptr;
    depthRenderBuffer = nullptr;
    framebuffer = nullptr;
    
}

@end
