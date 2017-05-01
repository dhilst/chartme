Work in progress:

Basic usage: 

```
git clone THIS_REPO
cd chartme
npm install
SOME_COMMAND | npm start
```

Where `SOME_COMMAND` output integers. Here is an
example to plot memory usage:

```
while free -b | awk '/Mem/ { print $3 }'; do sleep 1; done | npm start
```

License: Apache 2
