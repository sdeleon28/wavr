config:
  cmake -Bbuild -H"."

build:
  cmake --build build --target wavr

build-test:
  cmake --build build --target test

run:
  ./build/wavr/wavr > out_samples.txt
  cat out_samples.txt | grep LEFT | cut -d":" -f2 > left.txt
  cat out_samples.txt | grep RIGHT | cut -d":" -f2 > right.txt
  venv/bin/python play.py

run-test:
  ./build/test/test

