# Pico DM QD3503728 HAGL GFX

HAGL speed tests for Pico_DM_QD3503728 Display ext. board

![img1](./assets/IMG_0607_compressed.jpg)

![img2](./assets/IMG_0610_compressed.jpg)

## Requirements

- pico-sdk >= 2.1.1
- picotool >= 2.1.1

## How to build

### 1. Clone and initialize the repository
```bash
git clone https://github.com/embeddedboys/pico_dm_qd3503728_hagl_gfx.git
cd pico_dm_qd3503728_hagl_gfx

git submodule update --init
```

### 2. Build for Raspberry Pi Pico (RP2040)
```bash
mkdir build && cd build
cmake .. -G Ninja
ninja
```

### 3. Build for Raspberry Pi Pico 2 (RP2350 ARM core)
```bash
mkdir build-pico2 && cd build-pico2
cmake -DPICO_BOARD=pico2 .. -G Ninja
ninja
```

## Flash the target

Flash the target using picotool:
```bash
sudo picotool -fuvx pico_dm_qd3503728_hagl_gfx.uf2
```

Or you can using openocd by running under the build directory:
```bash
ninja flash
```

## Performance

RP2350 ARM core with double buffering:

```bash
[main] 1461014 PIXELS per second, 99 fps, 209880 free heap
[main] 18937 LINES per second, 99 fps, 209880 free heap
[main] 147969 VERTICAL LINES per second, 99 fps, 209880 free heap
[main] 418701 HORIZONTAL LINES per second, 99 fps, 209880 free heap
[main] 39009 CIRCLES per second, 99 fps, 209880 free heap
[main] 27269 FILLED CIRCLES per second, 99 fps, 209880 free heap
[main] 21150 ELLIPSES per second, 99 fps, 209880 free heap
[main] 11149 FILLED ELLIPSES per second, 99 fps, 209880 free heap
[main] 6364 TRIANGLES per second, 99 fps, 209880 free heap
[main] 3681 FILLED TRIANGLES per second, 99 fps, 209880 free heap
[main] 61308 RECTANGLES per second, 99 fps, 209880 free heap
[main] 5007 FILLED RECTANGLES per second, 99 fps, 209880 free heap
[main] 46758 ROUND RECTANGLES per second, 99 fps, 209880 free heap
[main] 4774 FILLED ROUND RECTANGLES per second, 99 fps, 209880 free heap
[main] 3837 POLYGONS per second, 99 fps, 209880 free heap
[main] 2145 FILLED POLYGONS per second, 99 fps, 209880 free heap
[main] 86666 CHARACTERS per second, 99 fps, 209880 free heap
[main] 5960 STRINGS per second, 99 fps, 209880 free heap
[main] 35554 SCALED CHARACTERS per second, 99 fps, 209880 free heap
[main] 699 RGB BARS per second, 99 fps, 209880 free heap
```