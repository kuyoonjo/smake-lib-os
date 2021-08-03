import { resolve } from 'path';
import { LLVM } from 'smake';

export function os(t: LLVM) {
  Object.defineProperty(t, 'sysIncludedirs', {
    value: [
      ...t.sysIncludedirs,
      resolve(__dirname, '..', 'os', 'include').replace(/\\/g, '/'),
    ],
    configurable: true,
  });
  Object.defineProperty(t, 'cxxflags', {
    value: [...t.cxxflags, '-D_CRT_SECURE_NO_WARNINGS'],
    configurable: true,
  });
}
