#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

/// Controller decoupling & 控制器解耦
@interface JHUIViewControllerDecoupler : NSObject

/// string -> controller, with some parameters.return `nil` if not exist.
+ (UIViewController *)jh_controllerFromString:(NSString *)string
                                     paramter:(NSDictionary *)dictionary;

@end
