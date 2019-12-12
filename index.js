// import native addon
const addonGreet = require('bindings')('greet');

console.log(addonGreet.greetHello('World'));

// expose module API
exports.hello = addonGreet.greetHello;