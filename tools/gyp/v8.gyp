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
        '../../src/arguments.cpp',
        '../../src/array.cpp',
        '../../src/accessor_info.cpp',
        '../../src/accessor_signature.cpp',
        '../../src/auto_release_pool.cpp',
        '../../src/boolean.cpp',
        '../../src/context.cpp',
        '../../src/data.cpp',
        '../../src/date.cpp',
        '../../src/debug.cpp',
        '../../src/duk_stack_scope.cpp',
        '../../src/exception.cpp',
        '../../src/function.cpp',
        '../../src/function_template.cpp',
        '../../src/gcobject.cpp',
        '../../src/global_stash.cpp',
        '../../src/handle_scope.cpp',
        '../../src/heap_statistics.cpp',
        '../../src/integer.cpp',
        '../../src/isolate.cpp',
        '../../src/local.cpp',
        '../../src/message.cpp',
        '../../src/number.cpp',
        '../../src/object.cpp',
        '../../src/object_list.cpp',
        '../../src/object_template.cpp',
        '../../src/persistent.cpp',
        '../../src/platform-linux.cpp',
        '../../src/primitive.cpp',
        '../../src/script.cpp',
        '../../src/script_data.cpp',
        '../../src/script_origin.cpp',
        '../../src/signature.cpp',
        '../../src/stack_trace.cpp',
        '../../src/string.cpp',
        '../../src/string_object.cpp',
        '../../src/template.cpp',
        '../../src/try_catch.cpp',
        '../../src/utility.cpp',
        '../../src/value.cpp',
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

