OPCIONES DE EJECUCIÓN:

Es necesario crear las carpetas bin, lib y obj

Debemos hacer 'make' para ejecutar el makefile.

1º:
    Ejecutar todos los casos a la vez y generar el archivo con los resultados 'resultados.txt'
        
        chmod +x script.sh          # Damos permisos de ejecución
        
        ./script.sh                 # Ejecutamos script.sh

2º:
    Desde la carpeta bin/ 
        
        ./[algoritmo] < ../datos_MDD/GKD-caso
   o
   
        ./[algoritmo] < prueba.txt

Para borrar el ejecutable y los archivos .o utilizar 'make clean'.
