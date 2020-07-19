# anne-color
Utility to control Anne Pro 2 keyboard RGB lighting. It supports C15 version and does not save settings upon restart.

## Usage
To set a solid color provide the RGB values:
```
Usage: annecolor static 255 255 0
```
To set an effect provide 12 hex color code values:
```
Usage: annecolor breathing ff002f ff2600 ff7b00 ffd000 d9ff00 84ff00 2fff00 00ff26 00ff7b 00ffd0 00d9ff 0084ff

Available effects:
  breathing
  breathing-key
  breathing-trigger
  gradual-breathing
  poptang
  rainbow
  rainbow-trigger
```

## Requirements
- [`hidapi`](https://github.com/libusb/hidapi#about)
- [`_annecolor`](_annecolor) for Zsh completions
- In order to make the keyboard accesible without root privileges you need to create a new [udev rule](https://aur.archlinux.org/cgit/aur.git/tree/obinskit.install?h=obinskit).
```
SUBSYSTEM=="usb", ATTRS{idVendor}=="04d9", ATTRS{idProduct}=="a292",MODE="0666", GROUP="plugdev"
KERNEL=="hidraw*", ATTRS{idVendor}=="04d9", ATTRS{idProduct}=="a292",MODE="0666", GROUP="plugdev"
```

## License
[MIT license](LICENSE).
