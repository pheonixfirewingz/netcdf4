# netcdf4-js

![Build status](https://github.com/parro-it/netcdf4/workflows/Node.js%20CI/badge.svg?branch=master)
[![NPM Version](https://img.shields.io/npm/v/netcdf4.svg)](https://npmjs.org/package/netcdf4)

NodeJS addon for reading and writing the files in the
[Network Common Data Form (NetCDF)](https://www.unidata.ucar.edu/software/netcdf/)
version <= 4,
built upon the C-library for netcdf.

## Installation

`netcdf4-js` requires `nodejs` >= 18.0.0

Install using `npm`:

```bash
npm install netcdf4
```

### Prerequisites

The package now automatically manages NetCDF dependencies using vcpkg. During installation:

1. vcpkg will be automatically downloaded and bootstrapped
2. NetCDF and its dependencies (HDF5, zlib, curl, etc.) will be installed via vcpkg
3. The native module will be compiled with these dependencies

**You need to have the following build tools installed:**

#### Windows
* Visual Studio 2019 or later (with C++ development tools)
* Or Visual Studio Build Tools 2019 or later
* Git (for vcpkg)

#### Linux
* GCC/G++ or Clang
* CMake >= 3.10
* Git (for vcpkg)
* Build essentials: `sudo apt-get install build-essential cmake git`

#### macOS
* Xcode Command Line Tools: `xcode-select --install`
* Git (for vcpkg)

### Manual Installation (Alternative)

If you prefer to manage NetCDF dependencies yourself:

1. Install NetCDF4 using your system package manager:
   * **Linux**: `sudo apt-get install libnetcdf-dev`
   * **macOS**: `brew install netcdf`
   * **Windows**: Download from [NetCDF website](https://www.unidata.ucar.edu/downloads/netcdf/index.jsp)

2. Skip the automated vcpkg setup by setting an environment variable:
   ```bash
   SKIP_VCPKG_SETUP=1 npm install netcdf4
   ```

## Usage

Open files with
```javascript
var netcdf4 = require("netcdf4");

var file = new netcdf4.File("test/testrh.nc", "r");
```
File modes are `"r"` for "reading", `"w"` for "writing", `"c"` for
"creation", and `"c!"` for "overwriting".

Then you can read variables using `read` or `readSlice`. The following example reads values at positions 5 to 15:
```javascript
console.log(file.root.variables['var1'].readSlice(5, 10));
```

### Classes

Properties marked *(r/w)* can be read and will be written to the file
when set.

#### File

Properties:
* `root` : Main `Group`-object in file

Methods:
* `close()` : Close file
* `sync()` : Sync (or "flush") file to disk

#### Group

Properties:
* `id` : ID used by C-library
* `name` : Name
* `fullname` : Full name (path in file)
* `variables` : Associative array of variables in group
* `dimensions` : Associative array of dimensions in group
* `unlimited` : Associative array of unlimited Dimensions in group
* `attribute` : Associative array of attributes of group
* `subgroups` : Associative array of subgroups of group

Methods:
* `addVariable(name, type, dimensions)` : Add a new variable in
  group. `type` is one of `"byte", "char", "short", "int", "ubyte", "ushort",
  "uint", "float", "double"`. `dimensions` is an array of ids of dimensions
  for the new variable. Returns new variable.
* `addDimension(name, length)` : Add new dimension of length `length`
  (can be `"unlimited"` for unlimited dimension). Returns new dimension.
* `addSubgroup(name)` : Add subgroup. Returns new subgroup.
* `addAttribute(name, type, value)` : Add and set new attribute. Returns new attribute.

### Dimension

Properties:
* `id` : ID used by C-library
* `name` : Name (r/w)
* `length` : Length or currently used length for unlimited dimensions

### Attribute

Properties:
* `id` : ID used by C-library
* `name` : Name (r/w)
* `value` : Value (r/w)

Methods:
* `delete()` : Delete attribute

### Variable

Properties:
* `id` : ID used by C-library
* `name` : Name (r/w)
* `type` : Type of variable
* `attributes` : Associative array of attributes
* `dimensions` : Array of dimensions used by variable
* `endianness` : Endianness: `"little"`, `"big"`, or `"native"` (r/w)
* `checksummode` : Checksum mode: `"none"`, or `"fletcher32"` (r/w)
* `chunkmode` : Chunk mode: `"contiguous"`, or `"chunked"` (r/w)
* `chunksizes` : Array of chunk sizes (one size per dimension) (r/w)
* `fillmode` : Boolean switch for fill mode (r/w)
* `fillvalue` : Fill value (r/w)
* `compressionshuffle` : Boolean switch for shuffle (r/w)
* `compressiondeflate` : Boolean switch for compression (r/w)
* `compressionlevel` : Compression level (1-9) (r/w)

Methods:
* `read(pos....)` : Reads and returns a single value at positions
  given as for `write`.
* `readSlice(pos, size....)` : Reads and returns an array of values (cf.
  ["Specify a Hyperslab"](https://www.unidata.ucar.edu/software/netcdf/docs/programming_notes.html#specify_hyperslab))
  at positions and sizes given for each dimension, `readSlice(pos1,
  size1, pos2, size2, ...)` e.g. `readSlice(2, 3, 4, 2)` gives an
  array of the values at position 2 for 3 steps along the first
  dimension and position 4 for 2 steps along the second one.
* `readStridedSlice(pos, size, stride....)` : Similar to `readSlice()`, but it
  adds a *stride* (interval between indices) parameter to each dimension. If stride is 4,
  the function will take 1 value, discard 3, take 1 again, etc.
  So for instance `readStridedSlice(2, 3, 2, 4, 2, 1)` gives an
  array of the values at position 2 for 3 steps with stride 2 (i.e.
  every other value) along the first dimension and position 4 for 2 steps
  with stride 1 (i.e. with no dropping) along the second dimension.
* `write(pos..., value)` : Write `value` at positions given,
  e.g. `write(2, 3, "a")` writes `"a"` at position 2 along the first
  dimension and position 3 along the second one.
* `writeSlice(pos, size..., valuearray)` : Write values in `valuearray`
  (must be a typed array) at positions and sizes given for each
  dimension, e.g. `writeSlice(2, 3, 4, 2, new
  Int32Array([0, 1, 2, 3, 4, 5]))` writes the array at position 2 for
  3 steps along the first dimension and position 4 for 2 step along
  the second one (cf.
  ["Specify a Hyperslab"](https://www.unidata.ucar.edu/software/netcdf/docs/programming_notes.html#specify_hyperslab)).
* `writeStridedSlice(pos, size, stride..., valuearray)` : Similar to
  `writeSlice()`, but it adds a *stride* parameter to each dimension.
  So for instance `writeStridedSlice(2, 3, 2, 4, 2, 1), new
  Int32Array([0, 1, 2, 3, 4, 5])` writes the array
  at position 2 for 3 steps with stride 2 (i.e.
  every other value) along the first dimension and position 4 for 2 steps
  with stride 1 (i.e. with no dropping) along the second dimension.
* `addAttribute(name, type, value)` : Adds and sets new attribute. Returns
  new attribute.
