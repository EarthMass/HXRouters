//
//  HXRouterUti.h
//  HXRouterUtiDEMO
//
//  Created by guohx on 2019/7/18.
//  Copyright © 2019 ghx. All rights reserved.
//

#ifndef HXRouterUti_h
#define HXRouterUti_h

#import <UIKit/UIKit.h>
#import <objc/runtime.h>


#if __has_include(<MJExtension/MJExtension.h>)

#import <MJExtension/MJExtension.h>

#endif


#import "HXAnnotationMananger.h"

/**
 通过protocol name【类】 获取对应的 协议

 @param protocolName protocol类
 @return 协议对象
 */
#define GetServiceWithProtocolName(protocolName) [[HXAnnotationMananger shareManager] createService:@protocol(protocolName)]
/**
 通过protocol 对象获取对应的 协议

 @param protocol 对象
 @return 协议对象
 */
#define GetServiceWithProtocol(protocol) [[HXAnnotationMananger shareManager] createService:protocol]

#pragma mark- 多参数调用
static inline id PerformSelectorWithParams(id service,SEL aSelector, NSArray * objects);

static inline NSMutableDictionary * GetUrlParamtersWithUrlStr(NSString * urlStr);

#pragma mark- 无参数调用
//通过 映射出 调用的 协议/方法
static inline UIViewController * GetServiceWithRouter(NSString * urlProtocol, NSDictionary * paramsDic) {

    //string like： "ModuleWorkbenchServiceProtocol/getNewWebView";

    NSDictionary * urlParams = GetUrlParamtersWithUrlStr(urlProtocol);
    if (urlParams) {
        NSMutableDictionary * tmpParams = [NSMutableDictionary dictionaryWithDictionary:paramsDic];
        [tmpParams addEntriesFromDictionary:urlParams];
        paramsDic = tmpParams;
    }

    urlProtocol = [urlProtocol stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    //如果有参数的话
    urlProtocol = ([urlProtocol containsString:@"?"])?[[urlProtocol componentsSeparatedByString:@"?"] firstObject]:urlProtocol;

    NSDictionary * routerMappingDic = [[HXAnnotationMananger shareManager] getRouterMappingAnnotation];
    NSString * routerUrl = [routerMappingDic objectForKey:urlProtocol];
    if (!routerUrl.length) {
        NSLog(@"配置中没对应的路由映射ModuleMapping.string,找不到这个路由协议 --%@",routerUrl);
        return nil;
    }

    NSArray * components = [routerUrl componentsSeparatedByString:@"/"];
    if (components.count == 2) {
        Protocol * protocol = NSProtocolFromString(components[0]);
        if (!protocol) {
            return nil;
        }
        id service = GetServiceWithProtocol(protocol);

        //去掉##以及后面字符串
        NSString * methodSelector = [components[1] containsString:@"##"]?[[components[1] componentsSeparatedByString:@"##"] firstObject]:components[1];

        if ([service respondsToSelector:NSSelectorFromString(methodSelector)]) {

            //组装params 如果路由中存在 ## 说明有参数，参数通过 & 分隔
            //组装params 如果路由中存在 ## 说明有参数，参数通过 & 分隔
            NSMutableArray * keyArr = [NSMutableArray array];

            NSMutableArray * paramsArr = [NSMutableArray array];

            if ([routerUrl containsString:@"##"] && paramsDic) {
                NSString * keyStr = [[routerUrl componentsSeparatedByString:@"##"] lastObject];

                if ([keyStr containsString:@"&"]) {

                    [keyArr addObjectsFromArray:[keyStr componentsSeparatedByString:@"&"]];

                } else if (keyStr.length) {

                    [keyArr addObject:keyStr];

                }

                //组装数据  对应不上的直接 填写空值，否者一个参数出问题，参数顺序对应不上
                for (NSString * key in keyArr) {
                    if ([paramsDic valueForKey:key]) {
                        [paramsArr addObject:[paramsDic valueForKey:key]];
                    } else {
                        [paramsArr addObject:@""];
                    }
                }

            }

            UIViewController * vc = PerformSelectorWithParams(service, NSSelectorFromString(methodSelector), paramsArr);
            if (vc) {
                return vc;
            }
        }
        return nil;
    }
    return nil;
}

//直接就是 调用的 协议/方法
//static inline UIViewController * GetServiceWithRouterStr(NSString * routerUrl, NSDictionary * paramsDic) {
//
//    NSArray * components = [routerUrl componentsSeparatedByString:@"/"];
//    if (components.count == 2) {
//        Protocol * protocol = NSProtocolFromString(components[0]);
//        if (!protocol) {
//            return nil;
//        }
//        id service = GetServiceWithProtocol(protocol);
//
//        //去掉##以及后面字符串
//        NSString * methodSelector = [components[1] containsString:@"##"]?[[components[1] componentsSeparatedByString:@"##"] firstObject]:components[1];
//
//        if ([service respondsToSelector:NSSelectorFromString(methodSelector)]) {
//
//            //组装params 如果路由中存在 ## 说明有参数，参数通过 & 分隔
//            //组装params 如果路由中存在 ## 说明有参数，参数通过 & 分隔
//            NSMutableArray * keyArr = [NSMutableArray array];
//
//            NSMutableArray * paramsArr = [NSMutableArray array];
//
//            if ([routerUrl containsString:@"##"] && paramsDic) {
//                NSString * keyStr = [[routerUrl componentsSeparatedByString:@"##"] lastObject];
//
//                if ([keyStr containsString:@"&"]) {
//
//                    [keyArr addObjectsFromArray:[keyStr componentsSeparatedByString:@"&"]];
//
//                } else if (keyStr.length) {
//
//                    [keyArr addObject:keyStr];
//
//                }
//
//                //组装数据
//                for (NSString * key in keyArr) {
//                    if ([paramsDic valueForKey:key]) {
//                        [paramsArr addObject:[paramsDic valueForKey:key]];
//                    }
//                }
//
//            }
//
//            UIViewController * vc = PerformSelectorWithParams(service, NSSelectorFromString(methodSelector), paramsArr);
//            if (vc) {
//                return vc;
//            }
//        }
//        return nil;
//    }
//    return nil;
//}

#pragma mark- 多参数调用
//string like： "ModuleWorkbenchServiceProtocol/getNewWebView";
static inline id PerformSelectorWithParams(id service,SEL aSelector, NSArray * objects) {



    NSMethodSignature *methodSignature = [[service class] instanceMethodSignatureForSelector:aSelector];
    if(methodSignature == nil)
    {
        @throw [NSException exceptionWithName:@"抛异常错误" reason:@"没有这个方法，或者方法名字错误" userInfo:nil];
        return nil;
    }
    else
    {
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSignature];
        [invocation setTarget:service];
        [invocation setSelector:aSelector];
        //签名中方法参数的个数，内部包含了self和_cmd，所以参数从第3个开始
        NSInteger  signatureParamCount = methodSignature.numberOfArguments - 2;
        NSInteger requireParamCount = objects.count;
        NSInteger resultParamCount = MIN(signatureParamCount, requireParamCount);
        for (NSInteger i = 0; i < resultParamCount; i++) {
            id  obj = objects[i];
            [invocation setArgument:&obj atIndex:i+2];
        }

        [invocation invoke];
        //返回值处理
        id __unsafe_unretained callBackObject = nil;

        if(methodSignature.methodReturnLength)
        {
            [invocation getReturnValue:&callBackObject];
        }
        return callBackObject;
    }
}


