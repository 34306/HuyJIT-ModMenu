
#import "JHUIViewControllerDecoupler.h"

@implementation JHUIViewControllerDecoupler

+ (UIViewController *)jh_controllerFromString:(NSString *)string
                                     paramter:(NSDictionary *)dictionary
{
    // vc from string.
    UIViewController *vc = [NSClassFromString(string) new];
    
    // if vc is nil.
    if (vc == nil) {
        return [self jh_notice_vc:string];
    }
    
    // just KVC it.
    for (NSString *key in dictionary.count > 0 ? dictionary.allKeys : @[]) {
        [vc setValue:dictionary[key] forKey:key];
    }
    
    return vc;
}

+ (UIViewController *)jh_notice_vc:(NSString *)string
{
#if DEBUG
    UIViewController *vc = [[UIViewController alloc] init];
    vc.view.backgroundColor = [UIColor whiteColor];
    vc.title = @"oops~😅";
    
    [vc.view addSubview:({
        
        NSString *text = @"Can't find this class\n\n"
                          "'%@'\n\n"
                          "in your project.😅\n\n"
                          "👇\n"
                          "Forget to import it\n"
                          "or\n"
                          "Doesn't create it yet~";
        
        UILabel *label = [[UILabel alloc] init];
        label.frame = [UIScreen mainScreen].bounds;
        label.text = [NSString stringWithFormat:text,string];
        label.textColor = [UIColor blackColor];
        label.font = [UIFont boldSystemFontOfSize:18];
        label.textAlignment = NSTextAlignmentCenter;
        label.numberOfLines = 0;
        label;
    })];
    return vc;
#else
    return nil;
#endif
}
@end

@interface UIViewController (JHUIViewControllerDecoupler)
@end

@implementation UIViewController (JHUIViewControllerDecoupler)
- (void)setValue:(id)value forUndefinedKey:(NSString *)key{
    NSLog(@"[<%@ %p> %s]: this class is not key value coding-compliant for the key: %@",NSStringFromClass([self class]),self,__FUNCTION__,key);
}
@end

