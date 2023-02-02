let wasm_memory = new WebAssembly.Memory({
  initial: 256,  // 256 pages of 64kB
  maximum: 256,
});

let wasm_table = new WebAssembly.Table({
  'initial': 1,
  'maximum': 1,
  'element': 'anyfunc',
});

let asm_library_arg = {
  '__handle_stack_overflow': () => {},
  'emcripten_resize_heap': () => {},
  '__lock': () => {},
  '__unlock': () => {},
  'memory': wasm_memory,
  'tablel': wasm_table,
};

let info = {
  'env': asm_library_arg,
  'wasi_snapshot_preview1': asm_library_arg,
};

WebAssembly.instantiateStreaming(fetch('main.wasm'), info).then(module => {
  let result = module.instance.exports.add(12, 8);
  console.log(result);
});
