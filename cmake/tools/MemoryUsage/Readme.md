# Memory Usage Script

## Description

This script uses the mapfile and the folder structure of the build directory to determine the memory usage of all tasks, their Modules and even their functions and compiles a presentable HTML file.
This HTML file presents multiple sortable tables for each Task Region and its corresponding modules.
If you click on a module row, the function names and their memory usage should appear below as additional childrows.
The function names will automatically be demangled if you have `arm-none-eabi-c++filt` installed and in your path. This can be done by activating your `conan` environment for the **Application** project. If you do not have a demangler in your path, it will simply print out the mangled names.

## Usage

`[python] memory_usage.py [-h] [--directory [DIRECTORY [DIRECTORY ...]]] [--exclude-folders [EXCLUDE_FOLDERS [EXCLUDE_FOLDERS ...]]] [--exclude-files [EXCLUDE_FILES [EXCLUDE_FILES ...]]] [--modules [MODULES [MODULES ...]]] --map-file MAP_FILE`

### Examples

Use the given mapfile and the check for the modules in the current working directory:  
`> python memory_usage.py -m /path/to/mapfile.map`  
Generates a memory_usage.html file in the current directory.  

Use the given mapfile, use the current working directory but exclude the BLE folder and every file that corresponds with "reliance" and "usb":  
`> python memory_usage.py -m /path/to/mapfile.map -e BLE --exclude-files *reliance* *usb*`  
Generates a memory_usage.html file in the current directory, where the BLE Module and every Module, that only had libraries with reliance or usb in their names, are missing.

Use the given mapfile, use the given directory and only look after the BLE Module:  
`> python memory_usage.py -m /path/to/mapfile.map -d /path/to/root -mod BLE`  
Generates a memory_usage.html file in the current directory, where ONLY the BLE modules is listed.


### Required Arguments

Command | Type | Help | Default
:-------|:----:|:-----|:-------
-m, --map, --map-file| String | The path to the mapfile of the build. | None

### Optional Arguments

Command | Type | Help | Default
:-------|:----:|:-----|:-------
-h, --help | Flag | show this help message and exit | False
-d, --dir,<br>--directory | Path List | The directories this program looks into. | Current Directory
-e, --exclude-folders | String List | Exclude these folders from the search. | Empty List
--exclude-files | Regex List | Ignores files that match the given Regular Expression. For more Infos check: https://docs.python.org/3/library/re.html#regular-expression-syntax | Empty List
-mod,<br>--modules | String List | Specify the module names you are interested in. | Everything
