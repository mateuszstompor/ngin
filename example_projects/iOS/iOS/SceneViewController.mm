//
//  SceneViewController.m
//  iOS
//
//  Created by Mateusz Stompór on 10/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#import "SceneViewController.h"
#import "nGin_iOS-Swift.h"
#import <MSCirclePad/MSCirclePad-Swift.h>

using namespace ms;
using namespace math;

CGPoint translation;
CGPoint rotation;

@interface SceneViewController () <MSCirclePadDelegate>
@end

@implementation SceneViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [(EAGLView*)self.view startAnimation];
    self.translationCirclePad.delegate = self;
    self.rotationCirclePad.delegate = self;
}

- (void) viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [(EAGLView*)self.view stopAnimation];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void) joyPositionDidChangedWithSender:(MSCirclePad * _Nonnull)sender {
    if (sender == _translationCirclePad) {
        translation = sender.currentPosition;
    } else {
        rotation = sender.currentPosition;
    }
}

- (void) joyTouchRecognitionDidEndWithSender:(MSCirclePad * _Nonnull)sender {
    translation = CGPoint{0, 0};
    rotation = CGPoint{0, 0};
}

//- (void) joyTouchRecognitionDidStartWithSender:(MSCirclePad *)sender {
//
//}

@end
