const { encodeRGBA } = require("./lib/binding.js")
const fs = require('fs')
const { PNG } = require('pngjs')

const data = fs.readFileSync('./webpExamples/raster.png')
const png = PNG.sync.read(data)

const pngAB = png.data.buffer
// const uint8 = new Uint8ClampedArray(pngAB)

console.log(pngAB, png.width, png.height)

const res = encodeRGBA(pngAB, png.width, png.height, 75)
console.log(res)

const webpBuffer = Buffer.from(res)
fs.writeFileSync('./webpExamples/testEncode.webp', webpBuffer)
