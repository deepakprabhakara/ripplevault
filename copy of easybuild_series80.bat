echo off

set app_path=C:\Deepak\workspace\Symbian\ripplevault
set sym_sdk=c:\Symbian\7.0s\S80_DP2_0_SDK\Epoc32\Release\armi\urel
set deploy_path=%app_path%\deployment\series80

@echo ######### Building Series 80 #############
set EPOCROOT=\Symbian\7.0s\S80_DP2_0_SDK\

@echo ######### Cleaning up first... #########

@echo del %sym_sdk%\ripplevault.*
@echo del %sym_sdk%\ripplevault_series80.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\group_series80

call bldmake bldfiles

@echo call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\ripplevault.app %deploy_path%\
copy %sym_sdk%\ripplevault_series80.r01 %deploy_path%\
copy %sym_sdk%\ripplevault_series80.r31 %deploy_path%\

@echo ######### DONE ############

@pause
