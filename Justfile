config:
  cmake -Bbuild -H"."

build:
  cmake --build build/ --target wavr

run:
  ./build/wavr
