idf_component_register(
    SRC_DIRS       #.c文件
    "."
   "BSP/."
    "CherryUSB/."
    "CherryUSB/Addition/."
    "CherryUSB/CherryUSB/class/cdc/."
    "CherryUSB/CherryUSB/class/hub/."
    "CherryUSB/CherryUSB/osal/."
    "CherryUSB/CherryUSB/core/."
    "CherryUSB/CherryUSB/port/dwc2/."

    INCLUDE_DIRS   #.h文件
    "."
    "BSP/."
     "CherryUSB/."
     "CherryUSB/Addition/."
     "CherryUSB/CherryUSB/class/cdc"
     "CherryUSB/CherryUSB/class/msc"
     "CherryUSB/CherryUSB/class/hub"
     "CherryUSB/CherryUSB/osal"
     "CherryUSB/CherryUSB/common"
     "CherryUSB/CherryUSB/core"
     "CherryUSB/CherryUSB/port/dwc2"
)
