const { LLVM, Toolchain } = require('smake');
const { readdirSync } = require('fs');
const { join } = require('path');
const { LibOS } = require('./lib');

const items = readdirSync(join(__dirname, 'test'))
  .filter(x => x.endsWith('.cc'))
  .map(x => {
    const name = x.replace(/\.cc$/, '');
    const item = new LLVM(name, 'x86_64-linux-gnu');
    item.files = ['test/' + x];
    LibOS.config(item);
    return item;
  });

class Test extends Toolchain {
  constructor() {
    super('test');
  }
  async generateCommands() {
    return items.map(x => ({
      label: 'test ' + x.name,
      cmd: x.outputPath,
    }));
  }
}

module.exports = [...items, new Test()];