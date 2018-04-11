#import "EAGLView.h"
#import "Renderer.h"

@interface EAGLView ()
{
    Renderer* _renderer;
    EAGLContext* _context;
    NSInteger _animationFrameInterval;
    CADisplayLink* _displayLink;
}
@end

@implementation EAGLView

+ (Class) layerClass {
    return [CAEAGLLayer class];
}

- (instancetype) initWithCoder:(NSCoder*)coder {
    if ((self = [super initWithCoder:coder])) {

        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE],
                                        kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, 
                                        kEAGLDrawablePropertyColorFormat, nil];
		
		
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        
        if (!_context || ![EAGLContext setCurrentContext:_context]) {
            return nil;
		}
		
		_renderer = [[Renderer alloc] initWithContext:_context AndDrawable:(id<EAGLDrawable>)self.layer];
		
		if (!_renderer) {
            return nil;
		}
        
		_animating = FALSE;
		_animationFrameInterval = 1;
		_displayLink = nil;
    }
	
    return self;
}

- (void) drawView:(id)sender {
	[EAGLContext setCurrentContext:_context];
    [_renderer render];
}

- (void) layoutSubviews {
	[_renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger) animationFrameInterval {
	return _animationFrameInterval;
}

- (void) setAnimationFrameInterval:(NSInteger)frameInterval {
	if (frameInterval >= 1) {
		_animationFrameInterval = frameInterval;
		if (_animating) {
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

- (void) startAnimation {
	if (!_animating) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        [_displayLink setFrameInterval:_animationFrameInterval];
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		_animating = YES;
	}
}

- (void)stopAnimation {
	if (_animating) {
        [_displayLink invalidate];
        _displayLink = nil;		
		_animating = NO;
	}
}

- (void) dealloc {
	if ([EAGLContext currentContext] == _context)
        [EAGLContext setCurrentContext:nil];
}

@end
