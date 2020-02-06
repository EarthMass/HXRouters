//
//  AppDelegate.m
//  BeeHiveRouterDemo
//
//  Created by guohx on 2019/7/18.
//  Copyright © 2019 ghx. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

   
#if DEBUG
    [HXAnnotationMananger shareManager].enableException = YES;
#endif

    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];

    ViewController * viewC = [[ ViewController alloc] init];
    UINavigationController * nav = [[UINavigationController alloc] initWithRootViewController:viewC];

    [self.window setRootViewController:nav];
    [self.window makeKeyAndVisible];

    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.

}


@end
