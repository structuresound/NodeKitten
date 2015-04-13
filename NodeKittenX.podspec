#
# Be sure to run `pod lib lint NodeKitten.podspec" to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# Any lines starting with a # are optional, but encouraged
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|

s.name             = "NodeKittenX"
s.version          = "0.1.1"
s.summary          = "experimental GL(es) framework written in c++11"
s.homepage         = "https://github.com/structuresound/NodeKittenX"
s.license          = "MIT"
s.author           = { "structuresound" => "leif@structuresound.com" }
s.source           = {:git => 'git://github.com/structuresound/NodeKittenX.git',
                      :tag => '0.1.1'
                     }
s.ios.deployment_target = "6.0"
s.osx.deployment_target = "10.9"

s.source_files = "blocks/leif/NodeKittenX/*.{h}","blocks/leif/NodeKittenX/Platform/Cocoa/*.{h,mm,cpp}","blocks/leif/NodeKittenX/Event/*.{h,cpp}","blocks/leif/NodeKittenX/Node/*.{h,cpp}","blocks/leif/NodeKittenX/Shader/*.{h,cpp}","blocks/leif/NodeKittenX/Texture/*.{h,cpp}","blocks/leif/NodeKittenX/Types/*.{h,cpp}","blocks/leif/NodeKittenX/Utils/*.{h,cpp}","blocks/leif/NodeKittenX/View/NKView.{h,cpp}","blocks/leif/NodeKittenX/Examples/*.{h,cpp}"

s.subspec "png" do |ss|
ss.source_files = "deps/leif/png/*.{h,c}"
ss.subspec "zlib" do |sss|
sss.source_files = "deps/leif/zlib/*.{h,c}"
end
end

s.subspec "picojpeg" do |ss|
ss.source_files = "deps/leif/picojpeg/*.{h,c,cpp}"
end

s.ios.frameworks = "UIKit"
s.osx.frameworks = "AppKit"

s.resources = 'blocks/leif/NodeKittenX/Examples/Assets/*.{png,jpg}'

s.requires_arc = true
s.xcconfig  =  { "CLANG_CXX_LIBRARY" => "libc++",
                 "GCC_OPTIMIZATION_LEVEL" => "3"
                 }

end
