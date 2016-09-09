# Modified by Aroonav Mishra. Originally written by Anton Anastasov, Chirantan Ekbote and Travis Hance

add_llvm_loadable_module(BaggyBounds
  BaggyBoundsGlobals.cpp
  BaggyBoundsPointers.cpp
  BaggyLoopAnalysis.cpp
  Util.cpp
  BaggyBoundsPointersOpt.cpp
)
