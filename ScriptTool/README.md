# Script Tool

Tool working directory must be `Samogonki/MechoSoma`.

## Language file generation

Command `localization` generates file with localization data for all supported languages in UTF-8 for easy editing.

Usage:

```bash
script-tool localization <game data directory> <output file>
```

Game data directory must contain original SCB files from CD for each supported language.

## SCB files generation

Command `scripts` generates localized SCB files from SCR files and localization file.

Usage:

```bash
script-tool scripts <localization file> <game data directory>
```
