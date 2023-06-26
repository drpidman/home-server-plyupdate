Startup script for Jellyfin home server to update PlutoTV playlist from https://i.mjh.nz/

## Requires
    - boost-dev
    - libcurl-dev
    - Directory in home user /home/user/infra/i.mjh.nz/PlutoTV/


Building
```sh
cd build 
cmake ..
cmake --build . 
```

Running
```sh
build/robus-infra-cmake
```