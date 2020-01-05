# Copyright 2019 Alexander Huber
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations 

CPPFILES=edge.cpp node.cpp nodeApp.cpp fileHandler.cpp message.cpp messageHandler.cpp networkApp.cpp  graphGenApp.cpp
COBJS=$(CFILES:%.c=obj/%.o)
CPPOBJS=$(CPPFILES:%.cpp=obj/%.o)
INCLUDE= -I include/
CC=gcc
CPPC=g++
CFLAGS = -Wall -pedantic
CPPFLAGS = ${CFLAGS} -std=c++17
LINKTARGET=../bin/networkApp
LINKTARGET2=../bin/NodeApp

all: $(CPPOBJS) networkApp nodeApp graphGen 

networkApp:
	$(CPPC) $(CPPFLAGS) ${INCLUDE} -o ../bin/networkApp obj/edge.o obj/node.o obj/fileHandler.o obj/message.o obj/messageHandler.o obj/networkApp.o -pthread

nodeApp:
	$(CPPC) $(CPPFLAGS) ${INCLUDE} -o ../bin/nodeApp obj/edge.o obj/node.o obj/fileHandler.o obj/message.o obj/messageHandler.o obj/nodeApp.o -pthread

graphGen:
	$(CPPC) $(CPPFLAGS) ${INCLUDE} -o ../bin/graphGenApp obj/edge.o obj/node.o obj/fileHandler.o obj/message.o obj/messageHandler.o obj/graphGenApp.o -pthread


obj/%.o : %.cpp
	$(CPPC) $(CPPFLAGS) ${INCLUDE} -o $@ -c $< 

# Cleans the directory

# *.o:
# 	rm -f ${OBJ}

clean:
	rm -Rf obj ../bin
	mkdir obj
	mkdir ../bin
