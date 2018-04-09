#import <UIKit/UIKit.h>

#import "Renderer.h"

@interface EAGLView : UIView

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void) startAnimation;
- (void) stopAnimation;
- (void) drawView:(id)sender;

@end
