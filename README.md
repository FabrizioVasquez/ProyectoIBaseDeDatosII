# <div align="center">
  <h1>Proyecto 1 - Base de datos II </h1>
</div>


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
Las técnicas utilizadas para estructurar la base de datos son SequentialFile y Extendible Hashing.
SequentialFile funciona a partir de dos archivos, uno en el que se guardaprincipalmente los registros y otro auxiliar.
El archivo mantiene los registros ordenados a partir de un key. Si un valor no entra en el archivo principal este será colocado en el auxiliar, el orden igual se mantendrá a partir de punteros.

Extendible hashing es un hash dinámico que reduce y aumenta su tamaño dependiendo de que y cuantos valores hay dentro de este. La función hash genera una secuencia de bits,a partir de esta los registros se almacenan en buckets dependiendo del key.


## Explicación procedimental del proceso de inserción, eliminacióny búsqueda


## Aspectos importantes de la implementación de dichas técnicas

# Resultados Experimentales

# Pruebas de uso de la aplicación

<div align="center"> 
  <img src="img/SGDBMin.png" width="600">
</div>

## Aspectos importantes de la implementación de dichas técnicas

## Video de uso de la aplicación



