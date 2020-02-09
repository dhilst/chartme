# Overview

Charme make easy to plot realtime charts from command line. You
just fill intergers to it's stdin and it will send them over
websocket to the frontend wich do the live plotting.

# Installation

```
git clone THIS_REPO
cd chartme
yarn global add file:$PWD
```

Where `SOME_COMMAND` output integers. Here is an
example to plot memory usage:

# Usage

You need a command that outputs integers, then you pipe it
to charme. It will open port 3000 and listen for that
values. Each value is printed in the stdout before being
sended to the frontend

You can access the web chart at http://localhost:3000

Here is and example for plotting current memory usage

```
while free -b | awk '/Mem/ { print $3 }'; do sleep 1; done | chartme
```

Here is another example using bash $RANDOM

```
while :; do echo $RANDOM; sleep 1; done | chartme
```

# Contributing, 

Just open an issue or a pull request. The whole project was
migrated to ReasonML, there are lots of spaces for features,
and PRs are very welcome.

# License

Apache 2
