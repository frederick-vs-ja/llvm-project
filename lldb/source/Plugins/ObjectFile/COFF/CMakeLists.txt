add_lldb_library(lldbPluginObjectFileCOFF PLUGIN
  ObjectFileCOFF.cpp

  LINK_COMPONENTS
    BinaryFormat
    Object
    Support
  LINK_LIBS
    lldbCore
    lldbHost
    lldbSymbol
    lldbTarget
  )
