# Introduction

lib_buddy_allocator contains the code for the buddy allocator system.

pass contains the following LLVM passes:
* baggy-globals - Insert code to initialize global state variables like the baggy size table
* baggy-pointers - Instrument all pointer arithmetic to use baggy bounds checking and insert a branch to the slow path function if the bounds check fails. Additionally, perform bounds checking before LLVM intrinsics like llvm.memset
