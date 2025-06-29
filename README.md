# C Service Base

[![Build and Deploy](https://github.com/briandowns/c_service_base/actions/workflows/main.yml/badge.svg)](https://github.com/briandowns/c_service_base/actions/workflows/compile_example.yml/badge.svg)

This repository is a base implementation for an HTTP JSON API in C. The structure is setup so it can be easily extended with API endpoints, configuration, and business logic.

## Components

* Application and API Request logging
* HTTP JSON API via Ulfius and Jansson with support for Basic and Token auth
* Makefile to build the project
* Dockerfile template ready to build images
* CI via Github Actions for building, testing, and spell check

## Requirements / Dependencies

* lulfius
* jansson
* orcania

## Contributing

Please feel free to open a PR!

## Contact

Brian Downs [@bdowns328](http://twitter.com/bdowns328)

## License

BSD 2 Clause [License](/LICENSE).
