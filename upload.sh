#!/bin/bash
make -B
pros upload --slot 1 --after none --target v5 --name "Basic"
make -B EXTRA_CXXFLAGS="-D RECORD_MATCH" EXTRA_CXXFLAGS="-D RECORD_MATCH"
pros upload --slot 2 --after none --target v5 --name "Record"
make -B EXTRA_CXXFLAGS="-D REPLAY_MATCH" EXTRA_CXXFLAGS="-D REPLAY_MATCH"
pros upload --slot 3 --after none --target v5 --name "Replay"
