[![Travis (.org)](https://img.shields.io/travis/FNickRU/Intellect-Battle.svg?style=popout)](https://travis-ci.org/FNickRU/Intellect-Battle)
[![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed/FNickRU/Intellect-Battle.svg?style=popout)](https://github.com/FNickRU/Intellect-Battle/pulls)
[![GitHub (pre-)release](https://img.shields.io/github/release/FNickRU/Intellect-Battle/all.svg?style=popout)](https://github.com/FNickRU/Intellect-Battle/releases)

# Intellect-Battle

"Intellect-Battle" is a group educational project to create a multiplayer network game.

The rules are simple: groups of players from 2 to 4 people compete in erudition, trying to answer the maximum number of questions that are contained on the server. One wrong answer is you're out. Could "survive" to the end of the game or just stayed last - you won!

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

To install and run the project, you need utilities make and gcc, as well as the libraries ncurses and cUnit. To install them, use your package manager.

For Ubuntu, use the following command:

```
sudo apt-get install make gcc libcunit1-dev libncurses5-dev libncursesw5-dev
```

### Installing

In order to install the game on your computer, you can either take the binaries from the latest [release](https://github.com/FNickRU/Intellect-Battle/releases), or build them from the sources.

To build from the sources, a simple command is enough:

```
make / make all
```

in the project's root folder.

After the successful assembly, the executable files of the server and the client should appear in the bin/ folder.

## Running the tests

To test the correct operation of the server and client parts, you can run unit tests. You can use the command to build and run them:

```
make test
```

from the root directory of the project.

## Deployment and execution

For the correct execution of the server, it is necessary that in a relative way:

```
<exec_path>/units/
```

there were units with questions that are used during the game.

There are no additional requirements for the correct execution of the client.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/FNickRU/Intellect-Battle/tags). 

## Authors

* **Stanislav Mayorov** - [FNickRU](https://github.com/FNickRU)
* **Michail Silkin** - [DeyMass](https://github.com/DeyMass)
* **Pavel Fedotov** - [AziRo](https://github.com/AziRo)
* **Ivan Sanin** - [Monstruos](https://github.com/Monstruos)
* **Evgeniy Chentsov** - [drenzoo](https://github.com/drenzoo)

## License

This project is unlicensed under the Unlicense - see the [LICENSE.md](LICENSE.md) file for details
