# This file is used with the GYP meta build system.
# http://code.google.com/p/gyp/
# To build try this:
#   svn co http://gyp.googlecode.com/svn/trunk gyp
#   ./gyp/gyp -f make --depth=`pwd` http_parser.gyp 
#   ./out/Debug/test 
{
  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      # TODO: hoist these out and put them somewhere common, because
      #       RuntimeLibrary MUST MATCH across the entire project
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
          },
        },
      }
    },
    'msvs_settings': {
      'VCCLCompilerTool': {
      },
      'VCLibrarianTool': {
      },
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
    'conditions': [
      ['OS == "win"', {
        'defines': [
          'WIN32'
        ],
      }]
    ],
  },

  'targets': [
      {
      'target_name': 'v8',
      'type': 'none',
      'include_dirs': [
        '../../include',
        '../../deps/duktape'
      ],
      'dependencies': ['v8_base'],
      'direct_dependent_settings': {
        'include_dirs': [
          '../../include',
          '../../deps/duktape'
        ],
      },
      'defines': [ 'HTTP_PARSER_STRICT=0' ],
      'sources': [],
      'conditions': [
        ['OS=="win"', {
          'msvs_settings': {
            'VCCLCompilerTool': {
              # Compile as C++. http_parser.c is actually C99, but C++ is
              # close enough in this case.
              'CompileAs': 2,
            },
          },
        }]
      ],
    },
    {
      'target_name': 'v8_base',
      'type': 'static_library',
      'include_dirs': [
        '../../include',
        '../../deps/duktape'
      ],
      'direct_dependent_settings': {
        'include_dirs': [ 
          '../../include',
          '../../deps/duktape',
        ],
      },
      'defines': [ 'HTTP_PARSER_STRICT=0' ],
      'sources': [ 
        '../../src/Arguments.cpp',
        '../../src/Array.cpp',
        '../../src/AccessorInfo.cpp',
        '../../src/AutoReleasePool.cpp',
        '../../src/Boolean.cpp',
        '../../src/Context.cpp',
        '../../src/Data.cpp',
        '../../src/Date.cpp',
        '../../src/Debug.cpp',
        '../../src/DukStackScope.cpp',
        '../../src/Exception.cpp',
        '../../src/Function.cpp',
        '../../src/FunctionTemplate.cpp',
        '../../src/GCObject.cpp',
        '../../src/GCObjectPool.cpp',
        '../../src/GlobalStash.cpp',
        '../../src/HandleScope.cpp',
        '../../src/HeapStatistics.cpp',
        '../../src/Integer.cpp',
        '../../src/Isolate.cpp',
        '../../src/Local.cpp',
        '../../src/Message.cpp',
        '../../src/Number.cpp',
        '../../src/Object.cpp',
        '../../src/ObjectList.cpp',
        '../../src/ObjectTemplate.cpp',
        '../../src/Persistent.cpp',
        '../../src/Primitive.cpp',
        '../../src/Script.cpp',
        '../../src/ScriptData.cpp',
        '../../src/ScriptOrigin.cpp',
        '../../src/Signature.cpp',
        '../../src/StackTrace.cpp',
        '../../src/String.cpp',
        '../../src/StringObject.cpp',
        '../../src/Template.cpp',
        '../../src/TryCatch.cpp',
        '../../src/Utility.cpp',
        '../../src/Value.cpp',
        '../../deps/duktape/duktape.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'msvs_settings': {
            'VCCLCompilerTool': {
              # Compile as C++. http_parser.c is actually C99, but C++ is
              # close enough in this case.
              'CompileAs': 2,
            },
          },
        }]
      ],
    }
  ]
}

