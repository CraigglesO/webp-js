const {
  getInfo,
  decodeRGB,
  decodeRGBA,
  encodeRGB,
  encodeRGBA,
  encodeLosslessRGB,
  encodeLosslessRGBA
} = require("../lib/binding.js")
const assert = require("assert")

// assert(WebpJs2, "The expected function is undefined")
// 
// function testBasic() {
//   const result = WebpJs2("hello")
//   assert.strictEqual(result, "world", "Unexpected value returned")
// }
//
// assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection")
//
// console.log("Tests passed- everything looks OK!")
