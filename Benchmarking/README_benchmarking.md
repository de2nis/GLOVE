# GLOVE Benchmarking

## glmark2

[glmark2](https://github.com/glmark2/glmark2) is an OpenGL 2.0 and ES 2.0 benchmark.
To clone this repository:
```
git clone https://github.com/glmark2/glmark2.git
```
Build and Install procedure can be found in README files of this repository. Note that _'--with-flavors=x11-glesv2'_ must be used in build configuration.

To run glmark2 benchmark use this command:
```
<path to glmark2-es2 executable>/glmark2-es2 --reuse-context -f <path to GLOVE root>/Benchmarking/glmark/glmark2_benchmarks_options
```

Note:
* "reuse-context" option is needed at this phase since GLOVE does not fully support multiple contexts yet
* glmark2\_benchmarks\_options contain a list of the so far supported benchmarks by GLOVE
