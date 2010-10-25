echo off

set app_path=C:\Deepak\workspace\Symbian\ripplevault\secure
set sym_sdk=c:\Symbian\uiq_21\Epoc32\Release\armi\urel
set deploy_path=C:\Deepak\workspace\Symbian\ripplevault\deployment\uiq

@echo ######### Building UIQ #############
set EPOCROOT=\Symbian\UIQ_21\

@echo ######### Cleaning up first... #########

del %sym_sdk%\Secure.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Secure... #########

cd %app_path%\group_uiq\

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\Secure.app %deploy_path%\
copy %sym_sdk%\Secure_uiq.r01 %deploy_path%\secure.r01
copy %sym_sdk%\Secure_uiq.r31 %deploy_path%\secure.r31

@echo ######### DONE ############

@pause
