# A JUCE module for high level audio application development.

dRowAudio is a 3rd party JUCE module designed for rapid audio application development. It contains classes for audio processing and GUI elements.

Additionally, there are several wrappers around 3rd party libraries including:
* [cURL](https://github.com/bagder/curl)
* [FFTReal](http://ldesoras.free.fr/prod.html)
* [SoundTouch](http://www.surina.net/soundtouch/index.html)

dRowAudio is written following JUCE's [Coding Standard](http://www.juce.com/learn/coding-standards).

This library is hosted on [GitHub](https://github.com/m-rest/drowaudio).
Please find the online documentation [here](http://drowaudio.co.uk/docs/).

## Prerequisites

This documentation assumes that the reader has a working knowledge of [JUCE](https://github.com/julianstorer/JUCE).

## Integration

This fork of dRowAudio requires JUCE 7.0.3.


## Platforms

All platforms supported by JUCE are also supported by dRowAudio.

Currently, these platforms include:
* **Windows**: Applications and VST/VST3/RTAS/AAX/NPAPI/ActiveX plugins can be built 
using MS Visual Studio. The results are all fully compatible with Windows XP, Vista, 7, 8, 8.1 and 10.
* **Mac OS X**: Applications and VST/VST3/AudioUnit/RTAS/AAX/NPAPI plugins with Xcode.
* **GNU/Linux**: Applications and plugins can be built for any kernel 2.6 or later.
* **iOS**: Native iPhone and iPad apps.
* **Android**: Supported.

## License

Copyright (C) 2013 - Present by [David Rowland](mailto:dave@drowaudio.co.uk)

dRowAudio is provided under the terms of The MIT License (MIT):

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Some portions of the software including but not limited to [SoundTouch](http://www.surina.net/soundtouch/index.html) and [FFTReal](http://ldesoras.free.fr/prod.html) are included with in the repository but released under separate licences. Please see the individual source files for details.

## Other Contributors

* [Joël R. Langlois](https://github.com/jrlanglois)
* [Maximilian Rest](https://github.com/m-rest)
* [Roland Rabien](https://github.com/FigBug) 
* [Atilio Menéndez](https://github.com/atiliomf)

