//
//  HXAnnotationMananger.m
//  OPAnnotation
//
//  Created by guohx on 2018/10/17.
//  Copyright © 2018年. All rights reserved.
//

#import "HXAnnotationMananger.h"

#include <mach-o/getsect.h>
#include <mach-o/loader.h>
#include <mach-o/dyld.h>
#include <dlfcn.h>
#import <objc/runtime.h>
#import <objc/message.h>
#include <mach-o/ldsyms.h>

/*
 
 __attribute__((constructor))
 void initProphet() {
 _dyld_register_func_for_add_image(dyld_callback);
 }
 当一个函数被__attribute__((constructor))修饰时，表示这个函数是这个image的初始化函数，在image被加载时，首先会调用这个函数。（image指的是mach-o和动态共享库，在工程运行时，可以使用lldb命令image list查看这个工程中加载的所有image。）
 上述代码表示initProphet函数被指定为mach-o的初始化函数，当dyld（动态链接器）加载mach-o时，执行initProphet函数，其执行时机在man函数和类的load方法之前。
 
 当_dyld_register_func_for_add_image(dyld_callback);被执行时，如果已经加载了image，则每存在一个已经加载的image就执行一次dyld_callback函数，在此之后，每当有一个新的image被加载时，也会执行一次dyld_callback函数。
 （dyld_callback函数在image的初始化函数之前被调用，mach-o是第一个被加载的image，调用顺序是：load mach-o -> initProphet -> dyld_callback -> load other_image -> dyld_callback -> other_image_initializers -> ......）
 
 所以，当程序启动时，会多次调用dyld_callback函数。
 
 在dyld_callback函数中，使用下列函数来获取[步骤2]中存储的类名
 
 extern uint8_t *getsectiondata(
 const struct mach_header_64 *mhp,
 const char *segname,
 const char *sectname,
 unsigned long *size);
 
 
 */
NSArray<NSString *>* HXReadConfiguration(const char *sectionName,const struct mach_header *mhp);

static void dyld_callback(const struct mach_header *mhp, intptr_t vmaddr_slide)
{

    //获取路由映射
    /*
     字符对应 因为char 只能显示特殊字符 _ $

     传入路由映射例子:  $dqsl$xxx = 协议名称$方法名称__参数__$$获取的参数1__获取的参数2
     实际使用路由映射例子:  /dqsl/xxx = 协议名称/方法名称:参数:##获取的参数1&获取的参数2

     对应:
     / -> $
     : -> __
     ## ->$$
     & -> __

     */
    NSArray * routerMapping = HXReadConfiguration(HXRouterMappingSectName, mhp);
    for (NSString * mapping in routerMapping) {
        NSData *jsonData =  [mapping dataUsingEncoding:NSUTF8StringEncoding];
        NSError *error = nil;
        id json = [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:&error];
        if (!error) {
            if ([json isKindOfClass:[NSDictionary class]] && [json allKeys].count) {

                NSString * routerName = [json allKeys][0];
                NSString * impStr  = [json allValues][0];

                NSString * realImpStr = nil;

                routerName = [routerName stringByReplacingOccurrencesOfString:@"$" withString:@"/"];
                NSArray * methodArr = [impStr componentsSeparatedByString:@"$$"];
                if (methodArr.count == 2) {
                    //有参数
                    NSString * methodStr = methodArr[0];
                    methodStr = [methodStr stringByReplacingOccurrencesOfString:@"$" withString:@"/"];
                    methodStr = [methodStr stringByReplacingOccurrencesOfString:@"__" withString:@":"];

                    NSString * paramStr = methodArr[1];
                    paramStr = [paramStr stringByReplacingOccurrencesOfString:@"$" withString:@"&"];

                    realImpStr = [NSString stringWithFormat:@"%@##%@",methodStr,paramStr];

                } else if (methodArr.count == 1) {
                    //无参数
                    NSString * methodStr = methodArr[0];
                    methodStr = [methodStr stringByReplacingOccurrencesOfString:@"$" withString:@"/"];
                    methodStr = [methodStr stringByReplacingOccurrencesOfString:@"__" withString:@":"];

                    realImpStr = [NSString stringWithFormat:@"%@",methodStr];

                }

                if (routerName && realImpStr) {
                    [[HXAnnotationMananger shareManager] addRouterMappingWithRouterStr:routerName mapping:realImpStr];
                }

            }
        }
    }
    
  
    
}

