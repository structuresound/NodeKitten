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
            freetype.osx.source_files = "deps/miguel/freetype/src/base/*.{h,c}"
            freetype.ios.source_files = "deps/miguel/freetype/src/base/ftadvanc.c",
                                        "deps/miguel/freetype/src/base/ftadvanc.c",
                                        "deps/miguel/freetype/src/base/ftbbox.c",
                                        "deps/miguel/freetype/src/base/ftbitmap.c",
                                        "deps/miguel/freetype/src/base/ftcalc.c",
                                        "deps/miguel/freetype/src/base/ftcid.c",
                                        "deps/miguel/freetype/src/base/ftdbgmem.c",
                                        "deps/miguel/freetype/src/base/ftfstype.c",
                                        "deps/miguel/freetype/src/base/ftgasp.c",
                                        "deps/miguel/freetype/src/base/ftgloadr.c",
                                        "deps/miguel/freetype/src/base/ftglyph.c",
                                        "deps/miguel/freetype/src/base/ftgxval.c",
                                        "deps/miguel/freetype/src/base/ftinit.c",
                                        "deps/miguel/freetype/src/base/ftlcdfil.c",
                                        "deps/miguel/freetype/src/base/ftmm.c",
                                        "deps/miguel/freetype/src/base/ftobjs.c",
                                        "deps/miguel/freetype/src/base/ftotval.c",
                                        "deps/miguel/freetype/src/base/ftoutln.c",
                                        "deps/miguel/freetype/src/base/ftpatent.c",
                                        "deps/miguel/freetype/src/base/ftpfr.c",
                                        "deps/miguel/freetype/src/base/ftrfork.c",
                                        "deps/miguel/freetype/src/base/ftsnames.c",
                                        "deps/miguel/freetype/src/base/ftstream.c",
                                        "deps/miguel/freetype/src/base/ftstroke.c",
                                        "deps/miguel/freetype/src/base/ftsynth.c",
                                        "deps/miguel/freetype/src/base/ftsystem.c",
                                        "deps/miguel/freetype/src/base/fttrigon.c",
                                        "deps/miguel/freetype/src/base/fttype1.c",
                                        "deps/miguel/freetype/src/base/ftutil.c",
                                        "deps/miguel/freetype/src/base/ftwinfnt.c",
                                        "deps/miguel/freetype/src/base/ftxf86.c"

            freetype.source_files = "deps/miguel/freetype/include/**/*.h",
                                    "deps/miguel/freetype/src/truetype/*.h",
                                    "deps/miguel/freetype/src/sfnt/*.h",
                                    "deps/miguel/freetype/src/autofit/*.h",
                                    "deps/miguel/freetype/src/smooth/*.h",
                                    "deps/miguel/freetype/src/raster/*.h",
                                    "deps/miguel/freetype/src/psaux/*.h",
                                    "deps/miguel/freetype/src/psnames/*.h",
                                    "deps/miguel/freetype/src/autofit/autofit.{h,c}",
                                    "deps/miguel/freetype/src/bdf/bdf.{h,c}",
                                    "deps/miguel/freetype/src/cff/cff.{h,c}",
                                    "deps/miguel/freetype/src/cid/type1cid.{h,c}",
                                    "deps/miguel/freetype/src/gzip/ftgzip.{h,c}",
                                    "deps/miguel/freetype/src/lzw/ftlzw.{h,c}",
                                    "deps/miguel/freetype/src/pcf/pcf.{h,c}",
                                    "deps/miguel/freetype/src/pfr/pfr.{h,c}",
                                    "deps/miguel/freetype/src/psaux/psaux.{h,c}",
                                    "deps/miguel/freetype/src/pshinter/pshinter.{h,c}",
                                    "deps/miguel/freetype/src/psnames/psmodule.{h,c}",
                                    "deps/miguel/freetype/src/raster/raster.{h,c}",
                                    "deps/miguel/freetype/src/sfnt/sfnt.{h,c}",
                                    "deps/miguel/freetype/src/smooth/smooth.{h,c}",
                                    "deps/miguel/freetype/src/truetype/truetype.{h,c}",
                                    "deps/miguel/freetype/src/type1/type1.{h,c}",
                                    "deps/miguel/freetype/src/type42/type42.{h,c}",
                                    "deps/miguel/freetype/src/winfonts/winfnt.{h,c}"
            freetype.compiler_flags = '-DFT2_BUILD_LIBRARY=1 -DDARWIN_NO_CARBON -DFT_CONFIG_OPTION_SYSTEM_ZLIB=1'
            freetype.xcconfig = { "HEADER_SEARCH_PATHS" => "$(PODS_ROOT)/deps/miguel/freetype/include" }
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
    png.subspec "zlib" do |zlib|
        zlib.source_files = "deps/leif/zlib/*.{h,c}"
    end
end

s.ios.frameworks = "UIKit", "OpenGLES"
s.osx.frameworks = "AppKit", "OpenGL"

s.resources = "blocks/leif/NodeKittenX/Examples/Assets/*.{png,jpg}",
              "deps/leif/freetype-gl/fonts/*.ttf",
              "deps/leif/freetype-gl/shaders/*.{vert,frag}"

s.requires_arc = true
s.xcconfig  =  { "CLANG_CXX_LIBRARY" => "libc++",
                 "CLANG_CXX_LANGUAGE_STANDARD" => "c++14",
                 "GCC_OPTIMIZATION_LEVEL" => "3"
                 }

end
