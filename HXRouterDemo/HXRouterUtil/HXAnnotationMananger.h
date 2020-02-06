//
//  HXAnnotationMananger.h
//  OPAnnotation
//
//  Created by guohx on 2018/10/17.
//  Copyright © 2018年 . All rights reserved.
//

#import <Foundation/Foundation.h>

/*
 调用  格式必须强制以这种方式，否者调用不到
@HXRouterSelector(xxx,xxx)

 字符对应 因为char 只能显示特殊字符 _ $

 传入路由映射例子:  $dqsl$xxx = 协议名称$方法名称__参数__$$获取的参数1__获取的参数2
 实际使用路由映射例子:  /dqsl/xxx = 协议名称/方法名称:参数:##获取的参数1&获取的参数2

 对应:
 / -> $
 : -> __
 ## ->$$
 & -> $

 如:  路由字符             实际调用
 /dddd/enter  对应 /DDProtocol/test:aa:bb##param1&param2
    $dddd$enter  对应 $DDProtocol$test__aa__$$pName1$pName2
*/

//存储区间路由调用映射
#ifndef HXRouterMappingSectName
#define HXRouterMappingSectName "HXRouterMapping"
#endif

//表示在项目的mach-o文件的名字为__DATA的segment中添加一个名字为  -XXX- 的section
#define HXDATA(sectname) __attribute((used, section("__DATA, "#sectname" ")))

//把协议实现 添加到 mach-o
#define HXRouterRegister(router,selectorName) \
class HXAnnotationMananger; char * k##router##selectorName##_Method HXDATA(HXRouterMapping) = "{ \""#router"\" : \""#selectorName"\" }";



#if DEBUG
#define HXLog(format,...) NSLog(format, ##__VA_ARGS__)
#else
#define HXLog(format,...)
#endif




@interface HXAnnotationMananger : NSObject


/**
 异常是否开启 默认否
 */
@property (nonatomic, assign) BOOL enableException;

- (id)init __attribute__((unavailable("init not available, call shareInstance instead")));
- (id)new __attribute__((unavailable("new not available, call shareInstance instead")));

+(HXAnnotationMananger *)shareManager;

/**
 获取协议对象

 @param proto 协议名称
 @return 对象
 */
- (id)createService:(Protocol *)proto;

- (void)addRouterMappingWithRouterStr:(NSString *)router mapping:(NSString *)mapping;
- (NSDictionary *)getRouterMappingAnnotation;
- (void)removerRouterMappingWithRounterName:(NSString *)rounter;




@end
