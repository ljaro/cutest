# cutest
A library for testing Qt applications with GUI. In general library consist of 2 parts - agent(c++) and server(python). 
### Install agent (c++)
 ```
 cd agent
 qmake CuTestAgent.pto
 make
 ```
 After that just use LD_PRELOAD mechanism
 ```
LD_PRELOAD=build/libTestRobotAgent.so your_application
```
### Install server (python)
All you need is just download repo and run `pip install .` 