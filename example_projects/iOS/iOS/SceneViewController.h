//
//  SceneViewController.h
//  iOS
//
//  Created by Mateusz Stompór on 10/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "EAGLView.h"
#import "../../../source_code/umbrellaHeader.hpp"

@class MSCirclePad;

std::unique_ptr<ms::NGin> engine;


@interface SceneViewController : UIViewController

@property (weak, nonatomic) IBOutlet EAGLView *eaglView;
@property (weak, nonatomic) IBOutlet MSCirclePad *rotationCirclePad;
@property (weak, nonatomic) IBOutlet MSCirclePad *translationCirclePad;

@end
