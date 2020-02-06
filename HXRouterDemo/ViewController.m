//
//  ViewController.m
//  BeeHiveRouterDemo
//
//  Created by guohx on 2019/7/18.
//  Copyright © 2019 ghx. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    self.view.backgroundColor = [UIColor whiteColor];
    //按钮
    UIButton * btn = [[UIButton alloc] initWithFrame:CGRectMake(100, 100, 100, 50)];
    [btn addTarget:self action:@selector(goVcAction) forControlEvents:UIControlEventTouchUpInside];
    [btn setTitle:@"协议调用" forState:UIControlStateNormal];
    [btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [btn setBackgroundColor:[UIColor lightGrayColor]];
    [self.view addSubview:btn];

    UIButton * btn2 = [[UIButton alloc] initWithFrame:CGRectMake(100, 180, 100, 50)];
    [btn2 addTarget:self action:@selector(goVcActionRouter) forControlEvents:UIControlEventTouchUpInside];
    [btn2 setTitle:@"路由无参" forState:UIControlStateNormal];
    [btn2 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [btn2 setBackgroundColor:[UIColor lightGrayColor]];
    [self.view addSubview:btn2];

    UIButton * btn3 = [[UIButton alloc] initWithFrame:CGRectMake(100, 260, 100, 50)];
    [btn3 addTarget:self action:@selector(goVcActionRouterMutiParam) forControlEvents:UIControlEventTouchUpInside];
    [btn3 setTitle:@"路由多参" forState:UIControlStateNormal];
    [btn3 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [btn3 setBackgroundColor:[UIColor lightGrayColor]];
    [self.view addSubview:btn3];

}

- (void)goVcAction {
//获取对应的视图 或者方法
   UIViewController * vc = [GetServiceWithProtocolName(TestServiceProtocol) getVC];
 //传值
   VcParamsSet(vc, @{@"text1":@"1",@"text2":@"2",@"text3":@"3"});
   [self.navigationController pushViewController:vc animated:YES];

}

- (void)goVcActionRouter {

    //映射获取 ， 一般情况是 【接口】配置对应的 路由字符串来调用相应的对应，这样就必须对应起来
    /*
     路由名称必须与 配置的一样
     */
        NSString * routerStr = @"/test/goVC?text1=1111";
        NSDictionary * params = GetUrlParamtersWithUrlStr(routerStr);
        UIViewController * vc = GetServiceWithRouter(@"/test/goVC", nil);
        // //传值
        VcParamsSet(vc, params);
        [self.navigationController pushViewController:vc animated:YES];
}

- (void)goVcActionRouterMutiParam {

    //路由多参数
//    NSString * routerMutiParamStr = @"/test/goVCWithTitle";
//    NSDictionary * mutiParams = @{@"title":@"标题",@"other":@"其他"};
//    UIViewController * mutiParamVc = GetServiceWithRouter(@"/test/goVCWithTitle", mutiParams);

    //直接从协议中取
    NSString * routerMutiParamStr = @"/test/goVCWithTitle?title=白头&other=666";
    UIViewController * mutiParamVc = GetServiceWithRouter(routerMutiParamStr, nil);

    [self.navigationController pushViewController:mutiParamVc animated:YES];
}

@end
