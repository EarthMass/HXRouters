//
//  TestService.m
//  BeeHiveRouterDemo
//
//  Created by guohx on 2019/7/18.
//  Copyright © 2019 ghx. All rights reserved.
//

#import "TestService.h"
#import "TestVC.h"

//注册 协议与类的绑定
@HXRouterRegister(TestServiceProtocol, TestService) //协议实现对应，写入配置中【不能放.h】

@interface TestService()

@property (nonatomic, copy) NSString * xxxx;

@end

@implementation TestService

+ (void)load {
    
    /*
    路由映射
    如:  路由字符             实际调用
    /dddd/enter  对应 /DDProtocol/test:aa:bb##param1&param2
    */
    [[HXAnnotationMananger shareManager] addRouterMappingWithRouterStr:@"/test/goVCWithTitle" mapping:@"TestServiceProtocol/getVCWithTitle:other:##title&other"];

    [[HXAnnotationMananger shareManager] addRouterMappingWithRouterStr:@"/test/goVC" mapping:@"TestServiceProtocol/getVC"];


}


- (UIViewController *)getVC {


    UIViewController * tmp = [TestVC new];
    tmp.title = @"666";
    return tmp;
}

- (UIViewController *)getVCWithTitle:(NSString *)title other:(NSString *)other {

        self.xxxx =  @"cx";

    UIViewController * tmp = [TestVC new];
    tmp.title =  [NSString stringWithFormat:@"%@-%@",title,other];
    return tmp;
}

@end
