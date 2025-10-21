# TypeScript Support

This package includes TypeScript type definitions for full IntelliSense and type safety when using the NetCDF4 library in TypeScript projects.

## Installation

The type definitions are automatically included when you install the package:

```bash
npm install nodenetcdf
```

## Usage

### Importing

```typescript
import { File, Group, Variable, Dimension, Attribute } from 'nodenetcdf';
```

### Type Definitions

The package exports the following types:

- **Classes**: `File`, `Group`, `Variable`, `Dimension`, `Attribute`
- **Type Aliases**: `NetCDFDataType`, `FileMode`, `FileFormat`, `Endianness`, `ChecksumMode`, `ChunkMode`, `FillMode`

### Example Usage

```typescript
import { File, NetCDFDataType } from 'nodenetcdf';

// Create a new NetCDF file
const file = new File('output.nc', 'c!', 'nodenetcdf');

// Access the root group (fully typed)
const root = file.root;

// Add dimensions with type safety
const timeDim = root.addDimension('time', 0);  // unlimited dimension
const latDim = root.addDimension('lat', 180);
const lonDim = root.addDimension('lon', 360);

// Add variables with proper typing
const tempVar = root.addVariable('temperature', 'float', [timeDim, latDim, lonDim]);

// Set variable properties with autocomplete support
tempVar.endianness = 'little';
tempVar.compressionLevel = 6;
tempVar.fillValue = -999.0;

// Add attributes (type-checked)
tempVar.addAttribute('units', 'string', 'degrees_C');
tempVar.addAttribute('long_name', 'string', 'Air Temperature');

// Write and read data with type safety
tempVar.write(data);
const retrievedData = tempVar.read();

// Sync and close
file.sync();
file.close();
```

## Available Types

### NetCDFDataType

```typescript
type NetCDFDataType = 
  | 'byte'
  | 'char'
  | 'short'
  | 'int'
  | 'float'
  | 'double'
  | 'ubyte'
  | 'ushort'
  | 'uint'
  | 'int64'
  | 'string';
```

### FileMode

```typescript
type FileMode = 
  | 'r'     // Read-only
  | 'w'     // Read-write
  | 'c'     // Create (no clobber)
  | 'c!';   // Create (clobber/overwrite)
```

### FileFormat

```typescript
type FileFormat = 
  | 'classic'           // Classic NetCDF format
  | 'classic64'         // 64-bit offset format
  | 'nodenetcdf'        // NetCDF-4 format
  | 'nodenetcdfclassic'; // NetCDF-4 classic model
```

## API Documentation

### File Class

```typescript
class File {
  constructor(filename: string, mode: FileMode, format?: FileFormat);
  readonly root: Group;
  sync(): void;
  close(): void;
  inspect(): string;
}
```

### Group Class

```typescript
class Group {
  readonly id: number;
  readonly variables: { [name: string]: Variable };
  readonly dimensions: { [name: string]: Dimension };
  readonly unlimited: Dimension | null;
  readonly attributes: { [name: string]: Attribute };
  readonly subgroups: { [name: string]: Group };
  readonly name: string;
  readonly fullname: string;
  
  addAttribute(name: string, type: NetCDFDataType, value: any): Attribute;
  addDimension(name: string, length: number): Dimension;
  addSubgroup(name: string): Group;
  addVariable(name: string, type: NetCDFDataType, dimensions: (string | Dimension)[]): Variable;
  inspect(): string;
}
```

### Variable Class

```typescript
class Variable {
  readonly id: number;
  readonly type: NetCDFDataType;
  readonly dimensions: Dimension[];
  readonly attributes: { [name: string]: Attribute };
  name: string;
  endianness: Endianness;
  checksumMode: ChecksumMode;
  chunkMode: ChunkMode;
  chunkSizes: number[];
  fillMode: FillMode;
  fillValue: any;
  compressionShuffle: boolean;
  compressionDeflate: boolean;
  compressionLevel: number;
  
  read(): any;
  readSlice(start: number[], count: number[]): any;
  readStridedSlice(start: number[], count: number[], stride: number[]): any;
  write(data: any): void;
  writeSlice(start: number[], count: number[], data: any): void;
  writeStridedSlice(start: number[], count: number[], stride: number[], data: any): void;
  addAttribute(name: string, type: NetCDFDataType, value: any): Attribute;
  inspect(): string;
}
```

### Dimension Class

```typescript
class Dimension {
  readonly id: number;
  readonly length: number;
  name: string;
  inspect(): string;
}
```

### Attribute Class

```typescript
class Attribute {
  name: string;
  value: any;
  delete(): void;
  inspect(): string;
}
```

## Benefits of TypeScript Support

1. **IntelliSense**: Get autocomplete suggestions for all methods and properties
2. **Type Safety**: Catch type errors at compile time instead of runtime
3. **Documentation**: Inline documentation available in your IDE
4. **Refactoring**: Safely rename and refactor code with confidence
5. **Error Prevention**: Prevent common mistakes like invalid file modes or data types

## Testing

The package includes a TypeScript test file to verify the type definitions:

```bash
npx tsc test/typescript-test.ts --noEmit --skipLibCheck
```

This ensures all type definitions are correct and complete.

## Contributing

If you find any issues with the type definitions or have suggestions for improvements, please open an issue on the GitHub repository.
