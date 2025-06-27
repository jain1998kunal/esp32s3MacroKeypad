# ESP32S3 Macro Keypad

A customizable USB macro keypad built using the ESP32-S3, capable of executing user-defined key combinations. This project supports LittleFS-based configuration, display output, and HID functionality, making it ideal for productivity workflows or automation tasks.

---

## 🔧 Features

- USB HID support for keypress, modifier, and consumer control commands  
- Configurable button actions via a `commands.txt` file stored on LittleFS  
- OLED or SPI display support for showing images (BMP) or text per page  
- Works out of the box with the **WeAct ESP32-S3 N4R2** board  
- LittleFS-based configuration for both macros and display content  
- Future support planned for RGB LED effects and encoder-based scrolling  

---

## ▶️ Getting Started

### ✅ Running the Project

1. Open the project using **PlatformIO** (VS Code recommended).  
2. Build and flash the firmware to your ESP32-S3 board.  
3. Connect the ESP32-S3 to your PC using the USB **device port**.  
4. Press any button on the keypad — it will execute the configured command on your PC.

---

## 📝 Configuring Commands

Commands are stored in `commands.txt` located in LittleFS. Each line maps to a button on the keypad, and supports a series of commands in the format:

<command> <value>,<command> <value>,...,<command> <value>


### Command Types

| Command | Description                      |
|---------|----------------------------------|
| 1       | Standard alphanumeric keypress   |
| 2       | Modifier keys (Ctrl, Shift, etc) |
| 3       | Consumer control keys (Media)    |
| 4       | Release single standard key      |
| 5       | Release all standard keys        |
| 6       | Release consumer control key     |

- Refer to the macros in `usb.h`, `hid.h`, and `USBHIDConsumerControl.h` for valid key values.
- Make sure to end the command line with a `Release` or `ReleaseAll` to avoid sticky keys.

---

## 🖼️ Display Information

- **Text Display**: Place files like `page1.txt`, `page2.txt`, etc., in LittleFS. These files will be shown as text on the display.
- **Image Display**: Use monochrome `.bmp` images named `page1.bmp`, `page2.bmp`. The firmware will automatically detect and display the image.

---

## 📌 Tested Hardware

- ESP32-S3 board: **WeAct ESP32-S3 N4R2**

---

## ✅ TODO

- [ ] Add scroll functionality using magnetic encoder  
- [x] Add LittleFS support  
- [x] Support for BMP and text display  
- [x] Command storage via file system  
- [ ] Define and implement onboard RGB LED functionality  

---

## 🚀 Future Improvements

- UI improvements for the display  
- Companion PC app for configuring macros and display content  

---

## 🤝 Contributing

If you'd like to collaborate or suggest improvements, feel free to reach out to jain1998kunal@gmail.com.
