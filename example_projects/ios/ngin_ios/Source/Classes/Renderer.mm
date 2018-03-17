#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <memory>

#import "Renderer.h"
#import "../../../../../source_code/umbrellaHeader.hpp"
#import "../../../../../source_code/scene/ogl/geometryOGL.hpp"
#import "../../../../../source_code/scene/ogl/sceneNodeOGL.hpp"

std::unique_ptr<ms::NGin> engine;


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

	glGenFramebuffers(1, &_defaultFBOName);

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
	
	NSString* frvsFP = [[NSBundle mainBundle] pathForResource:@"vshader" ofType:@"glsl"];
	NSString* frfsFP = [[NSBundle mainBundle] pathForResource:@"fshader" ofType:@"glsl"];
	NSString* drGvsgFP = [[NSBundle mainBundle] pathForResource:@"g_buf_vshader" ofType:@"glsl"];
	NSString* drGfsgFP = [[NSBundle mainBundle] pathForResource:@"g_buf_fshader" ofType:@"glsl"];
	NSString* drLvsgFP = [[NSBundle mainBundle] pathForResource:@"lighting_vshader" ofType:@"glsl"];
	NSString* drLfsgFP = [[NSBundle mainBundle] pathForResource:@"lighting_fshader" ofType:@"glsl"];

	NSString* frvs = [[NSString alloc] initWithContentsOfFile:frvsFP encoding:NSUTF8StringEncoding error:nil];
	NSString* frfs = [[NSString alloc] initWithContentsOfFile:frfsFP encoding:NSUTF8StringEncoding error:nil];

	NSString* drGvsg = [[NSString alloc] initWithContentsOfFile:drGvsgFP encoding:NSUTF8StringEncoding error:nil];
	NSString* drGfsg = [[NSString alloc] initWithContentsOfFile:drGfsgFP encoding:NSUTF8StringEncoding error:nil];
	NSString* drLvsg = [[NSString alloc] initWithContentsOfFile:drLvsgFP encoding:NSUTF8StringEncoding error:nil];
	NSString* drLfsg = [[NSString alloc] initWithContentsOfFile:drLfsgFP encoding:NSUTF8StringEncoding error:nil];

	std::shared_ptr<std::string> fvS = std::shared_ptr<std::string>(new std::string([frvs cStringUsingEncoding:NSUTF8StringEncoding]));
	std::shared_ptr<std::string> ffS = std::shared_ptr<std::string>(new std::string([frfs cStringUsingEncoding:NSUTF8StringEncoding]));

	std::shared_ptr<std::string> gvs = std::shared_ptr<std::string>(new std::string([drGvsg cStringUsingEncoding:NSUTF8StringEncoding]));
	std::shared_ptr<std::string> gfs = std::shared_ptr<std::string>(new std::string([drGfsg cStringUsingEncoding:NSUTF8StringEncoding]));
	std::shared_ptr<std::string> qvs = std::shared_ptr<std::string>(new std::string([drLvsg cStringUsingEncoding:NSUTF8StringEncoding]));
	std::shared_ptr<std::string> qfs = std::shared_ptr<std::string>(new std::string([drLfsg cStringUsingEncoding:NSUTF8StringEncoding]));

	float width = backingWidth;
	float height = backingHeight;

	engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(fvS, ffS, gvs, gfs, qvs, qfs, width, height, width, height, 0.01, 100, 90, width/height, _defaultFBOName));
	std::unique_ptr<ms::Loader> loader = std::unique_ptr<ms::Loader>(new ms::LoaderOGL());
	
	NSString* model = [[NSBundle mainBundle] pathForResource:@"classroom" ofType:@"obj"];
	std::string modelPath = std::string([model cStringUsingEncoding:NSUTF8StringEncoding]);
	
	engine->load_model(modelPath);
	
	engine->scene->set_directional_light(50, ms::math::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, ms::math::vec3{ -1.0f, -1.0f, -1.0f });

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
