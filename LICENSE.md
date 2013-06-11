License
=======

The Xli Library source code is released under a MIT-style license:

> Copyright (C) 2010-2013 Outracks Technologies AS
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**Note:** When linkning against Xli components you must also accept the licenses for the libraries Xli uses internally on your platform(s). See the section below for details.


3rdparty dependencies
---------------------

The table below shows the list of libraries Xli uses under the hood, their licenses and how they are linked on the different target platforms.

Library       | License   | Component | Android | iOS     | OS X    | *nix    | Win32 
--------------|:---------:|:---------:|:-------:|:-------:|:-------:|:-------:|:-------:
glew          | BSD-style | Core      | -       | -       | static  | dynamic | static
SDL2          | MIT-style | Core      | -       | static  | static  | dynamic | -
ConvertUTF    | ?         | Core      | static  | static  | static  | static  | static
freetype      | BSD       | Media     | static  | static  | static  | dynamic | static
[libjpeg]     | ?         | Media     | static  | -       | static  | dynamic | static
json_parser   | ?         | Media     | static  | static  | static  | static  | static
pnglib        | zlib      | Media     | static  | -       | static  | dynamic | static
tinyxml       | ?         | Media     | static  | static  | static  | static  | static
unzip         | ?         | Media     | static  | static  | static  | static  | static
zlib          | zlib      | Media     | dynamic | static  | dynamic | dynamic | static
portaudio     | ?         | Audio     | -       | -       | dynamic | dynamic | static
curl          | ?         | Http      | -       | -       | dynamic | dynamic | static

[libjpeg]: http://www.ijg.org/
