#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <memory>

#import "Renderer.h"
#import "../../../source_code/umbrellaHeader.hpp"

using namespace ms;
using namespace math;

std::shared_ptr<ms::FramebufferOGL> framebuffer;
std::shared_ptr<ms::RenderbufferOGL> depthRenderBuffer;

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

    framebuffer = make_shared<FramebufferOGL>(1, 1, 680, 375);
    framebuffer->use();
    
    glGenRenderbuffers(1, &_colorRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
	_context = context;
	
	[_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:drawable];
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);

	GLint backingWidth;
	GLint backingHeight;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    depthRenderBuffer = std::make_shared<ms::RenderbufferOGL>(ms::Texture::Format::depth_16,
                                                              ms::Texture::AssociatedType::UNSIGNED_BYTE,
                                                              0,
                                                              backingWidth,
                                                              backingHeight);
    
    framebuffer->bind_depth_buffer(depthRenderBuffer);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		return nil;
	}

    float width = backingWidth;
    float height = backingHeight;

    engine = std::unique_ptr<NGin>(new NGinOGL(width, height, width, height, 0.01, 100, 90, width/height, framebuffer));
    std::unique_ptr<Loader> loader = std::unique_ptr<Loader>(new LoaderOGL());

    NSString* model = [[NSBundle mainBundle] pathForResource:@"classroom" ofType:@"obj"];
    std::string modelPath = std::string([model cStringUsingEncoding:NSUTF8StringEncoding]);

    engine->load_model(modelPath);

    engine->scene->set_directional_light(50, vec3{1.0f, 1.0f, 1.0f}, vec3{0.0f, 1.0f, 0.0f });
    engine->scene->cam->pre_transform(transform::translate<float, 4>(vec3(-0.6f, -0.3f, -1.0f)));
    engine->load();
	
	if (nil == self) {
		glDeleteRenderbuffers(1, &_colorRenderbuffer);
	}
	
	return self;
}

- (void)render {

    
    vec3 camDir = back(engine->scene->cam->get_transformation());
    vec3 camRight = -1.0f * right(engine->scene->cam->get_transformation());
    
    engine->scene->cam->pre_transform(transform::rotate_about_y_radians<float, 4>(0.01f * rotation.x));
    engine->scene->cam->pre_transform(transform::rotate_about_x_radians<float, 4>(-0.01f * rotation.y));
    
    engine->scene->cam->post_transform(transform::translate<float, 4>(0.01f * camDir * translation.y));
    engine->scene->cam->post_transform(transform::translate<float, 4>(0.01f * camRight * translation.x));
    
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
	
    framebuffer->unload();
    depthRenderBuffer->unload();
    engine = nullptr;
    depthRenderBuffer = nullptr;
    framebuffer = nullptr;
    
}

@end
