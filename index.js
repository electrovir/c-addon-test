// import native addon
const addonGreet = require('bindings')('greet');

// console.log(addonGreet.greetHello('World'));
// console.log(addonGreet.test());

let playing = false;
var stdin = process.stdin;

// without this, we would only get streams once enter is pressed
stdin.setRawMode(true);

// resume stdin in the parent process (node app won't quit all by itself
// unless an error or process.exit() happens)
stdin.resume();

// i don't want binary, do you?
stdin.setEncoding('utf8');

let wait = 20;
// on any data into stdin
stdin.on('data', function (key) {
    // ctrl-c ( end of text )
    if (key === '\u0003') {
        process.exit();
    }
    // write the key to stdout all normal like
    if (key === '=') {
        wait--;
        if (wait < 0) {
            wait = 0;
        }
    } else if (key === '-') {
        wait++;
    }
});


function flatten2dArray(inputArray) {
    return inputArray.reduce((flattened, innerArray) => flattened.concat(innerArray), [])
}

// let values = flatten2dArray([
//     [0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
//     [0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
//     [0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0],
//     [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0],
//     [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0],
//     [0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0],
//     [0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
//     [0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0]
// ]);
let values = flatten2dArray(Array(8).fill(0).map((_, index) => Array(32).fill(index % 12)));



function increment() {
    addonGreet.drawStill(8, 32, 100, values);
    values = values.map(value => (value + 1) % 12);
};

let lastTime = process.hrtime()[0];
let frameCount = 0;

function animate() {
    setTimeout(() => {
        animate();
    }, wait);

    increment();
    frameCount++;
    const newTime = process.hrtime()[0];
    if (newTime > lastTime) {
        const diff = lastTime - newTime;
        lastTime = newTime;
        // log the fps
        // getting 60 fps on raspberry pi
        console.log(frameCount);
        frameCount = 0;
    }
}

animate();

// expose module API
exports.hello = addonGreet.greetHello;
exports.test = addonGreet.test;
exports.drawStill = addonGreet.drawStill;