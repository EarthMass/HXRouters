//
//  TestVC.m
//  BeeHiveRouterDemo
//
//  Created by guohx on 2019/7/18.
//  Copyright © 2019 ghx. All rights reserved.
//

#import "TestVC.h"

@interface TestVC ()

@property (nonatomic, copy) NSString * text1;
@property (nonatomic, copy) NSString * text2;
@property (nonatomic, copy) NSString * text3;

@end

@implementation TestVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];

    NSString * str =  [NSString stringWithFormat:@"text1=%@,text2=%@,text3=%@",self.text1,self.text2,self.text3];
    UIAlertView * alertV = [[UIAlertView alloc] initWithTitle:@"提示" message:str delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
    [alertV show];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
