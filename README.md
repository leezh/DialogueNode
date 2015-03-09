Dialogue Node
=============

This is an incomplete prototype that isn't ready for any real use at the
moment. Submit any bugs to (https://github.com/leezh/DialogueNode)

Dialogue Node is a conversation tree editor for games.

Each node can be connected *from* as many nodes as possible but can only
connect *to* a limited number of nodes. Text nodes can connect to only one,
while selection nodes are allowed only one for each choice.

## Building Instructions

To build this you need **Qt5** and any **C++11 compiler**. If you have
QtCreator (included with the Qt SDK) you can open the project and build it
from there. If you have *nix and want to build from the command line you
can instead use:

```Shell
qmake -o Makefile DialogueNode.pro && make
```

## License

Copyright (C) 2015  Zher Huei Lee (leezh@leezh.net)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see (http://www.gnu.org/licenses/).
 
