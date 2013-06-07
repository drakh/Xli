Thirdparty dependencies
=======================

The table below shows the list of libraries Xli uses internally, their licenses and how they are linked on the different target platforms.

Library       | License   | Component | Android | iOS     | OS X    | *nix    | Win32 
--------------|:---------:|:---------:|:-------:|:-------:|:-------:|:-------:|:-------:
glew          | BSD       | Core      | -       | -       | static  | dynamic | static
SDL2          | MIT       | Core      | -       | static  | static  | dynamic | -
ConvertUTF    | ?         | Core      | static  | static  | static  | static  | static
freetype      | BSD       | Media     | static  | static  | dynamic | dynamic | static
jpeg          | ?         | Media     | static  | -       | dynamic | dynamic | static
json_parser   | ?         | Media     | static  | static  | static  | static  | static
pnglib        | zlib      | Media     | static  | -       | dynamic | dynamic | static
tinyxml       | ?         | Media     | static  | static  | static  | static  | static
unzip         | ?         | Media     | static  | static  | static  | static  | static
zlib          | zlib      | Media     | dynamic | static  | dynamic | dynamic | static
portaudio     | ?         | Audio     | -       | -       | dynamic | dynamic | static
curl          | ?         | Http      | -       | -       | dynamic | dynamic | static
