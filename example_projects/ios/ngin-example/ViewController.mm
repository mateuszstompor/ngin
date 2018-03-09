//
//  ViewController.m
//  ngin-example
//
//  Created by Mateusz Stompór on 01/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#import "ViewController.h"
#import "/Users/mateuszstompor/Documents/ngin/source_code/umbrellaHeader.hpp"
#import "/Users/mateuszstompor/Documents/ngin/example_projects/mac/example_geometry.h"
#include "/Users/mateuszstompor/Documents/ngin/source_code/scene/ogl/geometryOGL.hpp"
#include "/Users/mateuszstompor/Documents/ngin/source_code/scene/ogl/sceneNodeOGL.hpp"
#include "/Users/mateuszstompor/Documents/ngin/source_code/rendering/shaders/ogl/shaderOGL.hpp"
#import <memory>

std::unique_ptr<ms::NGin> engine;

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	GLKView* renderView = (GLKView*)self.view;
	
	renderView.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
	
	// Configure renderbuffers created by the view
	renderView.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
	renderView.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	renderView.drawableStencilFormat = GLKViewDrawableStencilFormat8;
	self.preferredFramesPerSecond = 60;

	[EAGLContext setCurrentContext:[((GLKView*)[self view]) context]];
	//load

	NSString* frvsFP = [[NSBundle mainBundle] pathForResource:@"vshader" ofType:@"glsl"];
	NSString* frfsFP = [[NSBundle mainBundle] pathForResource:@"fshader" ofType:@"glsl"];
	NSString* drGvsgFP = [[NSBundle mainBundle] pathForResource:@"g_buf_vshader" ofType:@"glsl"];
	NSString* drGfsgFP = [[NSBundle mainBundle] pathForResource:@"g_buf_fshader" ofType:@"glsl"];
	NSString* drLvsgFP = [[NSBundle mainBundle] pathForResource:@"quad_vshader" ofType:@"glsl"];
	NSString* drLfsgFP = [[NSBundle mainBundle] pathForResource:@"quad_fshader" ofType:@"glsl"];

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
	
	std::shared_ptr<ms::Geometry> m = std::shared_ptr<ms::Geometry>(new ms::GeometryOGL());
	std::shared_ptr<ms::SceneNode> node = std::shared_ptr<ms::SceneNode>(new ms::SceneNodeOGL());
	
	float width = UIScreen.mainScreen.bounds.size.width;
	float height = UIScreen.mainScreen.bounds.size.height;
	
	engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(fvS, ffS, gvs, gfs, qvs, qfs, width, height, width, height, 0.01, 100, 90, width/height));
	
	m->vertices.insert(m->vertices.end(), &cube::vertices[0], &cube::vertices[108]);
	m->normals.insert(m->normals.end(), &cube::normals[0], &cube::normals[108]);
	
	node->geometry = m;
	
	engine->scene->set_directional_light(50, ms::math::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, ms::math::vec3{ -1.0f, -1.0f, -1.0f });
	
	engine->scene->nodes.push_back(node);
	engine->scene->get_camera().set_transformation(ms::math::transform::translate<float, 4>({0, 0, -1}));
	engine->load();
	
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
	engine->draw_scene();
}

-(void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:NO];
	engine->unload();
	engine = nullptr;
}

@end
