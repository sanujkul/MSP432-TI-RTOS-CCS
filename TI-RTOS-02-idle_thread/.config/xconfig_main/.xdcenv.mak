#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Applications/ti/simplelink_msp432p4_sdk_3_40_01_02/source;/Applications/ti/simplelink_msp432p4_sdk_3_40_01_02/kernel/tirtos/packages;/Users/sanujkul/CCSworkspace_v9/TI-RTOS-02-idle_thread/.config
override XDCROOT = /Applications/ti/ccs930/xdctools_3_60_02_34_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Applications/ti/simplelink_msp432p4_sdk_3_40_01_02/source;/Applications/ti/simplelink_msp432p4_sdk_3_40_01_02/kernel/tirtos/packages;/Users/sanujkul/CCSworkspace_v9/TI-RTOS-02-idle_thread/.config;/Applications/ti/ccs930/xdctools_3_60_02_34_core/packages;..
HOSTOS = MacOS
endif
