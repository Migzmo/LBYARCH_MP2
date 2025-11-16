nasm -f win64 assemblyComputeAccelerations.asm -o assemblyComputeAccelerations.obj
gcc -c cAcceleration.c -m64 -o cAcceleration.obj
gcc cAcceleration.obj assemblyComputeAccelerations.obj -o cAcceleration.exe -m64
cAcceleration.exe