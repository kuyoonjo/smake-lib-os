const { LLVM, Toolchain } = require('smake');
const { readdirSync } = require('fs');
const { join } = require('path');
const { LibOS } = require('./lib');

const items = readdirSync(join(__dirname, 'test'))
  .filter(x => x.endsWith('.cc'))
  .map(x => {
    const name = x.replace(/\.cc$/, '');
    const item = new LLVM(name, 'arm64-apple-darwin');
    // const item = new LLVM(name, 'x86_64-pc-windows-msvc');
    // const item = new LLVM(name, 'aarch64-linux-gnu');
    item.files = ['test/' + x];  
    item.ldflags = [
      ...item.ldflags,
      '-static-libstdc++',
    ];
    LibOS.config(item);
    return item;
  });

class Test extends Toolchain {
  constructor() {
    super('test');
  }
  async generateCommands() {
    if (process.argv.includes('--compdb'))
      return [];
    return items.map(x => ({
      label: 'test ' + x.name,
      // cmd: x.outputPath,
      // cmd: 'wine ' + x.outputPath,
      cmd: 'linux arm64 ' + x.outputPath,
    }));
  }
}

module.exports = [
  ...items, 
  // new Test(),
];