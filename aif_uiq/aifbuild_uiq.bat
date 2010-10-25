echo off

set app_path=D:\Projects\BTSync\aif_uiq

@echo ######### Building UIQ #############
set EPOCROOT=\Symbian\6.1\Series60\

@echo ######### Building Aif Files... #########

call bmconv /himages.mbg images.mbm /c12context_pane_icon.bmp /c12context_pane_icon_mask.bmp /c12list_icon.bmp /c12list_icon_mask.bmp 

call aiftool ripplevault images.mbm

@echo ######### DONE ############

@pause
