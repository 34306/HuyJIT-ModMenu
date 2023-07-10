//
//  ImGuiLoad.h
//  ImGuiTest
//
//  Created by yiming on 2021/6/2.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ImGuiLoad : NSObject

+ (instancetype)share;

///
- (void)show;
- (void)hide;

@end

NS_ASSUME_NONNULL_END
