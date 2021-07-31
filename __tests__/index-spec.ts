import * as index from '../src/index';

test('Should have os available', () => {
  expect(index.os).toBeTruthy();
});
