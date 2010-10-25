echo off

set app_path=C:\Deepak\workspace\Symbian\ripplevault\secure
set sym_sdk=c:\Symbian\7.0s\Series60_v20\Epoc32\Release\armi\urel
set deploy_path=C:\Deepak\workspace\Symbian\ripplevault\deployment\sym7
set deploy_path2=C:\Deepak\workspace\Symbian\ripplevault\deployment\sym8
set deploy_path3=C:\Deepak\workspace\Symbian\ripplevault\deployment\sym7_wap

@echo ######### Building S60 7.0s #############
set EPOCROOT=\Symbian\7.0s\Series60_v20\

@echo ######### Cleaning up first... #########

del %sym_sdk%\secure.*
del %sym_sdk%\secure_s60.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Secure... #########

cd %app_path%\group_sym7

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\secure.app %deploy_path%\
copy %sym_sdk%\secure.r01 %deploy_path%\

copy %sym_sdk%\secure.app %deploy_path2%\
copy %sym_sdk%\secure.r01 %deploy_path2%\

copy %sym_sdk%\secure.app %deploy_path3%\
copy %sym_sdk%\secure.r01 %deploy_path3%\

@echo ######### DONE ############

@pause
