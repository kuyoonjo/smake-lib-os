import { resolve } from 'path';
import { LLVM } from 'smake';

export abstract class LibOS {
  static config(llvm: LLVM) {
    llvm.includedirs = [
      ...llvm.includedirs,
      resolve(__dirname, '..', 'include').replace(/\\/g, '/'),
    ];
    if (llvm.platform === 'win32')
      llvm.cxxflags = [...llvm.cxxflags, '-D_CRT_SECURE_NO_WARNINGS'];
  }
}
