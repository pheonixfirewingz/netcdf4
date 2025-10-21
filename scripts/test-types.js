const { execSync } = require('child_process');

console.log('\nRunning TypeScript type checking...\n');

try {
  execSync('tsc test/typescript-test.ts --noEmit --skipLibCheck', { 
    stdio: 'inherit',
    cwd: process.cwd()
  });
  console.log('\n✅ TypeScript type checking passed!\n');
  process.exit(0);
} catch (error) {
  console.log('\n❌ TypeScript type checking failed!\n');
  process.exit(1);
}