static inline UIViewController * FindBestViewController(UIViewController* vc) {

    if (vc.presentedViewController) {

        // Return presented view controller
        return FindBestViewController(vc.presentedViewController);

    } else if ([vc isKindOfClass:[UISplitViewController class]]) {

        // Return right hand side
        UISplitViewController* svc = (UISplitViewController*) vc;
        if (svc.viewControllers.count > 0)
            return FindBestViewController(svc.viewControllers.lastObject);
        else
            return vc;

    } else if ([vc isKindOfClass:[UINavigationController class]]) {

        // Return top view
        UINavigationController* svc = (UINavigationController*) vc;
        if (svc.viewControllers.count > 0)
            return FindBestViewController(svc.topViewController);
        else
            return vc;

    } else if ([vc isKindOfClass:[UITabBarController class]]) {

        // Return visible view
        UITabBarController* svc = (UITabBarController*) vc;
        if (svc.viewControllers.count > 0)
            return FindBestViewController(svc.selectedViewController);
        else
            return vc;

    } else {

        // Unknown view controller type, return last child view controller
        return vc;

    }

}

#pragma mark - 当前界面
static inline UIViewController * CurrVC() {

    UIViewController* viewController = [UIApplication sharedApplication].keyWindow.rootViewController;

    return FindBestViewController(viewController);

}

