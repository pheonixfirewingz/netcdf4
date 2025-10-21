{
  "targets": [
    {
      "sources": [
        "src/Group.cpp",
        "src/File.cpp",
        "src/Variable.cpp",
        "src/Dimension.cpp",
        "src/Attribute.cpp",
        "src/netcdf4js.cpp"
      ],
      "target_name": "netcdf4",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "cflags": [ "-std=c++20" ],
      "cflags_cc": [ "-std=c++20" ],
      "conditions": [
        ['OS=="win"', {
          "variables": {
            "vcpkg_installed%": "<(module_root_dir)/vcpkg/installed/x64-windows",
            "netcdf_include%": "<!(node -p \"require('path').resolve(process.cwd(), 'vcpkg', 'installed', 'x64-windows', 'include')\")",
            "netcdf_lib%": "<!(node -p \"require('path').resolve(process.cwd(), 'vcpkg', 'installed', 'x64-windows', 'lib')\")"
          },
          "include_dirs": [
            "<(vcpkg_installed)/include",
            "<(netcdf_include)"
          ],
          "libraries": [
            "<(vcpkg_installed)/lib/netcdf.lib"
          ],
          "msvs_settings": {
            "VCLinkerTool": {
              "AdditionalLibraryDirectories": [
                "<(vcpkg_installed)/lib",
                "<(netcdf_lib)"
              ]
            },
            "VCCLCompilerTool": {
              "AdditionalOptions": [ "/std:c++20" ],
              "ExceptionHandling": 1,
              "AdditionalIncludeDirectories": [
                "<(vcpkg_installed)/include",
                "<(netcdf_include)"
              ]
            }
          },
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ],
          "msbuild_toolset": "v143"
        }],
        ['OS=="linux"', {
          "variables": {
            "vcpkg_installed%": "<(module_root_dir)/vcpkg/installed/x64-linux"
          },
          "include_dirs": [
            "<(vcpkg_installed)/include"
          ],
          "libraries": [
            "<(vcpkg_installed)/lib/libnetcdf.a",
            "<(vcpkg_installed)/lib/libhdf5_hl.a",
            "<(vcpkg_installed)/lib/libhdf5.a",
            "<(vcpkg_installed)/lib/libsz.a",
            "<(vcpkg_installed)/lib/libz.a",
            "<(vcpkg_installed)/lib/libcurl.a",
            "-ldl",
            "-lpthread"
          ]
        }],
        ['OS=="mac"', {
          "variables": {
            "vcpkg_installed%": "<(module_root_dir)/vcpkg/installed/x64-osx"
          },
          "include_dirs": [
            "<(vcpkg_installed)/include"
          ],
          "libraries": [
            "<(vcpkg_installed)/lib/libnetcdf.a",
            "<(vcpkg_installed)/lib/libhdf5_hl.a",
            "<(vcpkg_installed)/lib/libhdf5.a",
            "<(vcpkg_installed)/lib/libsz.a",
            "<(vcpkg_installed)/lib/libz.a",
            "<(vcpkg_installed)/lib/libcurl.a"
          ],
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LIBRARY": "libc++",
            "MACOSX_DEPLOYMENT_TARGET": "10.15",
            "CLANG_CXX_LANGUAGE_STANDARD": "c++20"
          }
        }]
      ]
    }
  ]
}
