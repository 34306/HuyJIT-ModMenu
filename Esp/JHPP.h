
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

/// A tool for  Push & Present ViewController
@interface JHPP : NSObject


/**
 Fetch current ViewController at the top.
 */
+ (UIViewController *)currentViewController;

/**
 Push a viewController.
 
 @param vc a viewController.
 @param responder a view or viewController.
 */
+ (void)pushVC:(UIViewController *)vc from:(id)responder;
+ (void)pushVC:(UIViewController *)vc from:(id)responder animated:(BOOL)animated;

/**
 Push a viewController.
 
 @param vcString name of viewController.
 @param dic viewController's property.
 @param responder a view or viewController.
 */
+ (void)pushVC:(NSString *)vcString paramter:(NSDictionary *)dic from:(id)responder;
+ (void)pushVC:(NSString *)vcString paramter:(NSDictionary *)dic from:(id)responder animated:(BOOL)animated;

/**
 Present a viewController.
 
 @param vc a viewController.
 @param responder a view or viewController.
 */
+ (void)presentVC:(UIViewController *)vc from:(id)responder;
+ (void)presentVC:(UIViewController *)vc from:(id)responder animated:(BOOL)animated;
+ (void)presentVC:(UIViewController *)vc from:(id)responder navigation:(BOOL)navigation;
+ (void)presentVC:(UIViewController *)vc from:(id)responder navigation:(BOOL)navigation animated:(BOOL)animated;

/**
 Present a viewController.
 
 @param vcString name of viewController.
 @param dic viewController's property.
 @param responder a view or viewController.
 */
+ (void)presentVC:(NSString *)vcString paramter:(NSDictionary *)dic from:(id)responder;
+ (void)presentVC:(NSString *)vcString paramter:(NSDictionary *)dic from:(id)responder animated:(BOOL)animated;
+ (void)presentVC:(NSString *)vcString paramter:(NSDictionary *)dic from:(id)responder navigation:(BOOL)navigation;
+ (void)presentVC:(NSString *)vcString paramter:(NSDictionary *)dic from:(id)responder navigation:(BOOL)navigation animated:(BOOL)animated;

@end
