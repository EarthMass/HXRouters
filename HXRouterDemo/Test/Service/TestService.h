//
//  TestService.h
//  BeeHiveRouterDemo
//
//  Created by guohx on 2019/7/18.
//  Copyright © 2019 ghx. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol TestServiceProtocol <NSObject>

- (UIViewController *)getVC;
- (UIViewController *)getVCWithTitle:(NSString *)title other:(NSString *)other;

@end

@interface TestService : NSObject <TestServiceProtocol>


@end

NS_ASSUME_NONNULL_END
