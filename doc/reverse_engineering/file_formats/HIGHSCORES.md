# Highscores

A [010 Editor](https://www.sweetscape.com/010editor/) Binary Template can be found [here](https://github.com/LockBlock-dev/ChickenInvaders-re/blob/8d63a57e4f12e67cc7080b0b00679454d8f84cc2/templates/ChickenInvadersHighscores.bt).

This file format is used in the `ChickenInvaders.hst` file.

---

> **Note**: All structure and field names used below are inferred based on behavior and naming conventions. They may not reflect the original or official terminology used in the source format.

### Highscore entry

Each highscore entry is a fixed-size structure consisting of a player name and a score. The file contains exactly 5 such entries, stored consecutively.

|  Name  | Type | Size (bytes) |           Description            |
| :----: | :--: | :----------: | :------------------------------: |
| pseudo | char |      10      | Player name (no null terminator) |
| score  | int  |      4       |         Highscore value          |
