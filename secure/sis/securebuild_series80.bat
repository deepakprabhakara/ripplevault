echo off

set app_path=D:\Debug\Asurion\Secure
set sym_sdk=D:\Symbian\7.0s\S80_DP2_0_SDK\Epoc32\Release\armi\urel
set deploy_path=D:\Debug\Asurion\Secure\sis

@echo ######### Building S60 7.0s #############
set EPOCROOT=\Symbian\7.0s\S80_DP2_0_SDK\

@echo ######### Cleaning up first... #########

del %sym_sdk%\secure.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Secure... #########

cd %app_path%\group_s80

call bldmake bldfiles

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

@echo ######### DONE ############

@pause
