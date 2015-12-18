#! /bin/bash

#Se inicia el Parser y el Lexer
bison -d robython.y
flex robython.l
g++ robython.tab.c lex.yy.c -lfl -o robython

#Se obtiene el nombre del programa de código intermedio.
NOMBRE=`./robython`

#Se manda al traductor para que genere el código final
echo $NOMBRE | ./translate

##Genera el nombre final
NOMBRE_INO=$NOMBRE".ino"
echo $NOMBRE_INO

#Inicia proceso de carga al microcontrolador
cd arduino
mkdir PROGRAMS/$NOMBRE #Crea una carpeta con el nombre del programa en la carpeta PROGRAMAS
mv ../$NOMBRE_INO PROGRAMS/$NOMBRE #Mueve el archivo recién creado a la carpeta programas
mv ../$NOMBRE PROGRAMS/$NOMBRE
cp ../Robotec.h PROGRAMS/$NOMBRE #Copia la libreria *.h necesaria
cp ../Robotec.cpp PROGRAMS/$NOMBRE #Copia la libreria *.cpp necesaria
./arduino PROGRAMS/$NOMBRE/$NOMBRE_INO --upload
