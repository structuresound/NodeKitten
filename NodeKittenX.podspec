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
s.version          = "0.1.4"
s.summary          = "experimental GL(es) framework written in c++"
s.homepage         = "https://github.com/structuresound/NodeKittenX"
s.license          = "MIT"
s.author           = { "structuresound" => "leif@structuresound.com" }
s.source           = {:git => 'https://github.com/structuresound/NodeKittenX.git',
                      :tag => '0.1.4'
                     }
s.ios.deployment_target = "6.0"
s.osx.deployment_target = "10.9"

s.source_files = "blocks/leif/NodeKittenX/*.{h}","blocks/leif/NodeKittenX/Platform/*.{h,cpp}","blocks/leif/NodeKittenX/Platform/Common/*.{h,cpp}","blocks/leif/NodeKittenX/Platform/Cocoa/*.{h,mm,cpp}","blocks/leif/NodeKittenX/Generic/*.{h,cpp}", "blocks/leif/NodeKittenX/Animation/*.{h,cpp}","blocks/leif/NodeKittenX/Node/*.{h,cpp}","blocks/leif/NodeKittenX/Shader/*.{h,cpp}","blocks/leif/NodeKittenX/Texture/*.{h,cpp}","blocks/leif/NodeKittenX/Types/*.{h,cpp}","blocks/leif/NodeKittenX/Utils/*.{h,cpp}","blocks/leif/NodeKittenX/View/*.{h,cpp}","blocks/leif/NodeKittenX/Examples/*.{h,cpp}"

s.subspec "freetypegl" do |freetypegl|
    freetypegl.source_files = "deps/leif/freetype-gl/*.{h,c,cpp}"

    freetypegl.subspec "freetype" do |freetype|
            freetype.source_files = "deps/leif/freetype/src/base/ftadvanc.c",
                                        "deps/leif/freetype/src/base/ftadvanc.c",
                                        "deps/leif/freetype/src/base/ftbbox.c",
                                        "deps/leif/freetype/src/base/ftbitmap.c",
                                        "deps/leif/freetype/src/base/ftcalc.c",
                                        "deps/leif/freetype/src/base/ftcid.c",
                                        "deps/leif/freetype/src/base/ftdbgmem.c",
                                        "deps/leif/freetype/src/base/ftfstype.c",
                                        "deps/leif/freetype/src/base/ftgasp.c",
                                        "deps/leif/freetype/src/base/ftgloadr.c",
                                        "deps/leif/freetype/src/base/ftglyph.c",
                                        "deps/leif/freetype/src/base/ftgxval.c",
                                        "deps/leif/freetype/src/base/ftinit.c",
                                        "deps/leif/freetype/src/base/ftlcdfil.c",
                                        "deps/leif/freetype/src/base/ftmm.c",
                                        "deps/leif/freetype/src/base/ftobjs.c",
                                        "deps/leif/freetype/src/base/ftotval.c",
                                        "deps/leif/freetype/src/base/ftoutln.c",
                                        "deps/leif/freetype/src/base/ftpatent.c",
                                        "deps/leif/freetype/src/base/ftpfr.c",
                                        "deps/leif/freetype/src/base/ftrfork.c",
                                        "deps/leif/freetype/src/base/ftsnames.c",
                                        "deps/leif/freetype/src/base/ftstream.c",
                                        "deps/leif/freetype/src/base/ftstroke.c",
                                        "deps/leif/freetype/src/base/ftsynth.c",
                                        "deps/leif/freetype/src/base/ftsystem.c",
                                        "deps/leif/freetype/src/base/fttrigon.c",
                                        "deps/leif/freetype/src/base/fttype1.c",
                                        "deps/leif/freetype/src/base/ftutil.c",
                                        "deps/leif/freetype/src/base/ftwinfnt.c",
                                        "deps/leif/freetype/src/base/ftxf86.c",

                                    "deps/leif/freetype/include/*.h",
                                    "deps/leif/freetype/src/truetype/*.h",
                                    "deps/leif/freetype/src/sfnt/*.h",
                                    "deps/leif/freetype/src/autofit/*.h",
                                    "deps/leif/freetype/src/smooth/*.h",
                                    "deps/leif/freetype/src/raster/*.h",
                                    "deps/leif/freetype/src/psaux/*.h",
                                    "deps/leif/freetype/src/psnames/*.h",
                                    "deps/leif/freetype/src/autofit/autofit.{h,c}",
                                    "deps/leif/freetype/src/bdf/bdf.{h,c}",
                                    "deps/leif/freetype/src/cff/cff.{h,c}",
                                    "deps/leif/freetype/src/cid/type1cid.{h,c}",
                                    "deps/leif/freetype/src/gzip/ftgzip.{h,c}",
                                    "deps/leif/freetype/src/lzw/ftlzw.{h,c}",
                                    "deps/leif/freetype/src/pcf/pcf.{h,c}",
                                    "deps/leif/freetype/src/pfr/pfr.{h,c}",
                                    "deps/leif/freetype/src/psaux/psaux.{h,c}",
                                    "deps/leif/freetype/src/pshinter/pshinter.{h,c}",
                                    "deps/leif/freetype/src/psnames/psmodule.{h,c}",
                                    "deps/leif/freetype/src/raster/raster.{h,c}",
                                    "deps/leif/freetype/src/sfnt/sfnt.{h,c}",
                                    "deps/leif/freetype/src/smooth/smooth.{h,c}",
                                    "deps/leif/freetype/src/truetype/truetype.{h,c}",
                                    "deps/leif/freetype/src/type1/type1.{h,c}",
                                    "deps/leif/freetype/src/type42/type42.{h,c}",
                                    "deps/leif/freetype/src/winfonts/winfnt.{h,c}"

            freetype.compiler_flags = '-DFT2_BUILD_LIBRARY=1 -DDARWIN_NO_CARBON -DFT_CONFIG_OPTION_SYSTEM_ZLIB=1'
            freetype.xcconfig = { "HEADER_SEARCH_PATHS" => "$(PODS_ROOT)/deps/leif/freetype/include" }
            freetype.libraries = "z"
        end
end

s.subspec "picojpeg" do |picojpeg|
picojpeg.source_files = "deps/leif/picojpeg/*.{h,c,cpp}"
end

s.subspec "kiwi" do |kiwi|
  kiwi.source_files = "deps/leif/kiwi/kiwi/*.{h, hpp}"
end

s.subspec "underscore" do |underscore|
underscore.source_files = "deps/leif/underscore/_.hpp",
                          "deps/leif/underscore/underscore/underscore.h",
                          "deps/leif/underscore/underscorehpp/*.hpp"
underscore.xcconfig = { "HEADER_SEARCH_PATHS" => "$(PODS_ROOT)/deps/leif/underscore" }
end

s.subspec "png" do |png|
    png.source_files = "deps/leif/png/*.{h,c}"
    png.libraries = "z"
end

s.ios.frameworks = "UIKit", "OpenGLES"
s.osx.frameworks = "AppKit", "OpenGL"

s.resources = "blocks/leif/NodeKittenX/Examples/Assets/*.{png,jpg,ttf}",
              "deps/leif/freetype-gl/fonts/*.ttf",
              "deps/leif/freetype-gl/shaders/*.{vert,frag}"

s.requires_arc = true
s.xcconfig  =  { "CLANG_CXX_LIBRARY" => "libc++",
                 "CLANG_CXX_LANGUAGE_STANDARD" => "c++14",
                 "GCC_OPTIMIZATION_LEVEL" => "3"
                 }

end
