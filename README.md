# Revolution Pi DataInOut JNI wrapper

Simple JNI wrapper to access the rev pi DIO hardware module from within Java  

## Getting Started

These instructions will get you a copy of the project on your rev pi and build the library on that controller.

## Built Instructions

* login to your raspberry pi and install the git tools
* git clone [https://github.com/clehne/revpi-dio-java](https://github.com/clehne/revpi-dio-java)
* cd librevpi-dio-java  
* make clean
* make all
* make check

## Deployment

Use the resulting library [./dist/librevpi-dio-java.jar](./dist/librevpi-dio-java.jar) for your needs.

## Contributing

* The basic project sceleton is copied from [waal70 - libsocket-can-java.git](https://github.com/waal70/libsocket-can-java.git)
* real rev pi hardware access code is taken from [RevPi - https://github.com/RevolutionPi/piControl](https://github.com/RevolutionPi/piControl)
* This library was developed to be part of the great and inspiring [openems](https://openems.io/) project. 

## Authors

* **C. Lehne** - *initial* - 

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
