const { decodeRGBA } = require("./lib/binding.js")
const fs = require('fs')
const { PNG } = require('pngjs')


const data = fs.readFileSync('./webpExamples/raster.webp')
const { buffer, width, height } = decodeRGBA(data.buffer)
const uint8clamped = new Uint8ClampedArray(buffer)
console.log(uint8clamped)

const png = new PNG({ width, height })
png.data = uint8clamped

const pngBuffer = PNG.sync.write(png)
// console.log(pngBuffer)

fs.writeFileSync('./webpExamples/testDecode.png', pngBuffer)
