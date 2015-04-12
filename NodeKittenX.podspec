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
s.version          = "0.1.0"
s.summary          = "experimental GL(es)2 based UI Framework"
s.homepage         = "https://github.com/<GITHUB_USERNAME>/NodeKitten"
s.license          = "MIT"
s.author           = { "structuresound" => "leif@structuresound.com" }

s.requires_arc = true

s.ios.frameworks = "UIKit", "MapKit"
s.osx.frameworks = "AppKit"

s.source_files = "blocks/leif/NodeKittenX/*.{h}","blocks/leif/NodeKittenX/Apple/*.{h,mm}","blocks/leif/NodeKittenX/Apple/*.{h,cpp}","blocks/leif/NodeKittenX/Event/*.{h,cpp}","blocks/leif/NodeKittenX/Node/*.{h,cpp}","blocks/leif/NodeKittenX/Shader/*.{h,cpp}","blocks/leif/NodeKittenX/Texture/*.{h,cpp}","blocks/leif/NodeKittenX/Types/*.{h,cpp}","blocks/leif/NodeKittenX/Utils/*.{h,cpp}","blocks/leif/NodeKittenX/View/NKView.{h,cpp}"

s.subspec "zlib" do |ss|
ss.source_files = "deps/leif/zlib/*.{h,c}"
end

s.subspec "png" do |ss|
ss.public_header_files = "deps/leif/png/*.h"
ss.source_files = "deps/leif/png/*.{h,c}"
end

s.subspec "picojpeg" do |ss|
ss.public_header_files = "deps/leif/picojpeg/*.h"
ss.source_files = "deps/leif/picojpeg/*.{h,c,cpp}"
end

end
