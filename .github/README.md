<div align="center">

## Almond Controller

</div>

<div align="center">

    Project almond hardware esp8266 control code for hydroponics system

  [![Almond](../resources/images/readme.svg)](https://almond.com)

  #### Simple but complicated almond

</div>

### Description
This is a hydroponics farm control system to be used at home with limited space, in greenhouses and indoors as well.

### Getting started
1. Install relevant compilers for your OS:
    - Windows: Visual Studio 2017 Community Edition x64 (this version is mandatory!)
    - macOS: XCode
2. Install [`PlatformIO`](https://docs.platformio.org/en/latest/ide/vscode.html#ide-vscode) and set up on VSCode.
3. Clone this repository and the necessary submodules:

```
    git clone https://github.com/almond-hydroponics/almond-controller.git
    git submodule update --init https://github.com/adafruit/RTClib.git
```

### Development set up
1. Install [`PlatformIO`](https://docs.platformio.org/en/latest/ide/vscode.html#ide-vscode) and set up on VSCode.
2. To clone, run `https://github.com/almond-hydroponics/almond-controller.git`.
3. `cd` into the root of the **project directory**.
4. Upload the file from src to the esp32 board by using `platformio run`

### ESP 8266 Datasheet
[Datasheet Download](https://circuits4you.com/wp-content/uploads/2018/12/esp32-wroom-32_datasheet_en.pdf)

### ESP 8266 Pinout
<br />
<img width="1440" alt="almond-screenshot" src="../resources/images/ESP8266_12X.jpg">
<br />


### Testing

**Unit tests** - Run `platformio test -e nodemcuv2 -v` on the terminal while within the **project root directory**. Unit testing is achieved through the use of `platformio`.

### Support or Contribution

For any suggestions or contributions please do not hesitate to contact the owners of this repository.

Contributions to this project are welcomed by all, If you need to contribute, follow the steps below

- **Fork** the repository
- Follow [Installation and Setup](#installation) as explained earlier
- Create a branch off `develop` for the feature you wish to add
- Make necessary changes, commit and raise a pull request against develop, conventions can be found on the [wiki page](https://github.com/mashafrancis/almond-hw/wiki).
  **Note** when making contributions, please endeavour to follow the [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) C++ and Arduino style guide. Also make sure to follow the [Almond Guide](https://github.com/mashafrancis/almond-hw/wiki) for conventions.

### FAQ
See the almond [wiki](https://github.com/mashafrancis/almond-hw/wiki)
