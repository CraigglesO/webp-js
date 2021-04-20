const { encodeLosslessRGBA } = require("./lib/binding.js")
const fs = require('fs')
const { PNG } = require('pngjs')
const { Worker, parentPort, isMainThread } = require('worker_threads')
const log = require('single-line-log').stdout

const FACE = 0
const ZOOM = 12
const WORKER_COUNT = 12

let tileCount = 0

if (isMainThread) {
  const tiles = tileIterator()
  for (let w = 0; w < WORKER_COUNT; w++) {
    const worker = new Worker(__filename)

    worker.on('message', response => {
      const { status } = response
      if (status === 'ready') {
        const { value, done } = tiles.next()
        if (value) {
          const { face, zoom, x, y } = value
          log(tileCount++, `${face}-${zoom}-${x}-${y}`)
          worker.postMessage({ type: 'tile', value })
        } else {
          worker.terminate()
        }
      }
    })
  }
} else {
  parentPort.on('message', ({ type, value }) => {
    const { face, zoom, x, y } = value
    convertAndClean(face, zoom, x, y)
    parentPort.postMessage({ status: 'ready' })
  })
  parentPort.postMessage({ status: 'ready' })
}

function convertAndClean (face, zoom, x, y) {
  // pull in PNG
  const data = fs.readFileSync(`/media/craiggles/S2Terrain/dem-v3/${face}/${zoom}/${x}/${y}.png`)
  const png = PNG.sync.read(data)
  // grab data and convert
  const pngAB = png.data.buffer
  const res = encodeLosslessRGBA(pngAB, png.width, png.height)
  // store
  const webpBuffer = Buffer.from(res)
  fs.writeFileSync(`/media/craiggles/S2Terrain/dem-v3/${face}/${zoom}/${x}/${y}.webp`, webpBuffer)
  // remove old file
  fs.unlinkSync(`/media/craiggles/S2Terrain/dem-v3/${face}/${zoom}/${x}/${y}.png`)
}

function *tileIterator () {
  const xs = fs.readdirSync(`/media/craiggles/S2Terrain/dem-v3/${FACE}/${ZOOM}`)
  for (const x of xs) {
    const X = +x
    const ys = fs.readdirSync(`/media/craiggles/S2Terrain/dem-v3/${FACE}/${ZOOM}/${X}`)
    for (const y of ys) {
      let [Y, fileType] = y.split('.')
      if (fileType === 'png') {
        Y = +Y
        yield { face: FACE, zoom: ZOOM, x: X, y: Y }
      }
    }
  }
}