__attribute__((constructor))
void initAnnotation() {
    _dyld_register_func_for_add_image(dyld_callback);
}

NSArray<NSString *>* HXReadConfiguration(const char *sectionName,const struct mach_header *mhp)
{
    NSMutableArray *configs = [NSMutableArray array];
    unsigned long size = 0;
#ifndef __LP64__
    uintptr_t *memory = (uintptr_t*)getsectiondata(mhp, SEG_DATA, sectionName, &size);
#else
    const struct mach_header_64 *mhp64 = (const struct mach_header_64 *)mhp;
    uintptr_t *memory = (uintptr_t*)getsectiondata(mhp64, SEG_DATA, sectionName, &size);
#endif
    
    unsigned long counter = size/sizeof(void*);
    for(int idx = 0; idx < counter; ++idx){
        char *string = (char*)memory[idx];
        NSString *str = [NSString stringWithUTF8String:string];
        if(!str)continue;
        
        NSLog(@"configMethod = %@", str);
        if(str) [configs addObject:str];
    }
    
    return configs;
    
    
}

@interface HXAnnotationMananger()

@property (nonatomic,strong) NSMutableDictionary * routerMappingDic;

@end

@implementation HXAnnotationMananger

static HXAnnotationMananger * manager = nil;

+(HXAnnotationMananger *)shareManager{
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        manager = [[HXAnnotationMananger alloc] initUniqueInstance];
        manager.routerMappingDic = [NSMutableDictionary dictionary];
    });
    return manager;
}

-(instancetype)initUniqueInstance {
    return [super init];
}

- (void)addRouterMappingWithRouterStr:(NSString *)router mapping:(NSString *)mapping {
    if (mapping && router) {
        [self.routerMappingDic setObject:mapping forKey:router];
    }

}
- (NSDictionary *)getRouterMappingAnnotation {
    return [NSDictionary dictionaryWithDictionary:self.routerMappingDic];
}

- (void)removerRouterMappingWithRounterName:(NSString *)rounter {
    [self.routerMappingDic removeObjectForKey:rounter];
}

- (id)createService:(Protocol *)proto {
    return [self createService:proto withServiceName:nil shouldCache:NO];
}

- (id)createService:(Protocol *)service withServiceName:(NSString *)serviceName shouldCache:(BOOL)shouldCache {
    if (!serviceName.length) {
        serviceName = NSStringFromProtocol(service);
    }
    id implInstance = nil;

    if (![self checkValidService:service]) {

        if (self.enableException) {
             @throw [NSException exceptionWithName:NSInternalInconsistencyException reason:[NSString stringWithFormat:@"%@ protocol does not been registed", NSStringFromProtocol(service)] userInfo:nil];
        }
    }

    NSString *serviceStr = serviceName;
    if (shouldCache) {
        id protocolImpl = [[self getRouterMappingAnnotation] objectForKey:serviceStr];
        if (protocolImpl) {
            return protocolImpl;
        }
    }

    Class implClass = [self serviceImplClass:service];

    implInstance = [[implClass alloc] init];
    if (shouldCache) {
        [[HXAnnotationMananger shareManager] addRouterMappingWithRouterStr:implInstance mapping:serviceStr];
        return implInstance;
    } else {
        return implInstance;
    }
    return [[implClass alloc] init];
}


#pragma mark - private
- (Class)serviceImplClass:(Protocol *)service
{
    NSString *serviceImpl = [self.routerMappingDic objectForKey:NSStringFromProtocol(service)];
    if (serviceImpl.length > 0) {
        return NSClassFromString(serviceImpl);
    }
    return nil;
}

- (BOOL)checkValidService:(Protocol *)service
{
    NSString *serviceImpl = [self.routerMappingDic objectForKey:NSStringFromProtocol(service)];
    if (serviceImpl.length > 0) {
        return YES;
    }
    return NO;
}


@end
