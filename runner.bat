nasm -f win64 assemacel.asm -o assemacel.obj
gcc -c cAcceleration.c -m64 -std=c99 -o cAcceleration.obj
gcc cAcceleration.obj assemacel.obj -o cAcceleration.exe -m64
cAcceleration.exe < input.txt
pause