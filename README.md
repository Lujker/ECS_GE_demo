# ECS_GE_demo
ECS_GE_demo

Build asimp and freetype and copy .lib and .dll file to ECS_GE_demo\libs\Windows for copy when proj bild to bin dir

Building ImageMagick Windows https://github.com/ImageMagick/ImageMagick-Windows on debug x64 version for MSVS
Then configur cmake dependence for finding lib and haders in cmakeGUI like:

-ImageMagick_EXECUTABLE_DIR: external\sabmodules\imagemagic\ImageMagick-Windows\VisualMagick\bin
-ImageMagick_Magick++_INCLUDE_DIR: external\sabmodules\imagemagic\ImageMagick-Windows\ImageMagick\Magick++\lib
-ImageMagick_Magick++_LIBRARY: external\sabmodules\imagemagic\ImageMagick-Windows\VisualMagick\lib\CORE_DB_Magick++_.lib

And like thet for MagickWand and MagickCore. Use ImageMagic on cpp for load images on any format.
Then cope dll from external\sabmodules\imagemagic\ImageMagick-Windows\VisualMagick\bin to ECS_GE_demo\libs\Windows for copy to bin dir
and rebild proj