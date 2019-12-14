{
  "targets": [
    {
      "target_name": "c_addon_test",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ 
          "<!@(node -p \"require('fs').readdirSync('src').filter(f=>f.match(/\.(cpp|h|c)/)).map(f=>'src/'+f).join(' ')\")"
        ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}