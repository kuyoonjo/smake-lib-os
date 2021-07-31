import { os } from '../src/os';
import { LLVM_Darwin } from 'smake';
import { addLibs } from '@smake/libs';
import { resolve, sep } from 'path';

test('os', () => {
  class A extends LLVM_Darwin {
    files = [];
  }
  const B = addLibs(A, os, os, os);
  const b = new B();
  const p = resolve(__dirname, '..', 'os', 'include').replace(
    new RegExp(sep, 'g'),
    '/'
  );
  expect(b.sysIncludedirs.includes(p)).toBe(true);
  expect(b.cxxflags.includes('-D_CRT_SECURE_NO_WARNINGS')).toBe(true);
  expect(b.sysIncludedirs.length).toBe(3);
});
