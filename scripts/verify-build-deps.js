const fs = require('fs');
const path = require('path');
const os = require('os');

// Determine the platform-specific vcpkg directory
const platform = os.platform();
let triplet;

if (platform === 'win32') {
  triplet = 'x64-windows';
} else if (platform === 'linux') {
  triplet = 'x64-linux';
} else if (platform === 'darwin') {
  triplet = 'x64-osx';
} else {
  console.error(`Unsupported platform: ${platform}`);
  process.exit(1);
}

console.log('Verifying build dependencies...');

// Check if vcpkg_installed directory exists
const vcpkgInstalledDir = path.join('vcpkg_installed', triplet);
if (!fs.existsSync(vcpkgInstalledDir)) {
  console.error(`✗ ERROR: vcpkg installation directory not found: ${vcpkgInstalledDir}`);
  console.error('Please run: npm run preinstall');
  process.exit(1);
}

// Verify netcdf.h exists
const includeDir = path.join(vcpkgInstalledDir, 'include');
const netcdfHeader = path.join(includeDir, 'netcdf.h');

if (!fs.existsSync(netcdfHeader)) {
  console.error(`✗ ERROR: netcdf.h not found at: ${netcdfHeader}`);
  console.error('vcpkg installation may be incomplete');
  console.error('Please run: npm run preinstall');
  process.exit(1);
}

console.log(`✓ Found netcdf.h at: ${netcdfHeader}`);

// Verify library files exist
const libDir = path.join(vcpkgInstalledDir, 'lib');
let requiredLib;

if (platform === 'win32') {
  requiredLib = path.join(libDir, 'netcdf.lib');
} else {
  requiredLib = path.join(libDir, 'libnetcdf.a');
}

if (!fs.existsSync(requiredLib)) {
  console.error(`✗ ERROR: NetCDF library not found at: ${requiredLib}`);
  console.error('vcpkg installation may be incomplete');
  console.error('Please run: npm run preinstall');
  process.exit(1);
}

console.log(`✓ Found NetCDF library at: ${requiredLib}`);
console.log(`✓ Build dependencies verified for ${triplet}`);
