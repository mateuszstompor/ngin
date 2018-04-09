#import "AppDelegate.h"
#import "EAGLView.h"

@implementation AppDelegate

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [self.window makeKeyAndVisible];
    
	[(EAGLView*)self.window.rootViewController.view startAnimation];

    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {

	[(EAGLView*)self.window.rootViewController.view stopAnimation];
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
	[(EAGLView*)self.window.rootViewController.view stopAnimation];
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
	[(EAGLView*)self.window.rootViewController.view startAnimation];
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	[(EAGLView*)self.window.rootViewController.view startAnimation];
}


- (void)applicationWillTerminate:(UIApplication *)application {
	[(EAGLView*)self.window.rootViewController.view stopAnimation];
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
}

@end
