# HXRouter
路由库，可设置映射。

# 引入
`pod 'HXRouter'`

# 兼容 iOS8+

#使用

```
1、
//配置 在pch预编译文件中，方便全局调用
#import "HXRouterUtil.h"

2、
//注册 协议与类的绑定
@HXRouterRegister(TestServiceProtocol, TestService) //协议实现对应，写入配置中【不能放.h】

@interface TestService()


@end

@implementation TestService
3、
+ (void)load {    
    /*
    路由映射
    如:  路由字符             实际调用
    /dddd/enter  对应 /DDProtocol/test:aa:bb##param1&param2
    */
    [[HXAnnotationMananger shareManager] addRouterMappingWithRouterStr:@"/test/goVCWithTitle" mapping:@"TestServiceProtocol/getVCWithTitle:other:##title&other"];

    [[HXAnnotationMananger shareManager] addRouterMappingWithRouterStr:@"/test/goVC" mapping:@"TestServiceProtocol/getVC"];


}

4、
//调用
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

```

