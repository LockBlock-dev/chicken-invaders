# Assets

A [010 Editor](https://www.sweetscape.com/010editor/) Binary Template can be found [here](https://github.com/LockBlock-dev/ChickenInvaders-re/blob/8d63a57e4f12e67cc7080b0b00679454d8f84cc2/templates/ChickenInvadersData.bt).

This file format is used in the `ChickenInvaders.dat` file. It contains all the [game assets](../ASSETS.md).

---

> **Note**: All structure and field names used below are inferred based on behavior and naming conventions. They may not reflect the original or official terminology used in the source format.

### File header

The file begins with a 4-byte unsigned integer that specifies the number of assets contained in the file.

| Name  |     Type     | Size (bytes) |       Description       |
| :---: | :----------: | :----------: | :---------------------: |
| count | unsigned int |      4       | Number of asset entries |

### Asset header

Immediately following the file header are `count` asset headers. Each asset header is 88 bytes in length and contains metadata about a single asset.

|  Name  |     Type     | Size (bytes) |             Description              |
| :----: | :----------: | :----------: | :----------------------------------: |
|  name  |     char     |      80      |      Null-terminated asset name      |
| offset | unsigned int |      4       | Byte offset to the asset in the file |
|  size  | unsigned int |      4       |    Size of the the asset in bytes    |

### Asset data

Each asset’s data is stored at the specified `offset` and is exactly `size` bytes long.
