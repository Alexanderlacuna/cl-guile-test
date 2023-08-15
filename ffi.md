# Notes on foreign function interfaces


## foreign libraries
dynamically linked loaded system libraries written in low level languages durinr runtime

example snappy 

distinct from dynamic linked which occurs before start of a program


## Foreign extensions
one way to use shared libraries is to extend guile.

Such loadable modules generally define one distinguished initiliaztion function that when called will use the libguile API to define procedure  in the current module,

```c
//bessel.c
#include <math.h>
#include <libguile.h>
SCM
jo_wrapper (SCM x)
{
	return scm_from_double(jo (scm t double (x "j0")))
}


void 
init_math_bessel(void) 
{
	scm_c_define_gsubr ("jo", 1 ,0,0 ,jo_wrapper)
}


```


compiling the c library to a shared llb


```bash
gcc -shared -o bessel.so -fPIC bessel.c
```

## Foreign pointers


foreign libraries are essentially key-value mappings,where the key are names of definition and the values are ethe addresses of those defintions 

to look up the address of a definition use
;;foreign-library-pointer 

cccl-lmdp

gn-dataset-dump

//open
//get-db

//put-db

lmdbb lisp

code smell


data computation 
 actions 


 gn_typee -> tt list

 i ->output 



 lines -> strains if true


 interface with c code