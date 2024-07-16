# graphex-lib V0.01

## A library for scalable Petrinet (and other graph execution).

With a selection of modules, a larger network can be assembled and run in parallel. It can be procedurally scaled by combining these modules. The graphex-lib allows for scaling and porting data in and out of this network.

## Current scope

.lpn files are imported from workcraft, added as modules, and run. Each transition group can be operated by one thread. There are multiple place groups that form shared memory. Some petrinet configurations are not thread safe (a transition must only ever have maximum one shared input and one shared output), however in scope any multithreading configuration is possible.

## Substitution method of building multithreadable, executable Petrinet

* Naming a place with a special prefix and format will register it as a shared place (across multiple transition groups)
* The first name slot within this naming convention can be substituted (such that you can generically connect up multiple modules by matching names)
  * Example: You have a 4 to 1 mux, and you wish to connect four fifo 1 to 1 pipelines to its inputs (x_in1_0, x_in1_1 ,, x_in2_0, x_in2_1 ,, x_in3_0, x_in3_1 ,, x_in4_0, x_in4_1) (dual rail logic, two places to represent one bit). By making a proceedural substitution to the fifo's output, such *x_out1_0* and *x_out1_1* becomes *x_in1_0* and *x_in1_1*, the two have been connected, and the same module can be reused for all its inputs (x_out1_0 can also become x_in2_0, and so on).

## TODO:

* Create a wizard class for common easy patterns.
* Add logic for piping data in or out.
* Refactor file format code, using method of populating concept structs, new formats should then be able to switched in.
