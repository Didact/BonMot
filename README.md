<img width=443 src="Resources/readme-images/BonMot-logo.png" alt="BonMot Logo" />

[![Swift 2.x + 3.0](https://img.shields.io/badge/Swift-2.3%20+%203.0-orange.svg?style=flat)](https://swift.org)
[![CI Status](http://img.shields.io/travis/Raizlabs/BonMot.svg?style=flat)](https://travis-ci.org/Raizlabs/BonMot)
[![Version](https://img.shields.io/cocoapods/v/BonMot.svg?style=flat)](http://cocoapods.org/pods/BonMot)
[![License](https://img.shields.io/cocoapods/l/BonMot.svg?style=flat)](http://cocoapods.org/pods/BonMot)
[![Platform](https://img.shields.io/cocoapods/p/BonMot.svg?style=flat)](http://cocoapods.org/pods/BonMot)
[![Carthage compatible](https://img.shields.io/badge/Carthage-compatible-4BC51D.svg?style=flat)](https://github.com/Carthage/Carthage)

BonMot (pronounced *Bon Mo*, French for *good word*) is a Swift attributed string library. It abstracts away the complexities of the iOS, macOS, tvOS, and watchOS typography tools, freeing you to focus on making your text beautiful.

To run the example project, run `pod try BonMot`, or clone the repo and open `Example/BonMot-Example.xcworkspace`.

# Usage

In any Swift file where you want to use BonMot, simply `import BonMot`. 

## Basics

Use an `AttributedStringStyle` to specify the style of your attributed string. Then, use the `styled(with:)` method on `String` to get your attributed string:

```swift
let quote = "I used to love correcting people’s grammar until" +
            "I realized what I loved more was having friends.\n" +
            "-Mara Wilson"

let style = AttributedStringStyle.style(
    .font(UIFont(name: "AmericanTypewriter", size: 17)!),
    .lineHeightMultiple(1.8)
)

let attributedString = quote.styled(with: style)

// You can also get the style’s attributes dictionary
// if you’re using an API that requires it.
let attributes = style.attributes
```

### Style  Inheritance

Styles can inherit from each other, which lets you create multiple styles that share common attributes:

```swift
let baseStyle = AttributedStringStyle.style(
    .lineHeightMultiple(1.2),
    .font(UIFont.systemFont(ofSize: 17))
)

let redStyle = baseStyle.byAdding(.color(.red))
let blueStyle = baseStyle.byAdding(.color(.blue))

let string = "bird"

let redBirdString = string.styled(with: redStyle)
let blueBirdString = string.styled(with: redStyle)
```

### XML Parsing

Are you trying to style just part of a string, perhaps even a localized string which is different depending on the locale of the app? No problem! BonMot can turn custom XML tags and simple HTML into attributed strings:

```swift
let string = "one fish, two fish, <red>red fish</red>,<BON:noBreakSpace/><blue>blue fish</blue>"

let redStyle = AttributedStringStyle.style(.color(.red))
let blueStyle = AttributedStringStyle.style(.color(.blue))

let fishStyle = AttributedStringStyle.style(
    .font(UIFont.systemFont(ofSize: 17)),
    .lineHeightMultiple(1.8),
    .color(.darkGray),
    .xmlRules([
        .style("red", redStyle),
        .style("blue", blueStyle),
        ])
)

let attributedString = string.styled(with: fishStyle)
```

This will produce:

<img width=227 src="Resources/readme-images/fish-with-black-comma.png" />

> Note the use of `<BON:noBreakSpace/>` to specify a special character within the string. This is a great way to add special characters to localized strings, since localizers might not know to look for special characters, and many of them are invisible or ambiguous when viewed in a normal text editor. You can use any characters in the `Special` enum, or use `<BON:unicode value='A1338'/>` or `&#a1338;`

## Image Attachments

BonMot uses `NSTextAttachment` to embed images in strings. You can use BonMot’s `NSAttributedString.composed(of:)` API to chain images and text together in the same string:

```swift
let someImage = ... // some UIImage or NSImage

let attributedString = NSAttributedString.composed(of: [
    someImage.styled(with: .baselineOffset(-4)), // shift vertically if needed
    Special.noBreakSpace, // a non-breaking space between image and text
    "label with icon", // raw or attributed string
    ])
```

> Note the use of the `Special` type, which gives you easy access to Unicode characters that are commonly used in UIs, such as spaces, dashes, and non-printing characters.

Outputs:

<img width=116 height=22 src="Resources/readme-images/label-with-icon.png" />

If you need to wrap multiple lines of text after an image, use `Tab.headIndent(...)` to align the whole paragraph after the image:

```swift
let attributedString = NSAttributedString.composed(of: [
    someImage.styled(with: .baselineOffset(-4.0)), // shift vertically if needed
    Tab.headIndent(10), // horizontal space between image and text
    "This is some text that goes on and on and spans multiple lines, and it all ends up left-aligned",
    ])
```

Outputs:

<img width=285 src="Resources/readme-images/wrapped-label-with-icon.png" />

## Dynamic Type

You can easily make any attributed string generated by BonMot respond to the system text size control. Simply add `.adapt` to any style declaration, and specify whether you want the style to scale like a `.control` or like `.body` text:

```swift
let style = AttributedStringStyle.style(
    .font(UIFont(name: "AmericanTypewriter", size: 17)!),
    .lineHeightMultiple(1.8)
    .adapt(.control) // <------------------ this line
)

someLabel.attributedText = "Label".styled(with: style)
```

`.control` and `.body` both scale the same, except that when enabling the "Larger Dynamic Type" accessibility setting, `.body` grows unbounded. Here is a graph of the default behaviors of the [system Dynamic Type styles](https://developer.apple.com/ios/human-interface-guidelines/visual-design/typography/):

<img width=443 src="Resources/readme-images/ios-type-scaling-behavior.png" alt="Graph of iOS Dynamic Type scaling behavior, showing that Control text tops out at the XXL size, but Body text keeps growing all the way up to AccessibilityXXL" />

## Debugging & Testing Helpers

Use `bonMotDeubgString` and `bonMotDebugAttributedString` to print out a version of any attributed string with all of the special characters and image attachments expanded into human-readable XML:

```swift
NSAttributedString.composed(of: [
	image,
	Special.noBreakSpace,
	"Monday",
	Special.enDash,
	"Friday"
	]).bonMotDebugString

// Result:
// <BON:image size='36x36'/><BON:noBreakSpace/>Monday<BON:enDash/>Friday
```

You can use [XML Rules](#xml-parsing) to re-parse the resulting string (except for images) back into an attributed string. You can also save the output of `bonMotDebugString` and use it to validate attributed strings in unit tests.

## Storyboard and XIB Integration

### TODO: write this section

## Updating

## Vertical Text Alignment

UIKit lets you align labels by top, bottom, or baseline. BonMot includes `TextAlignmentConstraint`, a layout constraint subclass that lets you align labels by cap height and x-height. For some fonts, this is essential to convey the designer’s intention:

<img width=320 src="Resources/readme-images/text-alignment.png" alt="Illustration of different methods of aligning text vertically" />

`TextAlignmentConstraint` works with any views that expose a `font` property. It uses Key-Value Observing to watch for changes to the `font` property, and adjust its internal measurements accordingly. This is ideal for use with Dynamic Type: if the user changes the font size of the app, `TextAlignmentConstraint` will update. You can also use it to align a label with a plain view, as illustrated by the red dotted line views in the example above.

**Warning:** `TextAlignmentConstraint` holds strong references to its `firstItem` and `secondItem` properties. Make sure that a view that is constrained by this constraint does not also hold a strong reference to said constraint, because it will cause a retain cycle.

You can use `TextAlignmentConstraint` programmatically or in Interface Builder. In code, use it like this:

```swift
TextAlignmentConstraint(
    item: someLabel,
    attribute: capHeight,
    relatedBy: .equal,
    toItem: someOtherLabel,
    attribute: capHeight).active = true
```

In Interface Builder, start by constraining two views to each other with a `top` constraint. Select the constraint, and in the Identity Inspector, change the class to `TextAlignmentConstraint`:

<img width=294 src="Resources/readme-images/text-alignment-identity-inspector.png" alt="setting the class in the Identity Inspector" />

Next, switch to the Attributes Inspector. `TextAlignmentConstraint` exposes two text fields through [IBInspectables](https://developer.apple.com/library/ios/recipes/xcode_help-IB_objects_media/Chapters/CreatingaLiveViewofaCustomObject.html). Type in the attributes you want to align. You will get a run-time error if you enter an invalid value.

<img width=294 src="Resources/readme-images/text-alignment-attributes-inspector.png" alt="setting the alignment attributes in the Attributes Inspector" />

The layout won’t change in Interface Builder (IBDesignable is not supported for constraint subclasses), but it will work when you run your code.

**Note:** some of the possible alignment values are not supported in all configurations. Check out [Issue #37](https://github.com/Raizlabs/BonMot/issues/37) for updates.

# Installation

## CocoaPods

BonMot is available through [CocoaPods](http://cocoapods.org). To install
it, simply add the following line to your Podfile:

```ruby
pod 'BonMot'
```

## Carthage

BonMot is also compatible with [Carthage](https://github.com/Carthage/Carthage). To install it, simply add the following line to your Cartfile:

```ogdl
github "Raizlabs/BonMot"
```

# Contributing

Issues and pull requests are welcome! Please format all code using [`clang-format`](http://clang.llvm.org/docs/ClangFormat.html) and the included `.clang-format` configuration file. Contributors are expected to abide by the [Contributor Covenant Code of Conduct](https://github.com/Raizlabs/BonMot/blob/master/CONTRIBUTING.md).

Please `brew install swiftlint` if you are going to be contributing to BonMot, so that any code you change is linted before you commit.

# Author

Zev Eisenberg: <mailto:zev.eisenberg@raizlabs.com>, [@ZevEisenberg](https://twitter.com/zeveisenberg)

Logo by Jon Lopkin: [@jonlopkin](https://twitter.com/jonlopkin)

# License

BonMot is available under the MIT license. See the LICENSE file for more info.
