//
//  UILabel+BonMotUtilities.h
//  BonMot
//
//  Created by Nora Trapp on 3/2/16.
//
//

@import UIKit;

@protocol BONTextable;

@interface UILabel (BonMotUtilities)

/**
 *  Assign a @c BONTextable object to apply to the label text. When a new value is assigned to @c bonString the chain attributes will be applied.
 */
@property (copy, nonatomic) id<BONTextable> bonTextable;

/**
 *  Assigning a string via this method will apply the attributes of any assigned @c BONTextable and set the @c attributedText with the resulting @c NSAttributedString.
 *
 *  @param string The text to be displayed.
 */
- (void)setBonString:(NSString *)string;

@end
