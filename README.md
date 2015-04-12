# NodeKittenX

NodeKittenX is an experimental cross-platform openGL toolkit

It is designed to be light-weight and be embedded in native platform tools, rather than being its own platform.

Traditional platform abstraction is provided through SDL.

To use this project with SDL on X86 / Linux use the biicode and point to the NodeKittenX block

#include leif/NodekittenX/NodeKitten.h

Modern platform abstraction is prolsvided through native hooks ie on iOS use NKUIView

To use this project inside an XCode project use CocoaPods

pod 'NodeKittenX' ~> '0.1.0'

to do: find a good Android dependency manager