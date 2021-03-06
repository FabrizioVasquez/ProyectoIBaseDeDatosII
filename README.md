# <div align="center">
  <h1>Proyecto 1 - Base de datos II </h1>
</div>

Integrantes
- Jean Miraval
- Fabrizio Vásquez
- Mateo Noel

# Tabla de contenido

- [Introducción](#Introducción)
    - [Objetivo del proyecto](#Objetivo-del-proyecto)
    - [Resultados que se esperan obtener](#Resultados-que-se-esperan-obtener)
- [Fundamente y describa las técnicas](#Fundamente-y-describa-las-técnicas)
    - [Explicación procedimental del proceso de inserción, eliminacióny búsqueda](#Explínimoicación-procedimental-del-proceso-de-inserción,-eliminación-y-búsqueda)
    - [Aspectos importantes de la implementación de dichas técnicas](#Aspectos-importantes-de-la-implementación-de-dichas-técnicas)
- [Resultados Experimentales](#Resultados-Experimentales)
- [Pruebas de uso de la aplicación](#Pruebas-de-uso-de-la-aplicación)
    - [Aspectos importantes de la implementación de dichas técnicas](#Aspectos-importantes-de-la-implementación-de-dichas-técnicas)
    - [Video de uso de la aplicación](#Video-de-uso-de-la-aplicación)


# Introducción

## Objetivos del proyecto
- Aprender a utilizar memoria secundaria.
- Entender las aplicaciones del uso de memoria secundaria.
- Comprender las diferentes técnicas de organización de archivos.
- Crear un sistema gestor de base de datos minimo. 
- SGBD mínimo posea operaciones inserción, busqueda, busqueda por rango y eliminación.

## Resultados que se esperan obtener
- Esperamos obtener registros o conjunto de registros en base a las consultas que se realizaremos.
- Conseguir un rendimiento promedio en la ejecución de las consultas.

## Fundamente y describa las técnicas
Las técnicas utilizadas para estructurar la base de datos son Sequential File y Extendible Hashing.

Sequantial file funciona a partir de dos archivos, uno en el que se guarda principalmente los registros y otro auxiliar. El archivo mantiene los registros ordenados a partir de un key.

Extendible hashing es un hash dinámico que reduce y aumenta su tamaño dependiendo de que y cuantos valores hay dentro de este.
La funcion hash genera una secuencia de bits, a partir de esta los registros se almacenan en buckets dependiendo del key.
A medida de que la cantidad de registros incrementan, los Buckets y los dígitos del código Hash que identifican a cada Bucket se incrementan, a menos que se llegue a la profundidad global, momento en el que los Buckets se extienden a Buckets secundarios de overflow.

## Explicación procedimental del proceso de inserción, eliminacióny búsqueda
Sequantial File
- Inserción: Primero, localizas donde se inserta el nuevo registro. Si el espacio está libre, insertas. Sino insertas en el espacio auxiliar y actualizas punteros.
- Búsqueda: Se realiza una búsqueda binaria.

Extendible hashing
</p> Donde D es la profundidad global del índice. </p>
- Inserción: Primero se localiza el bucket en donde se debe insertar. Si encontró bucket y no está lleno, inserta.  Si el bucket esta lleno, dividir el bucket, creando nuevos con mayor profundidad y reinsertar todos los registros que estaban en este bucket. Si no se puede incrementar la profundidad se produce desbordamiento de buckets. Por otro lado, si no encontró bucket, buscas el de profundidad local mínima, y si no hay, creas el bucket e insertas el elemento en este.

- Búsqueda: Aplicar la función hash sobre la clave de búsqueda y obtener la secuencia D-bit. 
Se busca el valor binario donde pertenecería el key a buscar y dependiendo de este nos dirigimos al bucket correspondiente. Se va buscando primero el valor binario completo y se va acortando hasta que coincida con el valor del bucket ya que estos se construyen a partir del prefijo.

## Aspectos importantes de la implementación de dichas técnicas

# Resultados Experimentales

# Pruebas de uso de la aplicación
Como incialmente se tenía previsto trabajar con QT creator para la parte de la interfaz gráfica del SGBD minimo, los retornos de las funciones de insercion y busqueda retornaban archivos .csv para que QT creator las mostrará al usuario; sin embargo no se logro vincular QT, ya que se no se reconocía los archivos generados por nuestro  programa por lo que se construyo de manera provisional un menu en la consola al ejecutar directamente archivo main.cpp.

<div align="center"> 
  <img src="img/SGDBMin.png" width="600">
</div>

Provisional

Dataset basico
<div align="center"> 
  <img src="img/dataProvisional.png" width="400">
</div>

Menu provisional
<div align="center"> 
  <img src="img/MenuProvisional.png" width="400">
</div>

Resultado de consulta Busqueda por rangos
<div align="center"> 
  <img src="img/rangeResult.png" width="400">
</div>

## Aspectos importantes de la implementación de dichas técnicas

Sequential File
- Se utiliza búsqueda binaria para obtener un registro en específico.
- En la busqueda por rango se usa como punto de partida de busqueda la primera key proporcionada por el usuario. 
- Los punteros durante una inserción se actualizan conforme al registro previo y al siguiente.

Extendible Hashing

- Se utiliza una función hash para asignar un valor binario a cada registro.
- En nuestra implementación, cada bucket está representado como un archivo independiente.
- para actualizar los Buckets, se reescribe por completo el archivo de index, para que queden registrado los Buckets actualizados.

## Video de uso de la aplicación



