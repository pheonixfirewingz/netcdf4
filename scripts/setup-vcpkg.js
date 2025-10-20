const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');
const os = require('os');

// Allow users to skip vcpkg setup if they prefer system libraries
if (process.env.SKIP_VCPKG_SETUP === '1') {
  console.log('SKIP_VCPKG_SETUP is set, skipping vcpkg setup');
  console.log('Make sure NetCDF libraries are installed on your system');
  process.exit(0);
}

const platform = os.platform();

console.log('Setting up vcpkg and NetCDF dependencies...');

// Determine vcpkg triplet based on platform
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

function runCommand(cmd, options = {}) {
  console.log(`Running: ${cmd}`);
  try {
    execSync(cmd, { 
      stdio: 'inherit',
      ...options
    });
  } catch (error) {
    console.error(`Failed to run command: ${cmd}`);
    throw error;
  }
}

// Check if vcpkg directory exists
if (!fs.existsSync('vcpkg')) {
  console.log('vcpkg not found, cloning repository...');
  runCommand('git clone https://github.com/microsoft/vcpkg.git');
} else {
  console.log('vcpkg directory already exists');
}

// Bootstrap vcpkg
const vcpkgExecutable = platform === 'win32' ? 'vcpkg.exe' : 'vcpkg';
const vcpkgPath = path.join('vcpkg', vcpkgExecutable);

if (!fs.existsSync(vcpkgPath)) {
  console.log('Bootstrapping vcpkg...');
  if (platform === 'win32') {
    runCommand('.\\vcpkg\\bootstrap-vcpkg.bat', { shell: true });
  } else {
    runCommand('./vcpkg/bootstrap-vcpkg.sh', { shell: true });
  }
} else {
  console.log('vcpkg already bootstrapped');
}

// Install netcdf4 using vcpkg
console.log(`Installing netcdf-c for ${triplet}...`);
try {
  runCommand(`${vcpkgPath} install netcdf-c:${triplet}`);
  console.log('NetCDF dependencies installed successfully!');
} catch (error) {
  console.error('Failed to install netcdf-c');
  console.error('You may need to install build tools for your platform:');
  console.error('- Windows: Visual Studio Build Tools');
  console.error('- Linux: build-essential, cmake');
  console.error('- macOS: Xcode Command Line Tools');
  process.exit(1);
}

console.log('vcpkg setup complete!');
