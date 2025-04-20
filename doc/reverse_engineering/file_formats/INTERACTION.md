# Interaction

A [010 Editor](https://www.sweetscape.com/010editor/) Binary Template can be found [here](https://github.com/LockBlock-dev/ChickenInvaders-re/blob/8d63a57e4f12e67cc7080b0b00679454d8f84cc2/templates/ChickenInvadersInteraction.bt).

This file format is used in the `interaction.uan` asset. It contains the necessary information to render the InterActions studios animated logo when you start the game.

---

> **Note**: All structure and field names used below are inferred based on behavior and naming conventions. They may not reflect the original or official terminology used in the source format.

### File header

The file starts with a fixed-size header defining the animation display parameters.

|   Name    |     Type     | Size (bytes) |          Description           |
| :-------: | :----------: | :----------: | :----------------------------: |
|   width   | unsigned int |      4       | Width of the animation frames  |
|  height   | unsigned int |      4       | Height of the animation frames |
| framerate | unsigned int |      4       |    Frame rate for rendering    |

### Interaction entries

Following the file header, the file contains a sequence of interaction entries. Each entry begins with a 4-byte `interactionType`, followed by a type-specific structure.

|      Name       |     Type     | Size (bytes) |              Description               |
| :-------------: | :----------: | :----------: | :------------------------------------: |
| interactionType | unsigned int |      4       | Identifier for the type of interaction |

### Interaction types

The file defines several interaction types used to control animation and audio behavior:

#### Type 0: End

No additional data. This is a marker indicating that the animation has finished.

#### Type 1: Frame

|   Name    |     Type     |         Size (bytes)          |            Description            |
| :-------: | :----------: | :---------------------------: | :-------------------------------: |
| frameSize | unsigned int |               4               | Total size of the frame structure |
| frameType | unsigned int |               4               |       Frame type identifier       |
|   data    |    char[]    | frameSize - sizeof(frameType) |        Frame-specific data        |

#### Type 2: Sleep

|   Name    |     Type     | Size (bytes) |            Description            |
| :-------: | :----------: | :----------: | :-------------------------------: |
| sleepTime | unsigned int |      4       | Time in milliseconds to sleep for |

#### Type 3: Load SFX

| Name |  Type  |      Size (bytes)      |            Description            |
| :--: | :----: | :--------------------: | :-------------------------------: |
| name | char[] |        Up to 20        | Null-terminated sound effect name |
| data | char[] | 256 - strlen(name) - 1 |      Filler or reserved data      |

#### Type 4: Play SFX

| Name  |     Type     | Size (bytes) |        Description         |
| :---: | :----------: | :----------: | :------------------------: |
| index | unsigned int |      4       | Index of the sound to play |
| data  | unsigned int |      4       |          Padding           |

#### Type 5: Stop SFX

| Name  |     Type     | Size (bytes) |        Description         |
| :---: | :----------: | :----------: | :------------------------: |
| index | unsigned int |      4       | Index of the sound to stop |
