// Type definitions for nodenetcdf
// Project: https://github.com/pheonixfirewingz/netcdf4
// Definitions by: Luke Shore <luke.a.shore@oliva.energy>

/**
 * NetCDF data types
 */
export type NetCDFDataType = 
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

/**
 * File open/create modes
 */
export type FileMode = 
  | 'r'     // Read-only
  | 'w'     // Read-write
  | 'c'     // Create (no clobber)
  | 'c!';   // Create (clobber/overwrite)

/**
 * NetCDF file formats
 */
export type FileFormat = 
  | 'classic'           // Classic NetCDF format
  | 'classic64'         // 64-bit offset format
  | 'nodenetcdf'        // NetCDF-4 format
  | 'nodenetcdfclassic'; // NetCDF-4 classic model

/**
 * Endianness options
 */
export type Endianness = 'little' | 'big' | 'native';

/**
 * Checksum modes
 */
export type ChecksumMode = 'none' | 'fletcher32';

/**
 * Chunk modes
 */
export type ChunkMode = 'contiguous' | 'chunked';

/**
 * Fill modes
 */
export type FillMode = 'fill' | 'nofill';

/**
 * Represents a NetCDF attribute
 */
export class Attribute {
  /**
   * The name of the attribute
   */
  name: string;

  /**
   * The value of the attribute
   */
  value: any;

  /**
   * Delete this attribute
   */
  delete(): void;

  /**
   * Inspect the attribute
   */
  inspect(): string;
}

/**
 * Represents a NetCDF dimension
 */
export class Dimension {
  /**
   * The dimension ID
   */
  readonly id: number;

  /**
   * The length of the dimension
   */
  readonly length: number;

  /**
   * The name of the dimension
   */
  name: string;

  /**
   * Inspect the dimension
   */
  inspect(): string;
}

/**
 * Represents a NetCDF variable
 */
export class Variable {
  /**
   * The variable ID
   */
  readonly id: number;

  /**
   * The data type of the variable
   */
  readonly type: NetCDFDataType;

  /**
   * The dimensions of the variable
   */
  readonly dimensions: Dimension[];

  /**
   * The attributes of the variable
   */
  readonly attributes: { [name: string]: Attribute };

  /**
   * The name of the variable
   */
  name: string;

  /**
   * The endianness of the variable
   */
  endianness: Endianness;

  /**
   * The checksum mode
   */
  checksumMode: ChecksumMode;

  /**
   * The chunk mode
   */
  chunkMode: ChunkMode;

  /**
   * The chunk sizes
   */
  chunkSizes: number[];

  /**
   * The fill mode
   */
  fillMode: FillMode;

  /**
   * The fill value
   */
  fillValue: any;

  /**
   * Whether compression shuffle is enabled
   */
  compressionShuffle: boolean;

  /**
   * Whether compression deflate is enabled
   */
  compressionDeflate: boolean;

  /**
   * The compression level (0-9)
   */
  compressionLevel: number;

  /**
   * Read the entire variable
   */
  read(): any;

  /**
   * Read a slice of the variable
   * @param start - Starting indices for each dimension
   * @param count - Number of elements to read in each dimension
   */
  readSlice(start: number[], count: number[]): any;

  /**
   * Read a strided slice of the variable
   * @param start - Starting indices for each dimension
   * @param count - Number of elements to read in each dimension
   * @param stride - Stride for each dimension
   */
  readStridedSlice(start: number[], count: number[], stride: number[]): any;

  /**
   * Write data to the entire variable
   * @param data - Data to write
   */
  write(data: any): void;

  /**
   * Write data to a slice of the variable
   * @param start - Starting indices for each dimension
   * @param count - Number of elements to write in each dimension
   * @param data - Data to write
   */
  writeSlice(start: number[], count: number[], data: any): void;

  /**
   * Write data to a strided slice of the variable
   * @param start - Starting indices for each dimension
   * @param count - Number of elements to write in each dimension
   * @param stride - Stride for each dimension
   * @param data - Data to write
   */
  writeStridedSlice(start: number[], count: number[], stride: number[], data: any): void;

  /**
   * Add an attribute to the variable
   * @param name - Attribute name
   * @param type - Data type
   * @param value - Attribute value
   */
  addAttribute(name: string, type: NetCDFDataType, value: any): Attribute;

  /**
   * Inspect the variable
   */
  inspect(): string;
}

/**
 * Represents a NetCDF group
 */
export class Group {
  /**
   * The group ID
   */
  readonly id: number;

  /**
   * Variables in this group
   */
  readonly variables: { [name: string]: Variable };

  /**
   * Dimensions in this group
   */
  readonly dimensions: { [name: string]: Dimension };

  /**
   * The unlimited dimension in this group
   */
  readonly unlimited: Dimension | null;

  /**
   * Attributes in this group
   */
  readonly attributes: { [name: string]: Attribute };

  /**
   * Subgroups in this group
   */
  readonly subgroups: { [name: string]: Group };

  /**
   * The name of the group
   */
  readonly name: string;

  /**
   * The full name (path) of the group
   */
  readonly fullname: string;

  /**
   * Add an attribute to the group
   * @param name - Attribute name
   * @param type - Data type
   * @param value - Attribute value
   */
  addAttribute(name: string, type: NetCDFDataType, value: any): Attribute;

  /**
   * Add a dimension to the group
   * @param name - Dimension name
   * @param length - Dimension length (0 for unlimited)
   */
  addDimension(name: string, length: number): Dimension;

  /**
   * Add a subgroup to the group
   * @param name - Subgroup name
   */
  addSubgroup(name: string): Group;

  /**
   * Add a variable to the group
   * @param name - Variable name
   * @param type - Data type
   * @param dimensions - Array of dimension names or Dimension objects
   */
  addVariable(name: string, type: NetCDFDataType, dimensions: (string | Dimension)[]): Variable;

  /**
   * Inspect the group
   */
  inspect(): string;
}

/**
 * Represents a NetCDF file
 */
export class File {
  /**
   * The root group of the file
   */
  readonly root: Group;

  /**
   * Open or create a NetCDF file
   * @param filename - Path to the file
   * @param mode - File mode ('r', 'w', 'c', 'c!')
   * @param format - File format (optional, defaults to 'nodenetcdf')
   */
  constructor(filename: string, mode: FileMode, format?: FileFormat);

  /**
   * Synchronize the file to disk
   */
  sync(): void;

  /**
   * Close the file
   */
  close(): void;

  /**
   * Inspect the file
   */
  inspect(): string;
}