#pragma mark- 从url中获取参数列表
static inline NSMutableDictionary * GetUrlParamtersWithUrlStr(NSString * urlStr) {
    NSRange range = [urlStr rangeOfString:@"?"];
    if (range.location == NSNotFound) {
        return nil;
    }
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    NSString *parametersString = [urlStr substringFromIndex:range.location + 1];
    if ([parametersString containsString:@"&"]) {
        NSArray *urlComponents = [parametersString componentsSeparatedByString:@"&"];
        for (NSString *keyValuePair in urlComponents) {
            NSArray *pairComponents = [keyValuePair componentsSeparatedByString:@"="];
            NSString *key = [pairComponents.firstObject stringByRemovingPercentEncoding];
            NSString *value = nil;
            if (pairComponents.count > 2) {
                value = [keyValuePair stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"%@=",key] withString:@""];
                value = [value stringByRemovingPercentEncoding];
            } else {
                value = [pairComponents.lastObject stringByRemovingPercentEncoding];
            }
            if (key == nil || value == nil) {
                continue;
            }
            id existValue = [params valueForKey:key];
            if (existValue != nil) {
                if ([existValue isKindOfClass:[NSArray class]]) {
                    NSMutableArray *items = [NSMutableArray arrayWithArray:existValue];
                    [items addObject:value];
                    [params setValue:items forKey:key];
                } else {
                    [params setValue:@[existValue, value] forKey:key];
                }
            } else {
                [params setValue:value forKey:key];
            }
        }
    } else {
        NSArray *pairComponents = [parametersString componentsSeparatedByString:@"="];
        if (pairComponents.count == 1) {
            return nil;
        }
        NSString *key = [pairComponents.firstObject stringByRemovingPercentEncoding];
        NSString *value = [pairComponents.lastObject stringByRemovingPercentEncoding];
        if (key == nil || value == nil) {
            return nil;
        }
        [params setValue:value forKey:key];
    }

    NSString * dicStr = nil;
#if __has_include(<MJExtension/MJExtension.h>)

    NSString * dicStr = [params mj_JSONString];
#else
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:params
                                                       options:NSJSONWritingPrettyPrinted
                                                         error:nil];

    dicStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];

#endif



    CGFloat version = [[UIDevice currentDevice].systemVersion floatValue];
    NSString* str = nil;
    if (version >= 9) {
        str = [dicStr stringByRemovingPercentEncoding];
    } else {
        str = [dicStr stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    }

    NSLog(@"strAfterDecodeByUTF8AndURI=%@", str);
//    params = [params mj_keyValues];
    return params;
}


#pragma mark- 参数传递

/**
 *  检测对象是否存在该属性
 */
static inline BOOL checkIsExistPropertyWithInstance(id instance, NSString * propertyName) {

    unsigned int outCount, i;

    // 获取对象里的属性列表
    objc_property_t * properties = class_copyPropertyList([instance
                                                           class], &outCount);

    for (i = 0; i < outCount; i++) {
        objc_property_t property =properties[i];
        //  属性名转成字符串
        NSString *propertyName = [[NSString alloc] initWithCString:property_getName(property) encoding:NSUTF8StringEncoding];
        // 判断该属性是否存在
        if ([propertyName isEqualToString:propertyName]) {
            free(properties);
            return YES;
        }
    }
    free(properties);

    return NO;
}

//runtime跳转

static inline void VcParamsSet(id vc, NSDictionary * paramsDic) {

    if (!vc) {
        return;
    }

    //类名(对象名)
    UIViewController * instance = (UIViewController *)vc;
    const char * className = [NSStringFromClass(instance.class)  cStringUsingEncoding:NSASCIIStringEncoding];
    Class newClass = [vc class];
    if (!newClass) {
        //创建一个类
        Class superClass = [NSObject class];
        newClass = objc_allocateClassPair(superClass, className, 0);
        //注册你创建的这个类
        objc_registerClassPair(newClass);
    }
    //下面是传值－－－－－－－－－－－－－－
    [paramsDic enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
        if (checkIsExistPropertyWithInstance(instance, key)) {
            //kvc给属性赋值

            [instance setValue:obj forKey:key];
        }else {
            NSLog(@"%@ 不包含key=%@的属性",NSStringFromClass(instance.class),key);
        }
    }];
}

#pragma mark- 页面跳转
static inline void VcPush(UIViewController * fromVC,UIViewController * toVC, NSDictionary * to_params, BOOL needLogin) {

    //拦截处理
//    if (needLogin && NO) {
//
//        UIAlertView * alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:@"您还未登录请先登录" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
//        [alertV show];
//
//    } else {
        VcParamsSet(toVC, to_params);
        [CurrVC().navigationController pushViewController:toVC animated:YES];
//    }



}


#endif /* HXRouterUti_h */
