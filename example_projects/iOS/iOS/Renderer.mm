#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <memory>

#import "Renderer.h"
#import "../../../source_code/umbrellaHeader.hpp"

std::unique_ptr<ms::NGin> engine;
std::shared_ptr<ms::FramebufferOGL> framebuffer;

using namespace ms;

@interface Renderer ()
{
	
    EAGLContext* _context;

    GLuint _colorRenderbuffer;
    GLuint _depthRenderbuffer;
	GLuint _defaultFBOName;
	
}
@end

@implementation Renderer

- (instancetype)initWithContext:(EAGLContext*)context AndDrawable:(id<EAGLDrawable>)drawable {

	framebuffer = FramebufferOGL::window_framebuffer(375, 680);
	
	glGenFramebuffers(1, &_defaultFBOName);
	framebuffer->set_underlying_id(_defaultFBOName);
	glGenRenderbuffers(1, &_colorRenderbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBOName);
	glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
	_context = context;
	
	[_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:drawable];
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);

	GLint backingWidth;
	GLint backingHeight;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);

	glGenRenderbuffers(1, &_depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		return nil;
	}

    float width = backingWidth;
    float height = backingHeight;

    engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(width, height, width, height, 0.01, 100, 90, width/height, framebuffer));
    std::unique_ptr<ms::Loader> loader = std::unique_ptr<ms::Loader>(new ms::LoaderOGL());

    NSString* model = [[NSBundle mainBundle] pathForResource:@"classroom" ofType:@"obj"];
    std::string modelPath = std::string([model cStringUsingEncoding:NSUTF8StringEncoding]);

    engine->load_model(modelPath);

    engine->scene->set_directional_light(50, ms::math::vec3{ 1.0f, 1.0f, 1.0f}, ms::math::vec3{ -1.0f, -1.0f, -1.0f });

    engine->scene->get_camera().set_transformation(ms::math::transform::translate<float, 4>({0, 0, -1}));
    engine->load();
	
	if (nil == self) {
		glDeleteFramebuffers(1, &_defaultFBOName);
		glDeleteRenderbuffers(1, &_colorRenderbuffer);
		glDeleteRenderbuffers(1, &_depthRenderbuffer);
	}
	
	return self;
}

- (void)render {

    glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBOName);

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
	
	glGenRenderbuffers(1, &_depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);
	
	// TELL RENDER THAT VIEW WAS RESIZED IN THIS PLACE !!
	// TELL RENDER THAT VIEW WAS RESIZED IN THIS PLACE !!
	// TELL RENDER THAT VIEW WAS RESIZED IN THIS PLACE !!
	//[super resizeWithWidth:backingWidth AndHeight:backingHeight];
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
	
    return YES;
}

- (void)dealloc {
	
	if (_defaultFBOName) {
		glDeleteFramebuffers(1, &_defaultFBOName);
		_defaultFBOName = 0;
	}
	
	if (_colorRenderbuffer) {
		glDeleteRenderbuffers(1, &_colorRenderbuffer);
		_colorRenderbuffer = 0;
	}
	
}

@end
