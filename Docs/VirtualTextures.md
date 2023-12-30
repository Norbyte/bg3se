# Notes on Virtual Texture Usage

### Building Virtual Textures

To build a tile set, lslib needs to know what textures to include in what layers. The tile set configuration XML is a descriptor that contains all information required to build and merge tile sets. (See below for a complete example)

The tile set needs a name; this can be any user-defined name. The generated GTS and GTP file names will take the form `YOUR_TILESET_FILE_NAME.gts` and `YOUR_TILESET_FILE_NAME_xxxx.gtp`.
```xml
<TileSet Version="2" Name="YOUR_TILESET_FILE_NAME">
```

The paths for where to read source textures from and where to put the generated virtual textures to needs to be defined in the tile set XML. These paths are relative to the root of your mod directory:
```xml
<Paths>
	<SourceTextures>Public\YOUR_MOD_NAME\Assets\Textures</SourceTextures>
	<VirtualTextures>Public\YOUR_MOD_NAME\Assets\VirtualTextures</VirtualTextures>
</Paths>
```

The tile set can contain an arbitrary number of textures. During tile set generation, the virtual texture is resized automatically to fit all source textures.
Each virtual texture has multiple source textures (layers), one source DDS file is required for each layer defined in the tile set. 
For BG3, the number of layers is hardcoded to 3, called `Albedo`, `Normal` and `Physical`. Note that character body textures still use the same layer names, despite not containing `Albedo` and `Physical` maps but rather `HMVY` and `CLEA` maps.

Source DDS files must satisfy following requirements:
 - Mipmaps must be included in the .DDS file (make sure to check "Generate mipmaps" or the equivalent in your exporter)
 - Texture must be at least 128x128
 - Texture width and height must be a multiple of two (Note: width and height can differ, eg. a 1024x512 texture is allowed)
 - Must use DXT5/BC3 format

DDS textures within the same virtual texture must have the same size ratio; eg. an `Albedo` of 1024x1024 and a `Normal` of 1024x512 is not allowed. However, their size can be a multiple of each other; eg. an `Albedo` of 1024x1024 and a `Normal` of 512x512 is allowed.

The texture name (`Name="e05176c4ac30d5228d0c0452d938f699"`) is the `GTex` name used in asset LSF files.
Layer name must be one of the 3 predefined names (`Albedo`, `Normal` or `Physical`).
The Source path (`Source="e05176c4ac30d5228d0c0452d938f699_0.dds"`) is relative to the `SourceTextures` path defined above.

```xml
<Textures>
	<Texture Name="e05176c4ac30d5228d0c0452d938f699"> <!-- GTex name -->
		<Layer Name="Albedo" Source="e05176c4ac30d5228d0c0452d938f699_0.dds" />
		<Layer Name="Normal" Source="e05176c4ac30d5228d0c0452d938f699_1.dds" />
		<Layer Name="Physical" Source="e05176c4ac30d5228d0c0452d938f699_2.dds" />
	</Texture>
```

In case a virtual texture only has two layers (eg. it has no `Physical` map), the missing layer can be omitted and it will not be included in the generated tile set:
```xml
<Textures>
	<Texture Name="e05176c4ac30d5228d0c0452d938f699">
		<Layer Name="Albedo" Source="e05176c4ac30d5228d0c0452d938f699_0.dds" />
		<Layer Name="Normal" Source="e05176c4ac30d5228d0c0452d938f699_1.dds" />
	</Texture>
```

A complete sample XML:
```xml
<?xml version="1.0" encoding="utf-8"?>
<TileSet Version="2" Name="YOUR_TILESET_FILE_NAME">
	<Paths>
		<SourceTextures>Public\YOUR_MOD_NAME\Assets\Textures</SourceTextures>
		<VirtualTextures>Public\YOUR_MOD_NAME\Assets\VirtualTextures</VirtualTextures>
	</Paths>
	
	<Textures>
		<Texture Name="e05176c4ac30d5228d0c0452d938f699">
			<Layer Name="Albedo" Source="e05176c4ac30d5228d0c0452d938f699_0.dds" />
			<Layer Name="Normal" Source="e05176c4ac30d5228d0c0452d938f699_1.dds" />
			<Layer Name="Physical" Source="e05176c4ac30d5228d0c0452d938f699_2.dds" />
		</Texture>
		<Texture Name="00fb1fe11e94bc9fad61170f5551017f">
			<Layer Name="Albedo" Source="00fb1fe11e94bc9fad61170f5551017f_0.dds" />
			<Layer Name="Normal" Source="00fb1fe11e94bc9fad61170f5551017f_1.dds" />
		</Texture>
	</Textures>
	
	<Layers>
		<Layer Name="Albedo" Type="R8G8B8A8_SRGB" />
		<Layer Name="Normal" Type="X8Y8Z8W8" />
		<Layer Name="Physical" Type="X8Y8Z8W8" />
	</Layers>
	
	<TileSetConfig>
		<Compression>Best</Compression>
		<OneFilePerGTex>false</OneFilePerGTex>
		<BackfillPages>true</BackfillPages>
		<DeduplicateTiles>true</DeduplicateTiles>
	</TileSetConfig>
</TileSet>
```

After making the config XML, the tile set can be built either using the LSLib UI on the Virtual Textures tab, or by using the `VTexTool.exe` cli using the parameters:
`VTexTool.exe \<XML path\> \<Mod root path\>`
(eg. `VTexTool.exe "C:\Dev\MyMod\Mods\YOUR_MOD_NAME\VTexConfig.xml" "C:\Dev\MyMod"`)

### Loading Virtual Textures

To load custom .GTS files, Script Extender v12 or later is required (currently `Devel` branch only).
The mod must include a configuration file at `Mods\YOUR_MOD_NAME\ScriptExtender\VirtualTextures.json` that describes which GTex resource can be found in which .GTP file.

If you specify a GTexName that is already defined by the base game, the original GTex will be overridden by the new one.
If you specify a GTexName that is defined by another mod, the mod that loads later will override the GTex of the one that loaded earlier.

```json
{
    "Mappings": [
        {
            "GTexName": "e05176c4ac30d5228d0c0452d938f699",
            "GTS": "Public/YOUR_MOD_NAME/Assets/VirtualTextures/YOUR_TILESET_FILE_NAME.gts"
        },
        {
            "GTexName": "00fb1fe11e94bc9fad61170f5551017f",
            "GTS": "Public/YOUR_MOD_NAME/Assets/VirtualTextures/YOUR_TILESET_FILE_NAME.gts"
        }
    ]
}
```

### Glossary

 - Tile Set - A huge texture containing multiple sub-textures merged into one
 - .GTS file - Graphine Tile Set file, containing a description of the tile set and the layout/position of all tiles and page files
 - .GTP file - Graphine Tile Page file, containing the textures subdivided into equally sized chunks (tiles)
 - GTex - A slice of the tile set containing only a specific source texture
