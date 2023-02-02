// let wasm_memory = new WebAssembly.Memory({
//   initial: 256,  // 256 pages of 64kB
//   maximum: 256,
// });

// let wasm_table = new WebAssembly.Table({
//   'initial': 1,
//   'maximum': 1,
//   'element': 'anyfunc',
// });

// let asm_library_arg = {
//   '__handle_stack_overflow': () => {},
//   'emcripten_resize_heap': () => {},
//   '__lock': () => {},
//   '__unlock': () => {},
//   'memory': wasm_memory,
//   'tablel': wasm_table,
// };

// let info = {
//   'env': asm_library_arg,
//   'wasi_snapshot_preview1': asm_library_arg,
// };

WebAssembly.instantiateStreaming(fetch('out.wasm'), info).then(module => {
  const width = 600;
  const height = 600;

  const canvas = document.getElementById("my_canvas");
  canvas.width = width;
  canvas.height = height;

  const buffer_address = module.instance.exports.BUFFER.value;
  const image = new ImageData(
      new Uint8ClampedArray(
        // module.instance.exports.memory.buffer,
        4 * width,
        buffer_address,
        4 * width * height,
      ),
      width,
  );

  const ctx = canvas.getContext("2d");

  module.instance.exports.go();

  ctx.putImageData(image, 0, 0);
});