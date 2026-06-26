# Mi Smart Projector 2 Pro — IR Sleep / Wake

> Put a Xiaomi Mi Smart Projector 2 Pro to sleep and wake it back up with **any infrared remote**, using a tiny Arduino-based IR-to-USB bridge.

<p align="center">
  <img src="/stl/ir-remote-box.jpg" width="600" alt="3D printed IR receiver box"/>
</p>

---

🇬🇧 **English** · [🇭🇺 Magyar](#magyar)

## The problem

The Xiaomi Mi Smart Projector 2 Pro has no convenient "remote power on". This project adds one. A small box plugs into the projector's USB port and pretends to be a **USB keyboard**. It listens for two buttons on an infrared remote and translates them into USB key events the projector understands:

| Remote button | USB event sent      | Result on projector |
|---------------|---------------------|---------------------|
| **Sleep**     | `System Sleep`      | Goes to standby     |
| **Wake**      | `Enter` key press   | Wakes from standby  |

Because it decodes the **NEC** protocol, it works with a Broadlink RM, a spare LG TV remote, or any cheap NEC remote — you just teach it the two button codes.

## How it works

```
 IR remote  ──(infrared)──▶  IR receiver ──▶  Arduino Pro Micro ──(USB HID)──▶  Projector
```

1. The IR receiver picks up the remote's NEC signal.
2. The Arduino Pro Micro (ATmega32U4, native USB) decodes the command code.
3. Matching a known code, it emulates a USB keyboard and sends either a **System Sleep** power event or an **Enter** key press.

## Bill of materials

- **Arduino Pro Micro** (ATmega32U4) — native USB is required for HID emulation
- **IR receiver module** (e.g. TSOP38238 or VS1838B)
- A few jumper wires / a bit of soldering
- A **3D printer** for the enclosure (STL files included in [`/stl`](/stl))

## Wiring

| IR receiver pin | Arduino Pro Micro |
|-----------------|-------------------|
| VCC             | VCC (5V)          |
| GND             | GND               |
| OUT / DATA      | `IR_RECEIVE_PIN`  |

The default `IR_RECEIVE_PIN` for the Pro Micro is defined in `PinDefinitionsAndMore.h` (pin `2`).

## Build & flash

1. Install the **Arduino IDE** and add Pro Micro / SparkFun AVR board support.
2. Install these libraries from the Library Manager:
   - **IRremote** by Armin Joachimsmeyer (v4.x or newer)
   - **HID-Project** by NicoHood
3. Open [`arduino/mi-smart-2-pro-ir/mi-smart-2-pro-ir.ino`](arduino/mi-smart-2-pro-ir/mi-smart-2-pro-ir.ino).
4. Select **SparkFun Pro Micro / ATmega32U4 (5V, 16 MHz)** as the board and upload.

## Teaching it your remote

The two button codes are configurable at the top of the sketch:

```cpp
constexpr uint8_t CMD_SLEEP = 177;  // button that puts the projector to sleep
constexpr uint8_t CMD_WAKE  = 176;  // button that wakes the projector up
```

To find the codes for *your* remote:

1. Open the **Serial Monitor** at `115200` baud.
2. Press a button — the decoded `command` value is printed.
3. Copy those values into `CMD_SLEEP` and `CMD_WAKE`, then re-upload.

## Enclosure

Print and assemble the box from the STL files in [`/stl`](/stl):

- [`box.stl`](stl/box.stl) — main body
- [`box-lid.stl`](stl/box-lid.stl) — lid

## License

Released under the [MIT License](LICENSE).

---

<a name="magyar"></a>
## Magyar

[🇬🇧 English](#mi-smart-projector-2-pro--ir-sleep--wake) · 🇭🇺 **Magyar**

> Altasd el és ébreszd fel a Xiaomi Mi Smart Projector 2 Pro projektort **bármilyen infravörös távirányítóval**, egy apró Arduino-alapú IR→USB híd segítségével.

### A probléma

A Xiaomi Mi Smart Projector 2 Pro nem rendelkezik kényelmes „távoli bekapcsolás” funkcióval. Ez a projekt pótolja ezt. Egy kis doboz csatlakozik a projektor USB portjára, és **USB billentyűzetnek** adja ki magát. Egy infravörös távirányító két gombját figyeli, és olyan USB eseményekké fordítja, amelyeket a projektor megért:

| Távirányító gomb | Küldött USB esemény | Hatás a projektoron |
|------------------|---------------------|---------------------|
| **Altatás**      | `System Sleep`      | Készenlétbe megy    |
| **Ébresztés**    | `Enter` lenyomás    | Felébred készenlétből |

Mivel a **NEC** protokollt dekódolja, működik Broadlink RM-mel, egy felesleges LG TV távirányítóval, vagy bármilyen olcsó NEC távirányítóval — csak meg kell tanítani neki a két gombkódot.

### Hogyan működik

```
 IR távirányító ──(infra)──▶ IR vevő ──▶ Arduino Pro Micro ──(USB HID)──▶ Projektor
```

1. Az IR vevő veszi a távirányító NEC jelét.
2. Az Arduino Pro Micro (ATmega32U4, natív USB) dekódolja a parancskódot.
3. Ismert kód esetén USB billentyűzetet emulál, és vagy egy **System Sleep** energiagazdálkodási eseményt, vagy egy **Enter** billentyűt küld.

### Alkatrészek

- **Arduino Pro Micro** (ATmega32U4) — a HID emulációhoz natív USB szükséges
- **IR vevő modul** (pl. TSOP38238 vagy VS1838B)
- Néhány jumper vezeték / kevés forrasztás
- **3D nyomtató** a dobozhoz (az STL fájlok a [`/stl`](/stl) mappában)

### Bekötés

| IR vevő láb | Arduino Pro Micro |
|-------------|-------------------|
| VCC         | VCC (5V)          |
| GND         | GND               |
| OUT / DATA  | `IR_RECEIVE_PIN`  |

A Pro Micro alapértelmezett `IR_RECEIVE_PIN` lába a `PinDefinitionsAndMore.h` fájlban van megadva (`2`-es láb).

### Fordítás és feltöltés

1. Telepítsd az **Arduino IDE**-t, és add hozzá a Pro Micro / SparkFun AVR board támogatást.
2. Telepítsd ezeket a könyvtárakat a Library Managerből:
   - **IRremote** — Armin Joachimsmeyer (v4.x vagy újabb)
   - **HID-Project** — NicoHood
3. Nyisd meg a [`arduino/mi-smart-2-pro-ir/mi-smart-2-pro-ir.ino`](arduino/mi-smart-2-pro-ir/mi-smart-2-pro-ir.ino) fájlt.
4. Válaszd ki a **SparkFun Pro Micro / ATmega32U4 (5V, 16 MHz)** boardot, és töltsd fel.

### A saját távirányítód betanítása

A két gombkód a forráskód tetején állítható:

```cpp
constexpr uint8_t CMD_SLEEP = 177;  // a projektort altató gomb
constexpr uint8_t CMD_WAKE  = 176;  // a projektort ébresztő gomb
```

A *saját* távirányítód kódjainak megtalálása:

1. Nyisd meg a **Serial Monitort** `115200` baud sebességen.
2. Nyomj meg egy gombot — a dekódolt `command` érték kiíródik.
3. Másold be ezeket az értékeket a `CMD_SLEEP` és `CMD_WAKE` konstansokba, majd töltsd fel újra.

### A doboz

Nyomtasd ki és szereld össze a dobozt a [`/stl`](/stl) mappa STL fájljaiból:

- [`box.stl`](stl/box.stl) — fő test
- [`box-lid.stl`](stl/box-lid.stl) — fedél

### Licenc

[MIT licenc](LICENSE) alatt kiadva.
