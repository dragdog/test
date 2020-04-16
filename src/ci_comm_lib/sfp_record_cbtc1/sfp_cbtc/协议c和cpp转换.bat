@echo on
if exist *.cpp goto cpptoc
if exist *.c goto ctocpp
goto end

:cpptoc
md ..\csource
copy *.cpp ..\csource\*.c
copy *.h ..\csource\*.h
goto end

:ctocpp
md ..\cppsource
copy *.c ..\cppsource\*.cpp
copy *.h ..\cppsource\*.h
goto end

:end