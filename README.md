# nodenetcdf

[![Ubuntu Build](https://github.com/pheonixfirewingz/nodenetcdf/actions/workflows/nodejs.yml/badge.svg?event=push&os=ubuntu-latest)](https://github.com/pheonixfirewingz/nodenetcdf/actions/workflows/nodejs.yml)
[![Windows Build](https://github.com/pheonixfirewingz/nodenetcdf/actions/workflows/nodejs.yml/badge.svg?event=push&os=windows-latest)](https://github.com/pheonixfirewingz/nodenetcdf/actions/workflows/nodejs.yml)
[![macOS Build](https://github.com/pheonixfirewingz/nodenetcdf/actions/workflows/nodejs.yml/badge.svg?event=push&os=macos-latest)](https://github.com/pheonixfirewingz/nodenetcdf/actions/workflows/nodejs.yml)

A Node.js native addon for reading and writing NodeNetCDF files, providing a comprehensive interface to the NetCDF-4 C library.

## Overview

This package provides Node.js bindings to the NetCDF-4 library, allowing you to read and write NodeNetCDF files directly from JavaScript. NetCDF (Network Common Data Form) is a set of software libraries and machine-independent data formats that support the creation, access, and sharing of array-oriented scientific data.

### Features

- Read and write NodeNetCDF files
- Full support for groups, variables, dimensions, and attributes
- Support for all NodeNetCDF data types
- Chunk mode and compression options
- Fill values and endianness control
- Strided slice operations for efficient data access
- Cross-platform support (Windows, Linux, macOS)

## Requirements

- Node.js >= 22.0.0
- C++ compiler with C++20 support
- CMake (for building dependencies)

## Installation

```bash
npm install nodenetcdf
```

The installation process will:
1. Set up vcpkg package manager
2. Verify build dependencies
3. Build the native addon using node-gyp
4. Copy required shared libraries

### Platform-Specific Notes

**Windows:**
- Requires Visual Studio 2022 (v143 toolset)
- Build uses vcpkg to manage NetCDF and HDF5 dependencies

**Linux:**
- Requires gcc with C++20 support
- Dependencies are statically linked via vcpkg

**macOS:**
- Requires Xcode command line tools
- Minimum deployment target: macOS 10.15

## Usage

### Opening a NetCDF File

```javascript
const nodenetcdf = require('nodenetcdf');

// Open an existing file for reading
const file = new nodenetcdf.File('path/to/file.nc', 'r');

// Create a new file
const newFile = new nodenetcdf.File('path/to/new-file.nc', 'w', 'nodenetcdf');

// Available modes:
// 'r'  - read-only
// 'w'  - write (create new file)
// 'c'  - create (fail if exists)
// 'a'  - append (read/write existing file)

// Available formats:
// 'nodenetcdf' - NetCDF-4 format
// 'classic'  - NetCDF classic format
// '64bit'    - 64-bit offset format
```

### Working with Groups

```javascript
// Access the root group
const root = file.root;

// Get group properties
console.log(root.name);      // Group name
console.log(root.fullname);  // Full path
console.log(root.id);        // NetCDF ID

// Create a subgroup
const dataGroup = root.addSubgroup('data');

// Access subgroups
const subgroups = root.subgroups;
console.log(subgroups);  // Object with subgroup names as keys
```

### Working with Dimensions

```javascript
// Add dimensions
const timeDim = root.addDimension('time', 0);     // Unlimited dimension
const latDim = root.addDimension('lat', 180);     // Fixed dimension
const lonDim = root.addDimension('lon', 360);

// Access dimension properties
console.log(timeDim.name);    // 'time'
console.log(timeDim.length);  // Current length
console.log(timeDim.id);      // NetCDF ID

// Get all dimensions
const dimensions = root.dimensions;
const unlimitedDims = root.unlimited;  // Array of unlimited dimensions
```

### Working with Variables

```javascript
// Create a variable
const tempVar = root.addVariable('temperature', 'float', ['time', 'lat', 'lon']);

// Supported data types:
// 'byte', 'char', 'short', 'int', 'float', 'double',
// 'ubyte', 'ushort', 'uint', 'int64', 'uint64'

// Write data
tempVar.write([1.5, 2.3, 3.7, ...]);

// Write a slice
tempVar.writeSlice([0, 0, 0], [1, 180, 360], data);

// Write with stride
tempVar.writeStridedSlice([0, 0, 0], [1, 1, 1], [10, 180, 360], data);

// Read data
const data = tempVar.read();

// Read a slice
const slice = tempVar.readSlice([0, 0, 0], [1, 180, 360]);

// Read with stride
const stridedData = tempVar.readStridedSlice([0, 0, 0], [2, 2, 2], [10, 180, 360]);

// Variable properties
console.log(tempVar.name);        // Variable name
console.log(tempVar.type);        // Data type
console.log(tempVar.dimensions);  // Array of dimension names
console.log(tempVar.id);          // NetCDF ID
```

### Variable Settings

```javascript
// Set endianness
tempVar.endianness = 'little';  // or 'big', 'native'

// Set checksum mode
tempVar.checksummode = 'fletcher32';  // or 'none'

// Set chunking
tempVar.chunkmode = 'chunked';  // or 'contiguous'
tempVar.chunksizes = [1, 180, 360];

// Set fill mode and value
tempVar.fillmode = true;
tempVar.fillvalue = -999.9;

// Set compression
tempVar.compression_shuffle = true;
tempVar.compression_deflate = true;
tempVar.compression_level = 6;  // 0-9
```

### Working with Attributes

```javascript
// Add attributes to variables
tempVar.addAttribute('units', 'Kelvin');
tempVar.addAttribute('long_name', 'Air Temperature');
tempVar.addAttribute('valid_range', [200.0, 350.0]);

// Add global attributes (to groups)
root.addAttribute('title', 'Climate Data');
root.addAttribute('institution', 'Research Center');
root.addAttribute('created', new Date().toISOString());

// Read attributes
const attrs = tempVar.attributes;
console.log(attrs.units.value);  // 'Kelvin'

// Modify attribute value
attrs.units.value = 'Celsius';

// Delete an attribute
attrs.units.delete();
```

### Closing Files

```javascript
// Sync changes to disk
file.sync();

// Close the file
file.close();
```

## API Reference

### File

- `new File(path, mode, format)` - Open or create a NetCDF file
- `file.root` - Access the root group
- `file.close()` - Close the file
- `file.sync()` - Sync changes to disk

### Group

Properties:
- `group.id` - NetCDF group ID
- `group.name` - Group name
- `group.fullname` - Full path of the group
- `group.dimensions` - Object containing dimensions
- `group.unlimited` - Array of unlimited dimensions
- `group.variables` - Object containing variables
- `group.attributes` - Object containing attributes
- `group.subgroups` - Object containing subgroups

Methods:
- `group.addDimension(name, length)` - Add a dimension
- `group.addVariable(name, type, dimensions)` - Add a variable
- `group.addAttribute(name, value)` - Add an attribute
- `group.addSubgroup(name)` - Add a subgroup

### Dimension

Properties:
- `dimension.id` - NetCDF dimension ID
- `dimension.name` - Dimension name
- `dimension.length` - Current length of the dimension

### Variable

Properties:
- `variable.id` - NetCDF variable ID
- `variable.name` - Variable name
- `variable.type` - Data type
- `variable.dimensions` - Array of dimension names
- `variable.attributes` - Object containing attributes
- `variable.endianness` - Byte order ('little', 'big', 'native')
- `variable.checksummode` - Checksum mode ('none', 'fletcher32')
- `variable.chunkmode` - Chunking mode ('contiguous', 'chunked')
- `variable.chunksizes` - Array of chunk sizes
- `variable.fillmode` - Whether fill values are enabled
- `variable.fillvalue` - Fill value
- `variable.compression_shuffle` - Shuffle filter enabled
- `variable.compression_deflate` - Deflate compression enabled
- `variable.compression_level` - Compression level (0-9)

Methods:
- `variable.read()` - Read all data
- `variable.readSlice(start, count)` - Read a slice
- `variable.readStridedSlice(start, stride, count)` - Read with stride
- `variable.write(data)` - Write data
- `variable.writeSlice(start, count, data)` - Write a slice
- `variable.writeStridedSlice(start, stride, count, data)` - Write with stride
- `variable.addAttribute(name, value)` - Add an attribute

### Attribute

Properties:
- `attribute.name` - Attribute name
- `attribute.value` - Attribute value

Methods:
- `attribute.delete()` - Delete the attribute

## Development

### Running Tests

```bash
npm test
```

### Building from Source

```bash
npm install
```

The build process uses:
- `node-gyp` for compiling the native addon
- `vcpkg` for managing C++ dependencies (NetCDF, HDF5, zlib, curl)

## Original Project and License

This is a fork and modernization of the original [nodenetcdf](https://github.com/parro-it/nodenetcdf) project.

### Original Contributors

- Andrea Parodi <andrea@parro.it> (http://www.parro.it/)
- Sven Willner <sven.willner@gmail.com> (http://svenwillner.net)

### Current Maintainer

- Luke Shore <luke.a.shore@oliva.energy> (https://oliva.energy)

### License

ISC License

Copyright (c) 2015, Sven Willner <sven.willner@gmail.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

## Links

- [Original Project](https://github.com/parro-it/nodenetcdf)
- [NetCDF Documentation](https://www.unidata.ucar.edu/software/netcdf/docs/)
- [GitHub Repository](https://github.com/pheonixfirewingz/nodenetcdf)
