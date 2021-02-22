Masternode config
=======================

Bitcreds allows controlling multiple remote Masternodes from a single wallet. The wallet needs to have a valid collateral output of 10000 BCRS for each Masternode and uses a configuration file named `masternode.conf` which can be found in the following data directory (depending on your operating system):
 * Windows: %APPDATA%\Bitcreds\
 * Mac OS: ~/Library/Application Support/Bitcreds/
 * Unix/Linux: ~/.bitcreds/

`masternode.conf` is a space separated text file. Each line consists of an alias, IP address followed by port, masternode private key, collateral output transaction id and collateral output index.

Example:
```
mn1 127.0.0.2:31000 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg c187ea281f8d21cb7cd6294e9321930f043d821cc4abcf07d8d1d21428f032d8 0
mn2 127.0.0.3:31000 92Da1aYg6sbenP6uwskJgEY2XWB5LwJ7bXRqc3UPeShtHWJDjDv adfafd4470fb94f081c10c25f573a8224ce425e07dc045e86639859c37ceab26 1
```

In the example above:
* the collateral of 10000 BCRS for `mn1` is output `0` of transaction [c187ea281f8d21cb7cd6294e9321930f043d821cc4abcf07d8d1d21428f032d8](https://explorer.bitcreds.org/tx/c187ea281f8d21cb7cd6294e9321930f043d821cc4abcf07d8d1d21428f032d8)
* the collateral of 10000 BCRS for `mn2` is output `1` of transaction [adfafd4470fb94f081c10c25f573a8224ce425e07dc045e86639859c37ceab26](https://explorer.bitcreds.org/tx/adfafd4470fb94f081c10c25f573a8224ce425e07dc045e86639859c37ceab26)

_Note: IPs like 127.0.0.* are not allowed actually, we are using them here for explanatory purposes only. Make sure you have real reachable remote IPs in you `masternode.conf`._

The following RPC commands are available (type `help masternode` in Console for more info):
* list-conf
* start-alias \<alias\>
* start-all
* start-missing
* start-disabled
* outputs
