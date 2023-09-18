# TestApp

## Toolchain setup

## 1. Clone the repository ##
```
      git clone https://github.com/eclipse/kiso-testing-testapp.git
```

## 2. Run one of the following CMake generators from within the CMake build folder for the corresponding toolchain ##

### 2.1. GCC ###
```
      mkdir build_gcc && cd build_gcc
      cmake -G"Ninja" ../integration-testapp -DCMAKE_TOOLCHAIN_FILE=../integration-testapp/cmake/toolchains/GCC.cmake -DAPP_OSADAPTER=FreeRtosTestAdapter
```
### 2.3. ARM "Hardware toolchain: ARM32" ###
```
      mkdir build_arm && cd build_arm
      cmake -G"Ninja" ../integration-testapp -DCMAKE_TOOLCHAIN_FILE=../integration-testapp/cmake/toolchains/stm32-arm-gcc-f401re.cmake -DAPP_OSADAPTER=FreeRtosAdapter
```
#### Or, To wrap unit tests with ARM emulator in Linux `qemu` : ####
```
      mkdir build_arm && cd build_arm
      cmake -G"Ninja" ../integration-testapp -DCMAKE_TOOLCHAIN_FILE=../integration-testapp/cmake/toolchains/unittest-qemu-arm.cmake -DAPP_OSADAPTER=FreeRtosTestAdapter
```

## 3. Run Ninja targets ##

### 3.1. Build TestApp project ###
```
      ninja
```
### 3.2. Run unit test targets ###
Build-only all unitt tests:
```
      ninja build-unit-tests-all
```
Build and Run all unit tests:
```
      ninja run-unit-tests-all
```
