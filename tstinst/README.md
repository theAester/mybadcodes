# TeST INSTructions [per second]

To test how many simple instructions a computer can do in one second.

Helps with analysing time complexity and what to expect before running a test.

## installation
Use the makefile:
``` bash
make
make installl
```
You can also modify the installation path with
``` bash
make INSTALL_PATH="yourpath"
```

## usage
There are three use cases to this tool:

1- ``` tstinst -p ```

The -p tag is optional. It extimates how many simple instructions your cpu can execute in 1 second according to "current process cpu time"

Useful for estimating result for online judgement

2- ``` tstinst -r  ```

It estimates how many simple instructions your cpu can execute in 1 second according to "real time"

Useful for estimating how many episodes of south park you can watch while the AI assignment code is being run

3- ``` tstinst -n [count] ``` 

It runs [count] simple instructions and records the time elapsed(according to curret process cpu time)

Also see ``` tstinst -h ```
