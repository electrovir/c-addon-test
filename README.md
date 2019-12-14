# Creating a C addon

Made for running on a Raspberry Pi.

This uses the [node-addon-api](https://www.npmjs.com/package/node-addon-api) package to create a native C addon.

Following this guide: https://itnext.io/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32

Using this repo: https://github.com/thatisuday/native-addon-example

This being used to control an LED matric using this repo: https://github.com/jgarff/rpi_ws281x

This is also useful for creating a TS npm package: https://medium.com/@nahush.farkande/creating-npm-modules-written-in-typescript-8d3cb4e65783 (note that this package is not ts).

## Usage

Clone the repo on a pi then do the following:

```bash
npm run setup
npm run compile
npm test
```

In order to get the LED matrix to work, run as `su`:

```bash
sudo su
npm test
```


To update after changing c code:

```bash
npm run compile
npm run test
```