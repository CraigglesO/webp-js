const { decodeRGBA } = require("./lib/binding.js")
const fs = require('fs')
const { PNG } = require('pngjs')

const pngData = fs.readFileSync('/media/craiggles/S2Terrain/dem-v3/0/12/34/694.png')
const png = PNG.sync.read(pngData)
console.log(png)
const pngUint8 = new Uint8ClampedArray(png.data)
console.log('pngUint8', pngUint8)

const webpData = fs.readFileSync('./webpExamples/demExample.webp')
const { buffer, width, height } = decodeRGBA(webpData.buffer)
const webpUint8 = new Uint8ClampedArray(buffer)
console.log('webpUint8', webpUint8)

let same = true

for (let i = 0, bl = pngUint8.length; i < bl; i++) {
  if (pngUint8[i] !== webpUint8[i]) {
    same = false
    break
  }
}

console.log('same', same)
