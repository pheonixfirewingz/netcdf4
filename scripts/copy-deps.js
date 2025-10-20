const fs = require('fs');
const path = require('path');
const os = require('os');

// Determine the platform-specific vcpkg directory
const platform = os.platform();
let vcpkgDir;

if (platform === 'win32') {
  vcpkgDir = 'vcpkg/installed/x64-windows/bin';
} else if (platform === 'linux') {
  vcpkgDir = 'vcpkg/installed/x64-linux/lib';
} else if (platform === 'darwin') {
  vcpkgDir = 'vcpkg/installed/x64-osx/lib';
} else {
  console.log('Unknown platform, skipping dependency copy');
  process.exit(0);
}

const buildDir = 'build/Release';

// Check if vcpkg directory exists
if (!fs.existsSync(vcpkgDir)) {
  console.log(`vcpkg directory not found: ${vcpkgDir}`);
  console.log('Skipping dependency copy - assuming system libraries are available');
  process.exit(0);
}

// Check if build directory exists
if (!fs.existsSync(buildDir)) {
  console.log(`Build directory not found: ${buildDir}`);
  console.log('Skipping dependency copy - build directory does not exist yet');
  process.exit(0);
}

// Copy shared libraries based on platform
try {
  const files = fs.readdirSync(vcpkgDir);
  let copied = 0;
  
  files.forEach(file => {
    const ext = path.extname(file);
    // Copy .dll on Windows, .so on Linux, .dylib on macOS
    if (ext === '.dll' || ext === '.so' || ext === '.dylib') {
      const src = path.join(vcpkgDir, file);
      const dest = path.join(buildDir, file);
      fs.copyFileSync(src, dest);
      copied++;
    }
  });
  
  console.log(`Copied ${copied} dependency files to ${buildDir}`);
} catch (error) {
  console.error('Error copying dependencies:', error.message);
  console.log('Continuing anyway - system libraries may be sufficient');
  process.exit(0);
}
