// TypeScript test file to verify type definitions
import { File, Group, Variable, Dimension, Attribute, FileMode, FileFormat, NetCDFDataType } from '../index';

// Test file creation
const file1 = new File('test.nc', 'c!', 'nodenetcdf');
const file2 = new File('test2.nc', 'r');

// Test root group access
const root: Group = file1.root;

// Test adding dimensions
const timeDim: Dimension = root.addDimension('time', 0); // unlimited
const latDim: Dimension = root.addDimension('lat', 180);
const lonDim: Dimension = root.addDimension('lon', 360);

// Test dimension properties
const dimId: number = timeDim.id;
const dimLength: number = latDim.length;
let dimName: string = lonDim.name;
dimName = 'longitude'; // should be settable
lonDim.name = 'longitude';

// Test adding variables
const tempVar: Variable = root.addVariable('temperature', 'float', [timeDim, latDim, lonDim]);
const presVar: Variable = root.addVariable('pressure', 'double', ['time', 'lat', 'lon']);

// Test variable properties
const varId: number = tempVar.id;
const varType: NetCDFDataType = tempVar.type;
const varDims: Dimension[] = tempVar.dimensions;
const varAttrs: { [name: string]: Attribute } = tempVar.attributes;

// Test variable settings
tempVar.name = 'temp';
tempVar.endianness = 'little';
tempVar.checksumMode = 'fletcher32';
tempVar.chunkMode = 'chunked';
tempVar.chunkSizes = [1, 90, 180];
tempVar.fillMode = 'fill';
tempVar.fillValue = -999.0;
tempVar.compressionShuffle = true;
tempVar.compressionDeflate = true;
tempVar.compressionLevel = 6;

// Test reading and writing
const data: any = tempVar.read();
tempVar.write([1, 2, 3, 4, 5]);

const sliceData: any = tempVar.readSlice([0, 0, 0], [1, 10, 10]);
tempVar.writeSlice([0, 0, 0], [1, 10, 10], sliceData);

const stridedData: any = tempVar.readStridedSlice([0, 0, 0], [1, 90, 180], [1, 2, 2]);
tempVar.writeStridedSlice([0, 0, 0], [1, 90, 180], [1, 2, 2], stridedData);

// Test attributes
const attr1: Attribute = tempVar.addAttribute('units', 'string', 'degrees_C');
const attr2: Attribute = root.addAttribute('title', 'string', 'Test NetCDF File');

const attrName: string = attr1.name;
const attrValue: any = attr1.value;
attr1.name = 'unit';
attr1.value = 'Celsius';
attr1.delete();

// Test subgroups
const subgroup: Group = root.addSubgroup('forecast');
const subgroupId: number = subgroup.id;
const subgroupVars: { [name: string]: Variable } = subgroup.variables;
const subgroupDims: { [name: string]: Dimension } = subgroup.dimensions;
const subgroupAttrs: { [name: string]: Attribute } = subgroup.attributes;
const subgroupSubgroups: { [name: string]: Group } = subgroup.subgroups;
const subgroupName: string = subgroup.name;
const subgroupFullname: string = subgroup.fullname;
const unlimitedDim: Dimension | null = subgroup.unlimited;

// Test file operations
file1.sync();
file1.close();

// Test inspect methods
const fileInspect: string = file1.inspect();
const groupInspect: string = root.inspect();
const varInspect: string = tempVar.inspect();
const dimInspect: string = timeDim.inspect();
const attrInspect: string = attr2.inspect();

console.log('TypeScript type checking passed!');
