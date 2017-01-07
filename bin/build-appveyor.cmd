:: Name: build-appveyor.cmd
:: Purpose: Support readable multi-line polly.py build commands
::
:: Multi-line commands are not currently supported directly in appveyor.yml files
::
:: See: http://stackoverflow.com/a/37647169

echo POLLY_ROOT %POLLY_ROOT%

python %POLLY_ROOT%\bin\polly.py ^
--verbose ^
--config "%1%" ^
--toolchain "%2%" ^
--test ^

